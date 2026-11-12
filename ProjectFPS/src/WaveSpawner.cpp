#include "pch.h"
#include "WaveSpawner.h"
#include "MeshImporter.h"
#include "Components/MeshRenderer.h"
#include "Components/Transform.h"
#include "Components/Enemy.h"
#include "Components/Colliders.h"

WaveSpawner& WaveSpawner::getInstance() {
	static WaveSpawner instance;
	return instance;
}

WaveSpawner::WaveSpawner() {
	// Constructor body
}

WaveSpawner::~WaveSpawner() {
	// Destructor body
}

void WaveSpawner::InitWave()
{
	CurrentWave = 0;
	EnemyRemaining = 0;	

	//SpawnWave(1);	
}


void WaveSpawner::WaveUpdate(float deltaTime)
{
	if (EnemyRemaining > 0 || CurrentWave >= WaveCount)
		return;

	if (CurrentWave > 0)
	{
		breakRemaining -= deltaTime;
		if (breakRemaining > 0.0f)
			return;
	}

	SpawnWave(++CurrentWave);
	breakRemaining = BreakDuration;
}

void WaveSpawner::SpawnWave(int wavenumber)
{
	switch (wavenumber)
	{
		case 1:
			Wave1();
			break;
		case 2:
			Wave2();
			break;
		case 3:
			Wave3();
			break;
		default:
			break;
	}
}

void WaveSpawner::Wave1()
{
	EnemyRemaining = 5;
	std::vector<glm::vec3> patrolPoints1 = {
		glm::vec3(-18.5f, 0.0f, -43.5f),
		glm::vec3(-13.2f, 0.0f, -43.5f)
	};
	std::vector<glm::vec3> patrolPoints2 = {
		glm::vec3(-21.8f, 0.0f, -39.0f),
		glm::vec3(-21.8f, 0.0f, -32.0f)
	};
	std::vector<glm::vec3> patrolPoints3 = {
		glm::vec3(-7.15f, 0.0f, -28.2f),
		glm::vec3(-1.8f, 0.0f, -28.2f)
	};
	std::vector<glm::vec3> patrolPoints4 = {
		glm::vec3(19.6f, 0.0f, -10.69f),
		glm::vec3(13.7f, 0.0f, -10.69f)
	};
	std::vector<glm::vec3> patrolPoints5 = {
		glm::vec3(5.9f, 0.0f, -23.3f),
		glm::vec3(5.9f, 0.0f, -18.2f)
	};

	CreateEnemy(patrolPoints1[1], patrolPoints1);
	CreateEnemy(patrolPoints2[1], patrolPoints2);
	CreateEnemy(patrolPoints3[1], patrolPoints3);
	CreateEnemy(patrolPoints4[1], patrolPoints4);
	CreateEnemy(patrolPoints5[1], patrolPoints5);
}

