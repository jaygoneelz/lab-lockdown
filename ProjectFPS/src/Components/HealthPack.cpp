#include "pch.h"
#include "HealthPack.h"
#include "Player.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light.h"

void HealthPack::Start()
{
	auto* lightObject = new GameObject();
	lightObject->GetTransform()->position = GetGameObject()->GetTransform()->position + glm::vec3(0, 0.65f, 0);
	beacon = lightObject->AddComponent(new Light());
	beacon->type = LightType::Point;
	beacon->color = { 0.15f, 1.0f, 0.42f };
	beacon->intensity = 3.0f;
}

void HealthPack::Update(float deltaTime)
{
	if (!Used)
	{
		if (Player::PlayerHealth < 100)
		{
			float distanceToPlayer = glm::length(Player::playerTransform->position - GetGameObject()->GetTransform()->position);

			if (distanceToPlayer < 1.5f)
			{
				Player::HealthBoost(50);
				Used = true;
			beacon->intensity = 0.0f;
				GetGameObject()->GetComponent<MeshRenderer>()->enabled = false;
			}
		}		
	}	
}