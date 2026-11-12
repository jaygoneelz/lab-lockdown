#include "pch.h"
#include "Enemy.h"

#include "Components/Colliders.h"
#include "Components/MeshRenderer.h"
#include "Transform.h"
#include "CollisionController.h"
#include "Colliders.h"
#include "WaveSpawner.h"
#include "AudioManager.h"
#include "Player.h"
#include "Light.h"

// Reference to the player
Transform* playerTransform = nullptr;

void Enemy::Start()
{
	Puppet::Start();
	baseModelTilt = GetGameObject()->GetTransform()->fixedRotation.z;

	collider = GetGameObject()->AddComponent(new BoxCollider());
	collider->SetSize(glm::vec3(0.7f, height - 0.5f, 0.7f) * GetGameObject()->GetTransform()->scale);
	collisionController->colliders.push_back(collider);
	playerTransform = Player::playerTransform;
	warningLight = (new GameObject())->AddComponent(new Light());
	warningLight->type = LightType::Point;
	warningLight->color = { 1.0f, 0.06f, 0.025f };
	warningLight->intensity = 0.0f;
}

void Enemy::TakeDamage(float damage)
{
	Puppet::TakeDamage(CoreExposed() ? damage * 2.0f : damage);
	flinchRemaining = 0.2f;
}

void Enemy::Update(float deltaTime)
{
	auto t = GetGameObject()->GetTransform();
	if (t->position.y > 200.0f)
		return;	

	collider->SetPosition(t->position + glm::vec3(0, height * 0.5f, 0));
	collider->SetRotation(t->rotation);

	warningLight->GetGameObject()->GetTransform()->position = t->position + glm::vec3(0, 1.25f, 0);
	warningLight->intensity = 0.0f;
	warningLight->color = { 1.0f, 0.06f, 0.025f };
	AttackTimer = std::max(0.0f, AttackTimer - deltaTime);
	auto* mesh = GetGameObject()->GetComponent<MeshRenderer>();
	mesh->animationOffset = glm::vec3(0.0f);
	const bool heavy = attackDamage > 5;
	const float windupDuration = heavy ? 1.0f : 0.65f;
	const float recoveryDuration = heavy ? 0.65f : 0.34f;
	float poseTilt = 0.0f;
	glm::vec3 facing(0.0f);
	if (playerTransform)
	{
		facing = playerTransform->position - t->position;
		facing.y = 0.0f;
		if (glm::dot(facing, facing) > 0.001f)
			facing = glm::normalize(facing);
	}

	if (!IsDead() && entranceRemaining > 0.0f)
	{
		entranceRemaining = std::max(0.0f, entranceRemaining - deltaTime);
		const float progress = glm::smoothstep(0.0f, 1.35f, 1.35f - entranceRemaining);
		mesh->animationOffset.y = -0.8f * (1.0f - progress);
		t->fixedRotation.z = baseModelTilt + glm::radians(25.0f) * (1.0f - progress);
		warningLight->intensity = progress * 6.0f;
		return;
	}

	if (!IsDead() && recoveryRemaining > 0.0f)
	{
		recoveryRemaining = std::max(0.0f, recoveryRemaining - deltaTime);
		const float fade = recoveryRemaining / recoveryDuration;
		if (heavy)
		{
			warningLight->color = { 0.15f, 0.9f, 1.0f };
			warningLight->intensity = 5.0f * fade;
		}
		mesh->animationOffset = facing * (0.35f * fade);
		poseTilt = -17.0f * fade;
		velocity.x = velocity.z = 0.0f;
		velocity += gravity * deltaTime;
	}
	else if (!IsDead())
	{
		// Step 1: Decision-making (run decision tree)
		DecideState();
		if (currentState != EnemyState::Attack)
			windupRemaining = 0.0f;

		// Step 2: Perform behavior based on current state
		switch (currentState)
		{
		case EnemyState::Roaming:
			UpdatePatrolling(deltaTime);
			break;
		case EnemyState::Chasing:
			UpdateChasing(deltaTime);
			break;
		case EnemyState::Attack:
			UpdateAttack(deltaTime);
			if (recoveryRemaining > 0.0f)
			{
				mesh->animationOffset = facing * 0.35f;
				poseTilt = -17.0f;
			}
			else if (windupRemaining > 0.0f)
			{
				const float progress = 1.0f - windupRemaining / windupDuration;
				const float pullback = glm::smoothstep(0.0f, 0.8f, progress);
				const float strike = glm::smoothstep(0.82f, 1.0f, progress);
				mesh->animationOffset = facing * (-0.18f * pullback + 0.53f * strike);
				poseTilt = 12.0f * pullback - 29.0f * strike;
			}
			break;
		default:
			break;
		}

		velocity += gravity * deltaTime;
	}		
	else
	{
		if (!DeadOnce)
		{
			AudioManager::getInstance().playSound("robodead");
			DeadOnce = true;
			WaveSpawner::getInstance().EnemyRemaining--;
			Player::Score += 50;
			GetGameObject()->GetComponent<BoxCollider>()->enabled = false;
			collisionController->colliders.remove(collider);
		}		
		shutdownElapsed += deltaTime;
		const float fall = glm::smoothstep(0.08f, 0.65f, shutdownElapsed);
		mesh->animationOffset.y = -0.5f * fall;
		t->fixedRotation.z = baseModelTilt + glm::radians(72.0f * fall +
			6.0f * std::sin(shutdownElapsed * 55.0f) * (1.0f - fall));
		warningLight->intensity = 3.0f * (1.0f - fall);
		mesh->enabled = shutdownElapsed < 0.9f;
		return;
	}

	flinchRemaining = std::max(0.0f, flinchRemaining - deltaTime);
	t->fixedRotation.z = baseModelTilt + glm::radians(poseTilt) + glm::radians(13.0f) *
		std::sin(glm::pi<float>() * flinchRemaining / 0.2f);
	mesh->animationOffset -= facing * (0.12f * std::sin(glm::pi<float>() * flinchRemaining / 0.2f));
	HandlePhysics(deltaTime);
}