void WaveSpawner::Wave2()
{
	EnemyRemaining = 9;

	std::vector<glm::vec3> patrolPoints1 = {
		glm::vec3(-18.5f, 0.0f, -43.5f),
		glm::vec3(-13.2f, 0.0f, -43.5f)
	};
	std::vector<glm::vec3> patrolPoints2 = {
		glm::vec3(-21.8f, 0.0f, -39.0f),
		glm::vec3(-21.8f, 0.0f, -32.0f)
	};
	std::vector<glm::vec3> patrolPoints3 = {
		glm::vec3(-7.15f, 0.0f, -28.2f),
		glm::vec3(-1.8f, 0.0f, -28.2f)
	};
	std::vector<glm::vec3> patrolPoints4 = {
		glm::vec3(19.6f, 0.0f, -10.69f),
		glm::vec3(13.7f, 0.0f, -10.69f)
	};
	std::vector<glm::vec3> patrolPoints5 = {
		glm::vec3(5.9f, 0.0f, -23.3f),
		glm::vec3(5.9f, 0.0f, -18.2f)
	};
	std::vector<glm::vec3> patrolPoints6 = {
		glm::vec3(-21.1f, 0.0f, -19.27f),
		glm::vec3(-16.48f, 0.0f, -19.27f)
	};
	std::vector<glm::vec3> patrolPoints7 = {
		glm::vec3(-17.4f, 0.0f, -7.67f),
		glm::vec3(-17.4f, 0.0f, -14.32f)
	};
	std::vector<glm::vec3> patrolPoints8 = {
		glm::vec3(-1.6f, 0.0f, -17.0f),
		glm::vec3(-1.6f, 0.0f, -22.75f)
	};
	std::vector<glm::vec3> patrolPoints9 = {
		glm::vec3(-20.2f, 0.0f, -25.1f),
		glm::vec3(-14.6f, 0.0f, -25.1f)
	};

	CreateEnemy(patrolPoints1[1], patrolPoints1);
	CreateEnemy(patrolPoints2[1], patrolPoints2);
	CreateEnemy(patrolPoints3[1], patrolPoints3);
	CreateEnemy(patrolPoints4[1], patrolPoints4);
	CreateEnemy(patrolPoints5[1], patrolPoints5);
	CreateEnemy(patrolPoints6[1], patrolPoints6)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints7[1], patrolPoints7)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints8[1], patrolPoints8)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints9[1], patrolPoints9)->moveSpeed = 3.4f;
}

void WaveSpawner::Wave3()
{
	EnemyRemaining = 13;

	std::vector<glm::vec3> patrolPoints1 = {
		glm::vec3(-18.5f, 0.0f, -43.5f),
		glm::vec3(-13.2f, 0.0f, -43.5f)
	};
	std::vector<glm::vec3> patrolPoints2 = {
		glm::vec3(-21.8f, 0.0f, -39.0f),
		glm::vec3(-21.8f, 0.0f, -32.0f)
	};
	std::vector<glm::vec3> patrolPoints3 = {
		glm::vec3(-7.15f, 0.0f, -28.2f),
		glm::vec3(-1.8f, 0.0f, -28.2f)
	};
	std::vector<glm::vec3> patrolPoints4 = {
		glm::vec3(19.6f, 0.0f, -10.69f),
		glm::vec3(13.7f, 0.0f, -10.69f)
	};
	std::vector<glm::vec3> patrolPoints5 = {
		glm::vec3(5.9f, 0.0f, -23.3f),
		glm::vec3(5.9f, 0.0f, -18.2f)
	};
	std::vector<glm::vec3> patrolPoints6 = {
		glm::vec3(-21.1f, 0.0f, -19.27f),
		glm::vec3(-16.48f, 0.0f, -19.27f)
	};
	std::vector<glm::vec3> patrolPoints7 = {
		glm::vec3(-17.4f, 0.0f, -7.67f),
		glm::vec3(-17.4f, 0.0f, -14.32f)
	};
	std::vector<glm::vec3> patrolPoints8 = {
		glm::vec3(-1.6f, 0.0f, -17.0f),
		glm::vec3(-1.6f, 0.0f, -22.75f)
	};
	std::vector<glm::vec3> patrolPoints9 = {
		glm::vec3(-20.2f, 0.0f, -25.1f),
		glm::vec3(-14.6f, 0.0f, -25.1f)
	};
	std::vector<glm::vec3> patrolPoints10 = {
		glm::vec3(-8.8f, 0.0f, -22.9f),
		glm::vec3(-8.8f, 0.0f, -17.5f)
	};
	std::vector<glm::vec3> patrolPoints11 = {
		glm::vec3(-14.28f, 0.0f, -38.1f),
		glm::vec3(-14.28f, 0.0f, -33.74f)
	};
	std::vector<glm::vec3> patrolPoints12 = {
		glm::vec3(-13.52f, 0.0f, -11.0f),
		glm::vec3(-8.48f, 0.0f, -11.0f)
	};
	std::vector<glm::vec3> patrolPoints13 = {
		glm::vec3(-0.7f, 0.0f, -10.8f),
		glm::vec3(5.9f, 0.0f, -10.8f)
	};

	CreateEnemy(patrolPoints1[1], patrolPoints1);
	CreateEnemy(patrolPoints2[1], patrolPoints2);
	CreateEnemy(patrolPoints3[1], patrolPoints3);
	CreateEnemy(patrolPoints4[1], patrolPoints4);
	CreateEnemy(patrolPoints5[1], patrolPoints5);
	CreateEnemy(patrolPoints6[1], patrolPoints6)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints7[1], patrolPoints7)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints8[1], patrolPoints8)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints9[1], patrolPoints9)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints10[1], patrolPoints10)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints11[1], patrolPoints11)->moveSpeed = 3.4f;
	CreateEnemy(patrolPoints12[1], patrolPoints12)->moveSpeed = 3.4f;
	auto heavy = CreateEnemy(patrolPoints13[1], patrolPoints13);
	heavy->health = 180.0f;
	heavy->entranceRemaining = 1.35f;
	heavy->attackDamage = 12;
	heavy->TimeBetweenAttack = 2.5f;
	heavy->GetGameObject()->GetTransform()->scale *= glm::vec3(1.65f, 1.35f, 1.5f);
	heavy->radius *= 1.35f;
	heavy->collider->SetSize(glm::vec3(0.7f, heavy->height - 0.5f, 0.7f) * glm::vec3(1.5f, 1.65f, 1.35f));
}


