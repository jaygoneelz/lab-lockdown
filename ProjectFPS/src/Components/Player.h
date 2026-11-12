#pragma once

#include "Puppet.h"

class CollisionController;
class CameraController;
class Light;

class Player : public Puppet
{
public:
	GameObject* weapon;
	Light* weaponFlashlight;
	Light* muzzleFlashlight;

	static Transform* playerTransform;

	//no of bullets data
	static int Totalbullets, Currentbullets, Magsize;
	static int PlayerHealth;
	static int Score;	

	static bool IsplayerDead;
	static bool ControlsEnabled;
	enum class Equipment { Gun, Knife, Empty };
	static Equipment Equipped;
	static void CycleEquipment(double scroll);
	struct CombatFeedback
	{
		glm::vec3 tracerStart{};
		glm::vec3 tracerEnd{};
		float shotAge = 1.0f;
		float damageAge = 1.0f;
		glm::vec3 damageSource{};
		bool surfaceHit = false;
		bool droneHit = false;
	};
	static CombatFeedback Feedback;
	static void DamagePlayer(int damage, glm::vec3 source);
	static void HealthBoost(int health);
	static void ExtraAmmo(int ammo);

	void Fire();

private:
	GameObject* knifeBlade = nullptr;
	GameObject* knifeGrip = nullptr;
	float slashRemaining = 0.0f;
	void Slash();
	bool CanFire = false;
	float stepDistance = 0.0f;
	float recoil = 0.0f;
	float stridePhase = 0.0f;
	float strideBlend = 0.0f;
	float landingDip = 0.0f;
	
	//firerate data
	float fireRateInSeconds = 0.3f, firingTimer = 0;
	//Reload Data
	float ReloadTime = 0.8f, ReloadTimer = 0;

	bool JustFired;

	//Muzzle Flash time
	float MuzzleTime = 0.1f, MuzzleTimer = 0;

	void Start() override;
	void Update(float deltaTime) override;

	void HandleMovement(float deltaTime);
	void FirerateHandler(float deltaTime);

	void Reload();

	CameraController* cameraController;
};