void Enemy::UpdatePatrolling(float deltaTime)
{
	if (patrolPoints.empty()) return; 

	
	glm::vec3 target = patrolPoints[currentPatrolIndex];
	glm::vec3 directionToTarget = glm::normalize(target - GetGameObject()->GetTransform()->position);

	LookInMovementDirection(directionToTarget, deltaTime);

	velocity = directionToTarget * moveSpeed;


	float distanceToTarget = glm::length(target - GetGameObject()->GetTransform()->position);
	if (distanceToTarget < patrolThreshold)
	{
		currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.size();
	}
}

void Enemy::UpdateChasing(float deltaTime)
{
	if (!playerTransform)
	{
		return;
		Debug::Log("No player");
	}	
	
	glm::vec3 directionToTarget = glm::normalize(playerTransform->position - GetGameObject()->GetTransform()->position);
	LookInMovementDirection(directionToTarget, deltaTime);

	float distanceToPlayer = glm::length(playerTransform->position - GetGameObject()->GetTransform()->position);

	if (distanceToPlayer > 1.5)
	{
		// Move toward the player
		glm::vec3 directionToPlayer = glm::normalize(playerTransform->position - GetGameObject()->GetTransform()->position);
		velocity = directionToPlayer * moveSpeed;
	}	
}

void Enemy::UpdateAttack(float deltaTime)
{
	velocity.x = velocity.z = 0.0f;
	if (AttackTimer > 0.0f)
		return;

	const bool heavy = attackDamage > 5;
	const float windupDuration = heavy ? 1.0f : 0.65f;
	auto position = GetGameObject()->GetTransform()->position;
	if (windupRemaining <= 0.0f)
	{
		windupRemaining = windupDuration;
		AudioManager::getInstance().playSound(heavy ? "heavycharge" : "dronecharge");
	}

	windupRemaining -= deltaTime;
	warningLight->intensity = 2.0f + 7.0f * (1.0f - windupRemaining / windupDuration);
	if (windupRemaining <= 0.0f)
	{
		Player::DamagePlayer(attackDamage, position);
		AttackTimer = TimeBetweenAttack;
		recoveryRemaining = heavy ? 0.65f : 0.34f;
		warningLight->intensity = 0.0f;
	}
}

void Enemy::DecideState()
{
	if (playerTransform)
	{
		float distanceToPlayer = glm::length(playerTransform->position - GetGameObject()->GetTransform()->position);

		if (distanceToPlayer < chaseDistance)
		{
			if (distanceToPlayer < 1.5)
			{
				currentState = EnemyState::Attack;
				return;
			}
			else
			{
				currentState = EnemyState::Chasing;
				return;
			}			
		}
	}

	currentState = EnemyState::Roaming; 
}

void Enemy::LookInMovementDirection(glm::vec3 direction, float deltatime)
{
	auto t = GetGameObject()->GetTransform();


	rotationAngle = glm::atan(-direction.z, direction.x) * (180 / 3.14f); // Target rotation

	t->rotation = glm::vec3{ 0.00f, glm::radians(rotationAngle), 0.0f };
}
