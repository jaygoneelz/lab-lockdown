#include "pch.h"
#include "glm/gtx/string_cast.hpp"
#include "LevelDescription.h"

#include "MeshImporter.h"
#include "Components/MeshRenderer.h"
#include "Components/Light.h"
#include "Components/Transform.h"
#include "Components/CollisionController.h"
#include "Components/Player.h"
#include "Components/Enemy.h"

#include "Rendering/Renderer.h"
#include "Rendering/TextureCube.h"
#include "Shapes.h"
#include "Flicker.h"
#include "Components/HealthPack.h"
#include "Components/AmmoPack.h"

//Contains the data that load the elements in the level
void LevelDescription::Load()
{
	auto ccgo = new GameObject();
	auto collisionController = ccgo->AddComponent(new CollisionController());
	ccgo->AddComponent(new BoxCollider());

	CreateLights();
	Flicker::LoadFlickerLights();
	CreatePlayer();

	CreatPickables();

	CreateCratesPrimitive();
	CreatePyramidGreebles();
	CreatePrismGreebles();
	CreateTriPyramidGreebles();

	CreateCratesImport();
	CreateGenerator();
	CreateBarrel();

	LoadLevel();
	CreateCollision(*collisionController);		
}

//creating a cube with primitve shapes
void LevelDescription::CreateCratesPrimitive()
{
	std::string basePath = "resources/Textures/Crate/";
	auto Diff = std::make_shared<Texture>(basePath + "diff.png");
	auto Norm = std::make_shared<Texture>(basePath + "nor.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);

	std::vector<glm::vec3> instancePositions = {
	glm::vec3(3.76f, -0.5f, -26.6f),
	glm::vec3(2.56f, -0.5f, -26.85f),
	glm::vec3(-13.4f, -0.5f, -30.18f),
	glm::vec3(21.8f, -0.5f, -10.8f),
	glm::vec3(-21.5f, -0.5f, -21.74f),
	glm::vec3(-5.0f, -0.5f, 1.38f),
	glm::vec3(-6.14f, -0.5f, 0.93f)
	};

	std::vector<float> instanceRotations = {
	glm::radians(-7.0f),   
	glm::radians(14.0f),  
	glm::radians(9.0f),  
	glm::radians(9.0f), 
	glm::radians(-37.0f),
	glm::radians(21.0f),   
	glm::radians(21.6f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	auto Cube = Shapes::CreateCube();

	auto gameObject = new GameObject();
	auto Renderer = gameObject->AddComponent(new MeshRenderer());
	Renderer->meshes.push_back(Cube);
	Renderer->meshes[0]->Setinstance(7);
	Renderer->castShadows = false;
	Renderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };

	Renderer->materials.push_back(Mat);
}

//creating and texturing pyramid
void LevelDescription::CreatePyramidGreebles()
{
	std::string basePath = "resources/Textures/Greebles/";
	auto Diff = std::make_shared<Texture>(basePath + "basecolor.jpg");
	auto Norm = std::make_shared<Texture>(basePath + "normal.jpg");
	auto Emm = std::make_shared<Texture>(basePath + "emission.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard5"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);
	Mat->SetTexture("emissionMap", Emm);
	Mat->SetFloat("emissionStrength", 2.0f);

	std::vector<glm::vec3> instancePositions = {
	glm::vec3(6.7f, -1.0f, -6.0f),
	glm::vec3(-5.32f, -1.0f, -18.0f),
	glm::vec3(-21.17f, -1.0f, -29.21f)
	};

	std::vector<float> instanceRotations = {
	glm::radians(-7.0f),
	glm::radians(14.0f),
	glm::radians(9.0f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	auto Pyramid = Shapes::CreatePyramid();

	auto gameObject = new GameObject();
	auto Renderer = gameObject->AddComponent(new MeshRenderer());
	Renderer->meshes.push_back(Pyramid);
	Renderer->meshes[0]->Setinstance(3);
	Renderer->castShadows = false;
	Renderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };

	Renderer->materials.push_back(Mat);
}

//creating and texturing a prism
void LevelDescription::CreatePrismGreebles()
{
	std::string basePath = "resources/Textures/Greebles/";
	auto Diff = std::make_shared<Texture>(basePath + "basecolor.jpg");
	auto Norm = std::make_shared<Texture>(basePath + "normal.jpg");
	auto Emm = std::make_shared<Texture>(basePath + "emission.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard6"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);
	Mat->SetTexture("emissionMap", Emm);
	Mat->SetFloat("emissionStrength", 2.0f);

	std::vector<glm::vec3> instancePositions = {
	glm::vec3(7.19f, -1.0f, -6.48f),
	glm::vec3(3.25f, -1.0f, -25.22f),
	glm::vec3(-21.86f, -1.0f, -28.0f)
	};

	std::vector<float> instanceRotations = {
	glm::radians(-7.0f),
	glm::radians(14.0f),
	glm::radians(9.0f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	auto Prism = Shapes::CreatePrism();

	auto gameObject = new GameObject();
	auto Renderer = gameObject->AddComponent(new MeshRenderer());
	Renderer->meshes.push_back(Prism);
	Renderer->meshes[0]->Setinstance(3);
	Renderer->castShadows = false;
	Renderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };

	Renderer->materials.push_back(Mat);
}


//creating and texturing a Hexaprism
void LevelDescription::CreateTriPyramidGreebles()
{
	std::string basePath = "resources/Textures/Greebles/";
	auto Diff = std::make_shared<Texture>(basePath + "basecolor.jpg");
	auto Norm = std::make_shared<Texture>(basePath + "normal.jpg");
	auto Emm = std::make_shared<Texture>(basePath + "emission.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard7"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);
	Mat->SetTexture("emissionMap", Emm);
	Mat->SetFloat("emissionStrength", 2.0f);

	std::vector<glm::vec3> instancePositions = {
	glm::vec3(6.2f, -1.0f, -6.72f),
	glm::vec3(-9.82f, -1.0f, -8.17f),
	glm::vec3(-20.95f, -1.0f, -40.76f)
	};

	std::vector<float> instanceRotations = {
	glm::radians(-7.0f),
	glm::radians(14.0f),
	glm::radians(9.0f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	auto HexaPrism = Shapes::CreateTriPyramid();

	auto gameObject = new GameObject();
	auto Renderer = gameObject->AddComponent(new MeshRenderer());
	Renderer->meshes.push_back(HexaPrism);
	Renderer->meshes[0]->Setinstance(3);
	Renderer->castShadows = false;
	Renderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };

	Renderer->materials.push_back(Mat);
}

//loading Crate mesh and instancing
void LevelDescription::CreateCratesImport()
{
	std::string basePath = "resources/Models/Crate/";
	auto crateRenderer = LoadAndCreate(basePath + "Crate.fbx");
	crateRenderer->castShadows = false;

	crateRenderer->meshes[0]->Setinstance(3);
	//crateRenderer->GetGameObject()->GetTransform()->fixedRotation = { glm::radians(-90.0f), 0.0f, 0.0f };
	crateRenderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };

	auto Diff = std::make_shared<Texture>(basePath + "diff.png");
	auto Norm = std::make_shared<Texture>(basePath + "nor.png");
	auto Emm = std::make_shared<Texture>(basePath + "emi.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard2"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);
	Mat->SetTexture("emissionMap", Emm);
	Mat->SetFloat("emissionStrength", 2.0f);

	std::vector<glm::vec3> instancePositions = {
	glm::vec3(-7.19f, -0.7f, -29.8f),
	glm::vec3(-14.0f, -0.7f, -45.5f),
	glm::vec3(-21.0f, -0.7f, -8.36f)
	};

	std::vector<float> instanceRotations = {
	glm::radians(90.0f),
	glm::radians(90.0f),
	glm::radians(30.0f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	crateRenderer->materials.push_back(Mat);
}

//loading generator mesh and instancing
void LevelDescription::CreateGenerator()
{
	std::string basePath = "resources/Models/generator/";
	auto GeneratorRenderer = LoadAndCreate(basePath + "Generator.fbx");
	GeneratorRenderer->castShadows = false;

	GeneratorRenderer->meshes[0]->Setinstance(4);

	auto Diff = std::make_shared<Texture>(basePath + "Basecolor.png");
	auto Norm = std::make_shared<Texture>(basePath + "Normal.png");
	auto Emm = std::make_shared<Texture>(basePath + "emm.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard3"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);
	Mat->SetTexture("emissionMap", Emm);
	Mat->SetFloat("emissionStrength", 2.0f);

	std::vector<glm::vec3> instancePositions = {
		glm::vec3(6.15f, -1.0f, 1.36f),
		glm::vec3(22.0f, -1.0f, -7.33f),
		glm::vec3(6.69f, -1.0f, -27.85f),
		glm::vec3(-19.74f, -1.0f, -41.76f)
	};

	std::vector<float> instanceRotations = {
		glm::radians(90.0f),
		glm::radians(90.0f),
		glm::radians(30.0f),
		glm::radians(30.0f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	GeneratorRenderer->materials.push_back(Mat);
}

//loading Barrell mesh and instancing
void LevelDescription::CreateBarrel()
{
	std::string basePath = "resources/Models/Barrel/";
	auto Renderer = LoadAndCreate(basePath + "Barrel.fbx");
	Renderer->castShadows = false;

	Renderer->meshes[0]->Setinstance(2);
	//GeneratorRenderer->GetGameObject()->GetTransform()->fixedRotation = { glm::radians(90.0f), 0.0f, 0.0f };

	auto Diff = std::make_shared<Texture>(basePath + "diffuse.png");
	auto Norm = std::make_shared<Texture>(basePath + "normal.png");

	auto Mat = std::make_shared<Material>(Shader::Find("InstanceStandard4"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);

	std::vector<glm::vec3> instancePositions = {
		glm::vec3(-9.87f, -1.0f, -6.84f),
		glm::vec3(4.0f, -1.0f, -16.33f)
	};

	std::vector<float> instanceRotations = {
		glm::radians(90.0f),
		glm::radians(0.0f)
	};

	Mat->shader->SetVec3Array("instancePositions", instancePositions);
	Mat->shader->SetFloatArray("instanceRotations", instanceRotations);

	Renderer->materials.push_back(Mat);
}

void LevelDescription::CreatPickables()
{
	//health pack
	std::string basePath = "resources/Models/Health Pack/";
	auto Renderer = LoadAndCreate(basePath + "Health Pack.fbx");
	Renderer->castShadows = false;


	Renderer->GetGameObject()->GetTransform()->rotation = { glm::radians(-90.0f), glm::radians(-90.0f), 0.0f };
	Renderer->GetGameObject()->GetTransform()->scale = { 0.8f, 0.8f, 0.8f };
	Renderer->GetGameObject()->GetTransform()->position = { 22.2f, -0.5f, -9.22f };
	Renderer->GetGameObject()->AddComponent(new HealthPack());

	auto Diff = std::make_shared<Texture>(basePath + "basecolor.png");
	auto Norm = std::make_shared<Texture>(basePath + "normal.png");

	auto Mat = std::make_shared<Material>(Shader::Find("Standard"));
	Mat->SetTexture("diffuseMap", Diff);
	Mat->SetTexture("normalMap", Norm);

	Renderer->materials.push_back(Mat);


	auto Renderer1 = LoadAndCreate(basePath + "Health Pack.fbx");
	Renderer1->castShadows = false;


	Renderer1->GetGameObject()->GetTransform()->rotation = { glm::radians(-90.0f), glm::radians(-90.0f), 0.0f };
	Renderer1->GetGameObject()->GetTransform()->scale = { 0.8f, 0.8f, 0.8f };
	Renderer1->GetGameObject()->GetTransform()->position = { -21.9f, -0.5f, -16.52f };
	Renderer1->GetGameObject()->AddComponent(new HealthPack());

	Renderer1->materials.push_back(Mat);

	//Extra ammo
	auto Renderer2 = LoadAndCreate(basePath + "Health Pack.fbx");
	Renderer2->castShadows = false;


	Renderer2->GetGameObject()->GetTransform()->rotation = { glm::radians(-90.0f), 0.0f, 0.0f };
	Renderer2->GetGameObject()->GetTransform()->scale = { 1.05f, 0.55f, 0.72f };
	Renderer2->GetGameObject()->GetTransform()->position = { 1.2f, -0.5f, -29.8f };
	Renderer2->GetGameObject()->AddComponent(new AmmoPack());

	auto Diff1 = std::make_shared<Texture>(basePath + "basecolor ammo.png");

	auto Mat2 = std::make_shared<Material>(Shader::Find("Standard"));
	Mat2->SetTexture("diffuseMap", Diff1);
	Mat2->SetTexture("normalMap", Norm);

	Renderer2->materials.push_back(Mat2);

	auto Renderer3 = LoadAndCreate(basePath + "Health Pack.fbx");
	Renderer3->castShadows = false;


	Renderer3->GetGameObject()->GetTransform()->rotation = { glm::radians(-90.0f), 0.0f, 0.0f };
	Renderer3->GetGameObject()->GetTransform()->scale = { 1.05f, 0.55f, 0.72f };
	Renderer3->GetGameObject()->GetTransform()->position = { -17.0f, -0.5f, -37.3f };
	Renderer3->GetGameObject()->AddComponent(new AmmoPack());

	Renderer3->materials.push_back(Mat2);
}

//loads the level terrain
void LevelDescription::LoadLevel()
{
	std::string basePath = "resources/Models/Level/";
	auto lvlRenderer = LoadAndCreate(basePath + "Level2.fbx");
	lvlRenderer->GetGameObject()->GetTransform()->rotation = { glm::radians(-90.0f), 0.0f, 0.0f};
	lvlRenderer->GetGameObject()->GetTransform()->scale = { 1.0f, 1.0f, 1.0f };
	lvlRenderer->GetGameObject()->GetTransform()->position = { 0.0f, -1.0f, 0.0f };
	//lvlRenderer->castShadows = false;

	//lvlRenderer->recieveShadows = false;

	// Floor
	auto floorDiff = std::make_shared<Texture>(basePath + "floor/basecolor.jpg");
	auto floorNorm = std::make_shared<Texture>(basePath + "floor/normal.jpg");

	auto floorMat = std::make_shared<Material>(Shader::Find("Standard"));
	floorMat->SetTexture("diffuseMap", floorDiff);
	floorMat->SetTexture("normalMap", floorNorm);

	// Wall
	auto wallDiff = std::make_shared<Texture>(basePath + "wall/basecolor.png");
	auto wallNorm = std::make_shared<Texture>(basePath + "wall/normal.png");

	auto wallMat = std::make_shared<Material>(Shader::Find("Standard"));
	wallMat->SetTexture("diffuseMap", wallDiff);
	wallMat->SetTexture("normalMap", wallNorm);
	//wallMat->SetTexture("maskMap", wallMask);	

	lvlRenderer->materials.push_back(floorMat);
	lvlRenderer->materials.push_back(floorMat);
	lvlRenderer->materials.push_back(wallMat);
}

//load the directional light in the scene
void LevelDescription::CreateLights()
{
	auto skybox = std::make_shared<TextureCube>();
	skybox->Load("resources/Textures/Skyboxes/sunflowers_polyhaven/.hdr");
	Renderer::SetSkybox(skybox);


	//sun light
	auto sunGO = new GameObject();
	auto sunLight = sunGO->AddComponent(new Light());
	sunLight->intensity = 1.0f;
	sunLight->color = { 1.0f, 0.97f, 0.9f };
	sunGO->GetTransform()->position = { 0, 5, 0 };
	sunGO->GetTransform()->rotation = { glm::radians(-45.0f), glm::radians(-10.0f), 0 };

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->position = { 6.15f, -1.0f, 1.36f };
		lightGO->GetTransform()->scale = { 0.5f, 0.5f, 0.5f };
		lightGO->GetTransform()->rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 0.3f, 0.3f, 1.0f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->position = { 22.0f, -1.0f, -7.33f };
		lightGO->GetTransform()->scale = { 0.5f, 0.5f, 0.5f };
		lightGO->GetTransform()->rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 0.3f, 0.3f, 1.0f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}

	//commented because used for flicker light
	/*
	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->position = { 6.69f, -1.0f, -27.85f };
		lightGO->GetTransform()->scale = { 0.5f, 0.5f, 0.5f };
		lightGO->GetTransform()->rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 0.3f, 0.3f, 1.0f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}*/
	
	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->position = { -19.74f, -1.0f, -41.76f };
		lightGO->GetTransform()->scale = { 0.5f, 0.5f, 0.5f };
		lightGO->GetTransform()->rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 0.3f, 0.3f, 1.0f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}
}

//creates the player 
void LevelDescription::CreatePlayer()
{
	// Loading the weapon
	auto wDiff = std::make_shared<Texture>("resources/Models/gun/BaseColor.png");
	auto wNorm = std::make_shared<Texture>("resources/Models/gun/Normal.png");
	auto wMask = std::make_shared<Texture>("resources/Models/gun/Roughness.png");

	auto wMat = std::make_shared<Material>(Shader::Find("Standard"));
	wMat->SetTexture("diffuseMap", wDiff);
	wMat->SetTexture("normalMap", wNorm);
	wMat->SetTexture("maskMap", wMask);

	auto weaponRenderer = LoadAndCreate("resources/Models/gun/Gun.fbx");
	weaponRenderer->materials.push_back(wMat);
	weaponRenderer->GetGameObject()->GetTransform()->position = { 0.0f, 1.0f, 0.0f };
	weaponRenderer->GetGameObject()->GetTransform()->rotation = { glm::radians(90.0f), 0, 0};
	weaponRenderer->GetGameObject()->GetTransform()->scale = { 0.01f, 0.01f, 0.01f };

	// Player
	auto playerGO = new GameObject();
	auto player = playerGO->AddComponent(new Player());
	playerGO->GetTransform()->position += glm::vec3(0, 0, 0);

	player->weapon = weaponRenderer->GetGameObject();
	//auto weapon = weaponRenderer->GetGameObject();
}

Enemy* LevelDescription::CreateEnemy(glm::vec3 pos, const std::vector<glm::vec3>& patrolPoints)
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

//adds box colliders for objects loaded in to scene
void LevelDescription::CreateCollision(CollisionController& collisionController)
{
	//level border
	collisionController.colliders.push_back(new BoxCollider({ 0.1f,2.35,4.5f }, { 16,5,1}, {0,0,0}));         //1
	collisionController.colliders.push_back(new BoxCollider({ -8.64f,2.35,0.7f }, { 1,5,10.5}, {0,0,0}));     //2
	collisionController.colliders.push_back(new BoxCollider({ -16.4f,2.35,-5.5f }, { 14.39,5,1}, {0,0,0}));   //3
	collisionController.colliders.push_back(new BoxCollider({ -24.38f,2.35,-26.57 }, { 1,5,43}, {0,0,0}));    //4
	collisionController.colliders.push_back(new BoxCollider({ -17.35,2.35,-47.53 }, { 13,5,1}, {0,0,0}));     //5
	collisionController.colliders.push_back(new BoxCollider({ -10.82f,2.35,-39.26 }, { 1,5,15.5}, {0,0,0}));   //6
	collisionController.colliders.push_back(new BoxCollider({ -0.8,2.35,-32.03}, { 19,5,1}, {0,0,0}));        //7
	collisionController.colliders.push_back(new BoxCollider({ 8.85f,2.35,-22.2 }, { 1,5,19}, {0,0,0}));       //8
	collisionController.colliders.push_back(new BoxCollider({ 16.74f,2.35,-13.24f }, { 14.5,5,1}, {0,0,0}));  //9
	collisionController.colliders.push_back(new BoxCollider({ 24.4f,2.35,-9.19f }, { 1,5,8}, {0,0,0}));       //10
	collisionController.colliders.push_back(new BoxCollider({ 16.26f,2.35,-5.33f }, { 15,5,1}, {0,0,0}));     //11
	collisionController.colliders.push_back(new BoxCollider({ 8.68f,2.35,0.11f }, { 1,5,10}, {0,0,0}));       //12

	//Boxes
	glm::vec3 boxscale = { 1,1,1 };

	collisionController.colliders.push_back(new BoxCollider({ 3.76f, -0.5f, -26.6f }, boxscale, { 0,glm::radians(-7.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ 2.56f, -0.5f, -26.85f }, boxscale, { 0,glm::radians(14.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -13.4f, -0.5f, -30.18f }, boxscale, { 0,glm::radians(9.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ 21.8f, -0.5f, -10.8f }, boxscale, { 0,glm::radians(9.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -21.5f, -0.5f, -21.74f }, boxscale, { 0,glm::radians(-37.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -5.0f, -0.5f, 1.38f }, boxscale, { 0,glm::radians(21.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -6.14f, -0.5f, 0.93f }, boxscale, { 0,glm::radians(21.0f),0 }));


	//pyramid
	glm::vec3 Greeblescale = { 0.5,0.5,0.5 };

	collisionController.colliders.push_back(new BoxCollider({ 6.7f, -1.0f, -6.0f }, Greeblescale, { 0,glm::radians(-7.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -5.32f, -1.0f, -18.0f }, Greeblescale, { 0,glm::radians(14.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -21.17f, -1.0f, -29.21f }, Greeblescale, { 0,glm::radians(9.0f),0 }));

	//prism
	collisionController.colliders.push_back(new BoxCollider({ 7.19f, -1.0f, -6.48f }, Greeblescale, { 0,glm::radians(-7.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ 3.25f, -1.0f, -25.22f }, Greeblescale, { 0,glm::radians(14.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -21.86f, -1.0f, -28.0f }, Greeblescale, { 0,glm::radians(9.0f),0 }));

	//TriPrism
	collisionController.colliders.push_back(new BoxCollider({ 6.2f, -1.0f, -6.72f }, Greeblescale, { 0,glm::radians(-7.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -9.82f, -1.0f, -8.17f }, Greeblescale, { 0,glm::radians(14.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -20.95f, -1.0f, -40.76f }, Greeblescale, { 0,glm::radians(9.0f),0 }));

	//Ammo Crate
	glm::vec3 cratescale = { 0.8,1,1.5 };

	collisionController.colliders.push_back(new BoxCollider({ -7.19f, -0.7f, -29.8f }, cratescale, { 0,glm::radians(90.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -14.0f, -0.7f, -45.5f }, cratescale, { 0,glm::radians(90.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -21.0f, -0.7f, -8.36f }, cratescale, { 0,glm::radians(30.0f),0 }));

	//Generator 
	glm::vec3 generatorscale = { 1,3,1 };

	collisionController.colliders.push_back(new BoxCollider({ 6.15f, 1.0f, 1.36f }, generatorscale, { 0,glm::radians(90.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ 22.0f, 1.0f, -7.33f }, generatorscale, { 0,glm::radians(90.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ 6.69f, 1.0f, -27.85f }, generatorscale, { 0,glm::radians(30.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ -19.74f, 1.0f, -41.76f }, generatorscale, { 0,glm::radians(30.0f),0 }));

	//Barell
	glm::vec3 Barrelscale = { 1.5,1,1.5 };

	collisionController.colliders.push_back(new BoxCollider({ -9.3f, -0.5f, -7.2f }, Barrelscale, { 0,glm::radians(90.0f),0 }));
	collisionController.colliders.push_back(new BoxCollider({ 4.5f, -0.5f, -16.0f }, Barrelscale, { 0,glm::radians(0.0f),0 }));
}

//loads the fbx file with the help of mesh importer
MeshRenderer* LevelDescription::LoadAndCreate(const std::string& path)
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
