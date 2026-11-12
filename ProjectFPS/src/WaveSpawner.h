#pragma once
#include <random>

#include "Mesh.h"

class MeshRenderer;
class Enemy;

class WaveSpawner
{
public:

	// Get the single instance of Wave Spawner
	static WaveSpawner& getInstance();

	// Delete copy constructor and assignment operator (to enforce Singleton)
	WaveSpawner(const WaveSpawner&) = delete;
	WaveSpawner& operator=(const WaveSpawner&) = delete;

	std::vector<std::vector<glm::vec3>> SetOfPatrolPoints;

	static constexpr int WaveCount = 3;
	static constexpr float BreakDuration = 10.0f;
	float BreakRemaining() const { return breakRemaining; }
	int CurrentWave = 0;
	int EnemyRemaining = 0;

	void InitWave();
	void WaveUpdate(float deltatime);
	void SpawnWave(int WaveNumber);
	int getRandomNumber(int min, int max);


private:
	WaveSpawner();
	~WaveSpawner();

	float breakRemaining = BreakDuration;
	static Enemy* CreateEnemy(glm::vec3 pos, const std::vector<glm::vec3>& patrolPoints);
	static MeshRenderer* LoadAndCreate(const std::string& path);

	void Wave1();
	void Wave2();
	void Wave3();
};