int WaveSpawner::getRandomNumber(int min, int max) {
	// Create a random number generator
	std::random_device rd;  // Seed generator
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_int_distribution<> distrib(min, max);

	return distrib(gen);
}

Enemy* WaveSpawner::CreateEnemy(glm::vec3 pos, const std::vector<glm::vec3>& patrolPoints)
{
	std::string basePath = "resources/Models/Robot/";
	auto enemyRenderer = LoadAndCreate(basePath + "Robot.fbx");
	enemyRenderer->GetGameObject()->GetTransform()->position = pos;
	enemyRenderer->GetGameObject()->GetTransform()->fixedRotation = { glm::radians(-90.0f),glm::radians(90.0f),0 };
	enemyRenderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };

	// Textures
	static auto diff = std::make_shared<Texture>(basePath + "Basecolor.png");
	static auto norm = std::make_shared<Texture>(basePath + "Normal.png");
	static auto mask = std::make_shared<Texture>(basePath + "Roughness.png");

	auto enemyMaterial = std::make_shared<Material>(Shader::Find("Standard"));
	enemyMaterial->SetTexture("diffuseMap", diff);
	enemyMaterial->SetTexture("normalMap", norm);
	enemyMaterial->SetTexture("maskMap", mask);
	enemyRenderer->materials.push_back(enemyMaterial);

	auto enemy = new Enemy();
	enemy->collisionRays = 8;
	enemy->SetPatrolPoints(patrolPoints);

	return enemyRenderer->GetGameObject()->AddComponent(enemy);
}

//loads the fbx file with the help of mesh importer
MeshRenderer* WaveSpawner::LoadAndCreate(const std::string& path)
{
	try
	{
		auto meshImporter = MeshImporter();
		auto model = meshImporter.Load(path);

		auto gameObject = new GameObject();
		auto meshRenderer = gameObject->AddComponent(new MeshRenderer());
		for (auto mesh : *model)
		{
			meshRenderer->meshes.push_back(mesh);
			//meshRenderer->materials.push_back(new Material(Shader::Find("Standard")));
		}

		return meshRenderer;
	}
	catch (const char* msg) {
		Debug::LogError("LevelDescription - LoadAndCreate(string)", msg);
	}

	return nullptr;
}