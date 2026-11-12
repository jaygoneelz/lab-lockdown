//Handles the logic of the player
#include "pch.h"
#include <SDL.h>
#include "glm/gtx/string_cast.hpp"

#include "Player.h"
#include "Enemy.h"
#include "Components/MeshRenderer.h"

#include "Scene.h"
#include "Components/CollisionController.h"
#include "Components/Transform.h"
#include "Components/CameraController.h"
#include "Rendering/Renderer.h"
#include "Shapes.h"
#include "Input.h"
#include "AudioManager.h"
#include "Flicker.h"

Transform* Player::playerTransform;

bool Player::ControlsEnabled = false;
Player::Equipment Player::Equipped = Equipment::Gun;

void Player::CycleEquipment(double scroll)
{
	if (!ControlsEnabled || IsplayerDead || scroll == 0.0)
		return;
	const int direction = scroll > 0.0 ? 2 : 1;
	Equipped = static_cast<Equipment>((static_cast<int>(Equipped) + direction) % 3);
}
Player::CombatFeedback Player::Feedback;
int Player::Totalbullets = 90;
int Player::Currentbullets = 6;
int Player::Magsize = 6;

int Player::PlayerHealth = 100;

int Player::Score = 0;

bool Player::IsplayerDead = false;

void Player::Start()
{
	Puppet::Start();

	playerTransform = GetGameObject()->GetTransform();
	cameraController = SceneManager::GetInstance().GetCurrentScene()->GetComponent<CameraController>();
	if (cameraController == nullptr)
		Debug::LogError("Player", "Camera controller not found.");
	else
		cameraController->enableMoving = false;	

	//flashlight creation
	auto lightGO = new GameObject();
	lightGO->GetTransform()->position = { 0.0f, 0.1f, 0.0f };
	weaponFlashlight = lightGO->AddComponent(new Light());
	weaponFlashlight->color = { 1.0f, 1.0f, 1.0f };
	weaponFlashlight->type = LightType::Spot;
	weaponFlashlight->SetSpotAngleDegrees(15.0f, 35.0f);
	weaponFlashlight->intensity = 5.0f;

	//muzzle flass
	auto lightGO1 = new GameObject();
	lightGO1->GetTransform()->position = { 0.0f, 0.1f, 0.0f };
	lightGO1->GetTransform()->scale = { 0.1f, 0.1f, 0.1f };
	muzzleFlashlight = lightGO1->AddComponent(new Light());
	muzzleFlashlight->color = { 1.0f, 0.5f, 0.1f };
	muzzleFlashlight->type = LightType::Point;
	muzzleFlashlight->SetSpotAngleDegrees(15.0f, 35.0f);
	muzzleFlashlight->intensity = 0.0f;

	auto createKnifePart = [](glm::vec3 size, glm::vec4 color) {
		auto* part = new GameObject();
		auto* mesh = part->AddComponent(new MeshRenderer());
		mesh->meshes.push_back(Shapes::CreateSimple(size));
		auto material = std::make_shared<Material>(Shader::Find("Base"));
		material->SetVec4("colorito", color);
		mesh->materials.push_back(material);
		mesh->castShadows = false;
		mesh->recieveShadows = false;
		mesh->enabled = false;
		return part;
	};
	knifeBlade = createKnifePart({0.032f, 0.011f, 0.22f}, {0.68f, 0.79f, 0.86f, 1.0f});
	knifeGrip = createKnifePart({0.047f, 0.037f, 0.10f}, {0.17f, 0.21f, 0.25f, 1.0f});

	Input::KeyChange += [](int key, bool down) {
		if (down && ControlsEnabled && !IsplayerDead && key >= GLFW_KEY_1 && key <= GLFW_KEY_3)
			Equipped = static_cast<Equipment>(key - GLFW_KEY_1);
	};

	// Events: fire Weapon
	Input::MouseButtonChange += [this](int button, bool isDown) {
		if (button == GLFW_MOUSE_BUTTON_1 && isDown)
			if (ControlsEnabled && !IsplayerDead)
			{
				if (Equipped == Equipment::Gun && CanFire)
					Fire();
				else if (Equipped == Equipment::Knife && slashRemaining <= 0.0f)
					Slash();
			}
						
	};

	//Flashlight ON/OFF
	Input::KeyChange += [this](int key, bool isDown) {
		if (key == GLFW_KEY_E && isDown)
		{
			if (ControlsEnabled && !IsplayerDead)
			{
				if (weaponFlashlight->intensity > 0.01f)
					weaponFlashlight->intensity = 0.0f;
				else
					weaponFlashlight->intensity = 8.0f;
				AudioManager::getInstance().playSound("flashlight");
			}			
		}
		};

	//Reload event
	Input::KeyChange += [this](int button, bool isDown) {
		if (button == GLFW_KEY_R && isDown)
		{
			if (ControlsEnabled && !IsplayerDead)
			{
				if (Equipped == Equipment::Gun)
					Reload();
			}			
		}
	};
}

