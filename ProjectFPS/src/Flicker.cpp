#include<pch.h>
#include<Components/Transform.h>
#include<Components/Player.h>
#include"Flicker.h"

Light* Flicker::light1;
float Flicker::FlickerTimer;

void Flicker::LoadFlickerLights()
{
	auto Flickerlight = new GameObject();
	Flickerlight->GetTransform()->position = { 6.69f, -1.0f, -27.85f };
	Flickerlight->GetTransform()->scale = { 0.5f, 0.5f, 0.5f };
	Flickerlight->GetTransform()->rotation = { glm::radians(-90.0f), 0, 0 };
	light1 = Flickerlight->AddComponent(new Light());
	light1->color = { 0.3f, 0.3f, 1.0f };
	light1->type = LightType::Point;
	light1->intensity = 1.0f;
}

void Flicker::FlickLights(float deltatime)
{
	if (FlickerTimer > 0)
	{
		FlickerTimer -= deltatime;
	}
	else
	{
		light1->intensity = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		FlickerTimer = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.5f));
	}
}