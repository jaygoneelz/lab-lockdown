#include "pch.h"
#include "AmmoPack.h"
#include "Player.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light.h"

void AmmoPack::Start()
{
	auto* lightObject = new GameObject();
	lightObject->GetTransform()->position = GetGameObject()->GetTransform()->position + glm::vec3(0, 0.65f, 0);
	beacon = lightObject->AddComponent(new Light());
	beacon->type = LightType::Point;
	beacon->color = { 1.0f, 0.57f, 0.12f };
	beacon->intensity = 3.0f;
}

void AmmoPack::Update(float deltaTime)
{
	if (!Used)
	{
		float distanceToPlayer = glm::length(Player::playerTransform->position - GetGameObject()->GetTransform()->position);

		if (distanceToPlayer < 1.5f)
		{
			Player::ExtraAmmo(50);
			Used = true;
			beacon->intensity = 0.0f;
			GetGameObject()->GetComponent<MeshRenderer>()->enabled = false;
		}
	}	
}