void Player::Update(float deltaTime)
{
	Feedback.shotAge += deltaTime;
	Feedback.damageAge += deltaTime;
	auto t = GetGameObject()->GetTransform();

	if (cameraController != nullptr)
	{
		auto camTransform = cameraController->GetGameObject()->GetTransform();
		camTransform->position = t->position + glm::vec3(0, height, 0);

		t->rotation = { 0, camTransform->rotation.y, 0 };

		slashRemaining = std::max(0.0f, slashRemaining - deltaTime);
		const auto cameraRotation = glm::quat(camTransform->rotation);
		const bool knifeVisible = Equipped == Equipment::Knife;
		const float swing = std::sin(glm::pi<float>() * slashRemaining / 0.45f);
		const glm::vec3 knifeRotation = camTransform->rotation + glm::vec3(0.6f, -0.9f * swing, -0.28f);
		const glm::vec3 grip = camTransform->position + cameraRotation * glm::vec3(
			0.22f - 0.20f * swing, -0.22f, -0.56f - 0.13f * swing);
		knifeGrip->GetComponent<MeshRenderer>()->enabled = knifeVisible;
		knifeBlade->GetComponent<MeshRenderer>()->enabled = knifeVisible;
		knifeGrip->GetTransform()->position = grip;
		knifeBlade->GetTransform()->position = grip + glm::quat(knifeRotation) * glm::vec3(0, 0, -0.15f);
		knifeGrip->GetTransform()->rotation = knifeRotation;
		knifeBlade->GetTransform()->rotation = knifeRotation;

		if (weapon != nullptr)
		{
			weapon->GetComponent<MeshRenderer>()->enabled = Equipped == Equipment::Gun;
			auto weaponExtraRot = glm::vec3{ glm::radians(180.0), glm::radians(-270.0), -glm::radians(180.0) };
			auto weaponOffset = glm::quat(camTransform->rotation) * glm::vec3(0.015f, -0.005f, -0.018f);
			weapon->GetTransform()->fixedRotation = weaponExtraRot;
			glm::vec3 newWeaponPos = camTransform->position + weaponOffset;
			const float reloadProgress = 1.0f - glm::clamp(ReloadTimer / ReloadTime, 0.0f, 1.0f);
			glm::vec3 newWeaponRot = camTransform->rotation;
			const auto* movementKeys = Input::GetKeys();
			const bool walking = movementKeys[GLFW_KEY_LEFT_SHIFT] || movementKeys[GLFW_KEY_RIGHT_SHIFT];
			const float stride = strideBlend * (walking ? 0.45f : 1.0f);
			const float lower = glm::smoothstep(0.0f, 0.27f, reloadProgress) *
				(1.0f - glm::smoothstep(0.62f, 1.0f, reloadProgress));
			const float seat = glm::smoothstep(0.35f, 0.57f, reloadProgress) *
				(1.0f - glm::smoothstep(0.57f, 0.72f, reloadProgress));
			newWeaponPos += glm::quat(camTransform->rotation) * glm::vec3(
				0.002f * stride * std::sin(stridePhase),
				0.0017f * stride * std::cos(stridePhase * 2.0f) - landingDip - 0.009f * lower,
				recoil * 0.008f);
			newWeaponRot.x += recoil + glm::radians(36.0f * lower - 9.0f * seat);
			newWeaponRot.z += glm::radians(22.0f * lower + 2.0f * stride * std::sin(stridePhase));
			landingDip *= std::exp(-12.0f * deltaTime);
			recoil *= std::exp(-17.0f * deltaTime);

			// Weapon sway effect
			weapon->GetTransform()->position = glm::mix(weapon->GetTransform()->position + posChange, newWeaponPos, fminf(deltaTime * 15.0f, 1.0f));
			weapon->GetTransform()->position.y = newWeaponPos.y;
			weapon->GetTransform()->rotation = glm::mix(weapon->GetTransform()->rotation, newWeaponRot, deltaTime * 10.0f);

			//muzzle Light
			auto MuzzlelightT = muzzleFlashlight->GetGameObject()->GetTransform();
			MuzzlelightT->position = weapon->GetTransform()->position + weapon->GetTransform()->GetForwardVector() * 0.5f;
			MuzzlelightT->rotation = weapon->GetTransform()->rotation;

			//Flash Light
			auto lightT = weaponFlashlight->GetGameObject()->GetTransform();
			lightT->position = weapon->GetTransform()->position + weapon->GetTransform()->GetForwardVector() * 0.1f;
			lightT->rotation = weapon->GetTransform()->rotation;			

			FirerateHandler(deltaTime);			

			//muzzle flash
			if (JustFired)
			{
				if (MuzzleTimer > 0)
				{
					MuzzleTimer -= deltaTime;
					muzzleFlashlight->intensity = Equipped == Equipment::Gun ? 1.0f : 0.0f;
				}
				else
				{
					muzzleFlashlight->intensity = 0.0f;
					JustFired = false;
				}
			}
		}
	}

	HandleMovement(deltaTime);
	const glm::vec3 beforeMove = t->position;
	const bool wasGrounded = isGrounded;
	const float fallSpeed = velocity.y;
	HandlePhysics(deltaTime);
	if (!wasGrounded && isGrounded && fallSpeed < -1.0f)
		landingDip = std::min(0.006f, -fallSpeed * 0.001f);
	const glm::vec3 travel = t->position - beforeMove;
	const float travelDistance = glm::length(glm::vec2(travel.x, travel.z));
	stridePhase = std::fmod(stridePhase + travelDistance * 4.2f, glm::two_pi<float>());
	strideBlend = glm::mix(strideBlend, isGrounded && travelDistance > 0.001f ? 1.0f : 0.0f,
		1.0f - std::exp(-12.0f * deltaTime));
	if (isGrounded)
	{
		const glm::vec3 movement = t->position - beforeMove;
		stepDistance += glm::length(glm::vec2(movement.x, movement.z));
		const auto* keys = Input::GetKeys();
		const bool walking = keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT];
		if (stepDistance >= (walking ? 1.25f : 2.1f))
		{
			AudioManager::getInstance().playSound(walking ? "walkstep" : "step");
			stepDistance = 0.0f;
		}
	}
	
	//Renderer::DrawMeshWireframe(Shapes::CreateCube(glm::vec3(0.02f, 0.02f, 1.5f)), nullptr,
	//	world, CameraController::GetCurrent()->camera);
}

