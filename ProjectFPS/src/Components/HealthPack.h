#pragma once

#include "Component.h"

class Light;

class HealthPack : public Component
{
public:
	Transform* PlayerTranform;
	bool Used = false;

	void Update(float deltaTime) override;

private:
	void Start() override;
	Light* beacon = nullptr;

};