//Gun reload and fire rate control 
void Player::FirerateHandler(float deltaTime)
{	
	if (ReloadTimer <= 0)
	{
		if (firingTimer <= 0)
		{
			CanFire = true;
		}
		else
		{
			firingTimer -= deltaTime;
		}

	}
	else
	{
		ReloadTimer -= deltaTime;
		if (ReloadTimer <= 0.0f)
			AudioManager::getInstance().playSound("ready");
	}
}

void Player::Slash()
{
	if (!cameraController)
		return;
	slashRemaining = 0.45f;

	const auto* camera = cameraController->GetGameObject()->GetTransform();
	const glm::vec3 origin = camera->position;
	HitInfo hit{};
	if (!collisionController->Raycast(origin, cameraController->GetGameObject()->GetTransform()->GetForwardVector(),
		hit, collider) || glm::distance2(origin, hit.intersectionPoint) > 2.4f * 2.4f || !hit.collider)
		return;

	auto* enemy = hit.collider->GetGameObject()->GetComponent<Enemy>();
	if (enemy && !enemy->IsDead())
	{
		enemy->TakeDamage(35.0f);
		AudioManager::getInstance().playSound("robohurt");
		Feedback.droneHit = true;
		Feedback.surfaceHit = false;
		Feedback.shotAge = 0.0f;
		Feedback.tracerStart = origin;
		Feedback.tracerEnd = origin;
	}
}

void Player::Fire()
{
	//no ammo firing sound
	if (Currentbullets <= 0 )
	{
		AudioManager::getInstance().playSound("shotempty");
		return;
	}

	//Normal firing sound
	AudioManager::getInstance().playSound("shot");

	//Gun Firing and ammo data update
	firingTimer = fireRateInSeconds;
	CanFire = false;
	JustFired = true;
	MuzzleTimer = MuzzleTime;
	Currentbullets--;	

	//Ray cast
	auto camT = cameraController->GetGameObject()->GetTransform();
	const auto rayOrigin = camT->position;
	const auto rayDirection = camT->GetForwardVector();
	const auto cameraRotation = glm::quat(camT->rotation);
	camT->rotation.x += glm::radians(3.0f);

	Feedback.tracerStart = rayOrigin + rayDirection * 0.35f +
		cameraRotation * glm::vec3(0.14f, -0.12f, 0.0f);
	Feedback.tracerEnd = rayOrigin + rayDirection * 45.0f;
	Feedback.shotAge = 0.0f;
	Feedback.surfaceHit = false;
	Feedback.droneHit = false;

	recoil = glm::radians(16.0f);

	HitInfo hitInfo;
	if (collisionController->Raycast(rayOrigin, rayDirection, hitInfo, collider))
	{
		Feedback.tracerEnd = hitInfo.intersectionPoint;
		Feedback.surfaceHit = true;
		if (hitInfo.collider != nullptr && hitInfo.collider->GetGameObject() != nullptr)
		{
			auto enemy = hitInfo.collider->GetGameObject()->GetComponent<Enemy>();
			auto t = hitInfo.collider->GetGameObject()->GetTransform();
			if (enemy != nullptr && !enemy->IsDead())
			{
				Feedback.droneHit = true;
				
				AudioManager::getInstance().playSoundAt("robohurt", 0, t->position.x, t->position.y, camT->position.x, camT->position.y);
				enemy->TakeDamage(20.0f);
			}
				
		}
	}
}

void Player::HandleMovement(float deltaTime)
{
	const auto* keys = Input::GetKeys();
	auto* transform = GetGameObject()->GetTransform();
	glm::vec3 direction(0.0f);
	if (keys[GLFW_KEY_W])
		direction += transform->GetForwardVector();
	if (keys[GLFW_KEY_S])
		direction -= transform->GetForwardVector();
	if (keys[GLFW_KEY_A])
		direction -= transform->GetRightVector();
	if (keys[GLFW_KEY_D])
		direction += transform->GetRightVector();

	const bool walking = keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT];
	const float speed = moveSpeed * (walking ? 0.45f : 1.35f);
	if (glm::dot(direction, direction) > 0.0f)
		direction = glm::normalize(direction) * speed;

	velocity.x = direction.x;
	velocity.z = direction.z;
	if (keys[GLFW_KEY_SPACE] && isGrounded)
		velocity.y += 5.0f;
	velocity += gravity * deltaTime;
}

void Player::Reload()
{
	if (Totalbullets > 0)
	{
		if (Currentbullets == 6)
			return;
		CanFire = false;
		ReloadTimer = ReloadTime;		
		if (Totalbullets >= Magsize)
		{
			Totalbullets = Totalbullets - (Magsize - Currentbullets);
			Currentbullets = Magsize;			
		}
		else
		{
			int bulletsneeded = Magsize - Currentbullets;
			if (Totalbullets >= bulletsneeded)
			{
				Currentbullets = Magsize;
				Totalbullets -= bulletsneeded;
			}
			else
			{
				Currentbullets += Totalbullets;
				Totalbullets = 0;
			}
		}		
		AudioManager::getInstance().playSound("reload");
	}	
}

void Player::DamagePlayer(int damage, glm::vec3 source)
{
	if (!ControlsEnabled || IsplayerDead || damage <= 0)
		return;

	Feedback.damageAge = 0.0f;
	Feedback.damageSource = source;
	AudioManager::getInstance().playSound("playerhurt");
	PlayerHealth = std::max(0, PlayerHealth - damage);
	IsplayerDead = PlayerHealth == 0;
}

void Player::HealthBoost(int health)
{
	AudioManager::getInstance().playSound("healthboost");
	PlayerHealth += health;
	if (PlayerHealth > 100)
		PlayerHealth = 100;
}

void Player::ExtraAmmo(int ammo)
{
	AudioManager::getInstance().playSound("ammopickup");
	Totalbullets += ammo;
}
