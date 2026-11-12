#include "pch.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

#include<ft2build.h>
#include FT_FREETYPE_H

#include <SDL.h>

#include "OpenGLWindow.h"
#include "Debug.h"
#include "Input.h"
#include "Mesh.h"
#include "Components/Transform.h"
#include "Components/Player.h"
#include "Components/Light.h"
#include "Components/CameraController.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/HUD.h"
#include "Shade.h"
#include "Text.h"
#include "AudioManager.h"
#include "WaveSpawner.h"
#include "Flicker.h"



#define NOMINMAX
#include <Windows.h>
#include <process.h>

#undef main

static OpenGLWindow* mainWindow;

//to determine where the game is in splashscreen or actual game
enum class GameState {
	SplashScreen,
	Game,
	Paused,
	Results
};

GameState currentState = GameState::SplashScreen;

//loads the shader that are used in the game
void LoadShaders()
{
	// Basic shader
	std::string baseShaderPath = "resources/Shaders/shader";
	auto baseShader = new Shader("Base");
	baseShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Missing shader
	baseShaderPath = "resources/Shaders/Missing";
	auto missingShader = new Shader("__ERROR_SHADER");
	missingShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Shadow map (depth) shader
	baseShaderPath = "resources/Shaders/Shadow";
	auto shadowShader = new Shader("__ShadowMap");
	shadowShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// UI shader
	baseShaderPath = "resources/Shaders/UI";
	auto uiShader = new Shader("UI");
	uiShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Skybox shader
	baseShaderPath = "resources/Shaders/Skybox";
	auto skyboxShader = new Shader("Skybox");
	skyboxShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Standard shader
	baseShaderPath = "resources/Shaders/Standard";
	auto standardShader = new Shader("Standard");
	standardShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	const std::string instanceShaderPath = "resources/Shaders/InstanceStandard";
	for (int variant = 1; variant <= 8; ++variant)
	{
		const std::string suffix = variant == 1 ? "" : std::to_string(variant);
		auto shader = new Shader("InstanceStandard" + suffix);
		shader->Load(instanceShaderPath + suffix + ".vert", instanceShaderPath + ".frag");
	}
}

void LoadAudios()
{
	std::string basepath = "resources/Audios/";
	AudioManager& audiomanager = AudioManager::getInstance();
	audiomanager.loadMusic("MM BG", basepath + "Mainmenu.wav");
	audiomanager.loadMusic("Game BG", basepath + "Ambience.wav");
	audiomanager.loadSound("dronecharge", basepath + "DroneCharge.wav");
	audiomanager.loadSound("heavycharge", basepath + "HeavyCharge.wav");
	audiomanager.loadSound("step", basepath + "Step.wav");
	audiomanager.loadSound("walkstep", basepath + "WalkStep.wav");
	audiomanager.loadSound("ready", basepath + "Ready.wav");
	audiomanager.loadSound("shot", basepath + "Shot.wav");
	audiomanager.loadSound("shotempty", basepath + "Shot_Empty.mp3");
	audiomanager.loadSound("reload", basepath + "Reload.wav");
	audiomanager.loadSound("flashlight", basepath + "Flashlight.mp3");
	audiomanager.loadSound("robohurt", basepath + "Robot/Robo_Hurt.wav");
	audiomanager.loadSound("robodead", basepath + "Robot/Robo_Dead.mp3");
	audiomanager.loadSound("playerhurt", basepath + "Player_Hurt.mp3");
	audiomanager.loadSound("healthboost", basepath + "healthBoost.mp3");
	audiomanager.loadSound("ammopickup", basepath + "Ammo.mp3");
}

void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	if (width > 0 && height > 0)
	{
		glViewport(0, 0, width, height);
		Renderer::SetWindowResolutionInfo(width, height);
	}
}

//initialize the requried datas
void Initialize()
{
	std::srand(std::time(nullptr));

	Renderer::SetWindowResolutionInfo(mainWindow->GetWidth(), mainWindow->GetHeight());

	glfwSetInputMode(mainWindow->GetWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(mainWindow->GetWindowPointer(), FramebufferResizeCallback);

	//initialize shaders
	LoadShaders();

	AudioManager& audioManager = AudioManager::getInstance();

	//auido initialization
	if (!audioManager.init())
		Debug::LogError("Audio Manager Failed To Initialize");
	else
		LoadAudios();

	//Text Initialization
	Text::LoadFont("resources/Fonts/impact.ttf", 72);
	Text::SetupTextRendering();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderCombatFeedback(Shade& shader, GLuint vao, bool showDamage)
{
	const auto& feedback = Player::Feedback;
	if (feedback.shotAge >= 0.22f && feedback.damageAge >= 0.65f)
		return;

	const auto* camera = CameraController::GetCurrent();
	if (!camera)
		return;
	const auto transform = camera->camera.GetProjectionMatrix() * camera->camera.GetViewMatrix();
	const glm::vec4 start = transform * glm::vec4(feedback.tracerStart, 1.0f);
	const glm::vec4 end = transform * glm::vec4(feedback.tracerEnd, 1.0f);
	const bool visible = start.w > 0.001f && end.w > 0.001f;

	shader.use();
	if (visible)
	{
		shader.setVec3("startPoint", glm::vec3(start) / start.w);
		shader.setVec3("endPoint", glm::vec3(end) / end.w);
	}
	shader.setVec3("effect", glm::vec3(visible ? std::max(0.0f, 1.0f - feedback.shotAge / 0.16f) : 0.0f,
		visible && feedback.surfaceHit ? std::max(0.0f, 1.0f - feedback.shotAge / 0.22f) : 0.0f,
		showDamage ? std::max(0.0f, 1.0f - feedback.damageAge / 0.65f) : 0.0f));
	const glm::vec3 sourceView = glm::vec3(camera->camera.GetViewMatrix() * glm::vec4(feedback.damageSource, 1.0f));
	const float horizontalDistance = glm::length(glm::vec2(sourceView.x, sourceView.z));
	shader.setVec3("damageSide", glm::vec3(horizontalDistance > 0.01f ? sourceView.x / horizontalDistance : 0.0f, 0, 0));
	shader.setVec3("impactColor", feedback.droneHit ? glm::vec3(1.0f, 0.84f, 0.38f) : glm::vec3(1.0f, 0.58f, 0.25f));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

struct MenuRect
{
	float x, y, width, height;

	bool Contains(double px, double py) const
	{
		return px >= x && px < x + width && py >= y && py < y + height;
	}

	void Draw(int frameWidth, int frameHeight, glm::vec3 color) const
	{
		const float scaleX = frameWidth / 1600.0f;
		const float scaleY = frameHeight / 900.0f;
		glScissor(static_cast<int>(x * scaleX), static_cast<int>(y * scaleY),
			static_cast<int>(width * scaleX), static_cast<int>(height * scaleY));
		glClearColor(color.r, color.g, color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

bool RestartMission()
{
	wchar_t* executable = nullptr;
	if (_get_wpgmptr(&executable) != 0 || !executable)
		return false;

	std::wstring command = L"\"" + std::wstring(executable) + L"\" --retry";
	STARTUPINFOW startup{};
	startup.cb = sizeof(startup);
	PROCESS_INFORMATION child{};
	if (!CreateProcessW(executable, command.data(), nullptr, nullptr, FALSE, 0,
		nullptr, nullptr, &startup, &child))
		return false;

	CloseHandle(child.hThread);
	CloseHandle(child.hProcess);
	glfwSetWindowShouldClose(mainWindow->GetWindowPointer(), true);
	return true;
}

int main(int argc, char** argv)
{
	OpenGLWindow window(1600, 900);
	mainWindow = &window;
	if (!window.Initialize())
		return EXIT_FAILURE;

	Initialize();

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	Game game;
	game.Initialize();

	// Initialize shader
	Shade myShader("resources/Shaders/UI.vert", "resources/Shaders/UI.frag");

	Shade combatShader("resources/Shaders/CombatFeedback.vert", "resources/Shaders/CombatFeedback.frag");
	GLuint combatVao = 0;
	glGenVertexArrays(1, &combatVao);

	double lastTime = glfwGetTime();
	float elapsed = 0.0f;
	bool missionComplete = false;
	int resupplyWave = 0;
	int relaysRestored = 0;
	float relayProgress = 0.0f;
	float linkRemaining = 0.0f;
	bool linkArmed = false;
	const glm::vec3 objectives[] = {{1.2f, 0, -29.8f}, {-17.0f, 0, -37.3f}, {0, 0, 0}};
	auto* beaconObject = new GameObject();
	auto* beacon = beaconObject->AddComponent(new Light());
	beacon->type = LightType::Point;
	beacon->color = {0.15f, 0.9f, 1.0f};
	beacon->intensity = 0.0f;
	bool retry = argc > 1 && std::string(argv[1]) == "--retry";
	bool retryFailed = false;
	bool enterWasDown = false;
	bool showDamage = true;
	int volume = 70;
	int sensitivity = 100;
	int selection = 0;
	int dragging = -1;
	bool mouseWasDown = false;
	double previousMouseX = 0, previousMouseY = 0;
	auto* handle = window.GetWindowPointer();
	glfwSetScrollCallback(handle, [](GLFWwindow*, double, double y) { Player::CycleEquipment(y); });
	const MenuRect rows[] = {{150, 512, 430, 76}, {644, 513, 806, 100}, {644, 393, 806, 100},
		{644, 273, 806, 100}, {150, 410, 430, 76}, {150, 308, 430, 76}};
	bool previousKeys[GLFW_KEY_LAST + 1]{};
	auto pressed = [&](int key) {
		const bool down = Input::GetKeys()[key];
		const bool edge = down && !previousKeys[key];
		previousKeys[key] = down;
		return edge;
	};
	auto& waves = WaveSpawner::getInstance();
	const glm::vec3 white(0.94f);
	const glm::vec3 cyan(0.55f, 0.9f, 0.95f);

	//play BG Music
	AudioManager::getInstance().playMusic("MM BG");
	Mix_VolumeMusic(50);

	while (!mainWindow->ShouldClose())
	{
		const bool enterDown = Input::GetKeys()[GLFW_KEY_ENTER];
		const bool enterPressed = enterDown && !enterWasDown;
		enterWasDown = enterDown;
		const bool mouseDown = glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		const bool clicked = mouseDown && !mouseWasDown;
		mouseWasDown = mouseDown;
		const bool escape = pressed(GLFW_KEY_ESCAPE);
		const bool justPaused = escape && currentState == GameState::Game;
		const bool up = pressed(GLFW_KEY_UP);
		const bool down = pressed(GLFW_KEY_DOWN);
		const bool left = pressed(GLFW_KEY_LEFT);
		const bool right = pressed(GLFW_KEY_RIGHT);
		const bool interact = pressed(GLFW_KEY_F);
		const bool requestHealth = pressed(GLFW_KEY_H);
		const bool requestAmmo = pressed(GLFW_KEY_J);
		if (escape && currentState == GameState::Game)
		{
			selection = 0;
			currentState = GameState::Paused;
			Player::ControlsEnabled = false;
			CameraController::GetCurrent()->enableRotating = false;
			glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			dragging = -1;
			glfwGetCursorPos(handle, &previousMouseX, &previousMouseY);
			Mix_Pause(-1);
			Mix_PauseMusic();
		}
		else if (escape && currentState != GameState::Paused)
			glfwSetWindowShouldClose(mainWindow->GetWindowPointer(), true);

		if (currentState == GameState::SplashScreen)
		{
			glClearColor(0.035f, 0.055f, 0.08f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			Text::RenderTexPivotCenter(myShader, "LAB LOCKDOWN", 800, 610, 1.25f, white);
			Text::RenderTexPivotCenter(myShader, "Survive the drones, restore power, then extract.", 800, 513, 0.49f, white);
			Text::RenderTexPivotCenter(myShader, "ENTER  START MISSION", 800, 402, 0.58f, cyan);
			Text::RenderTexPivotCenter(myShader, "WASD  Run     Shift  Walk     Mouse  Look     Space  Jump", 800, 235, 0.38f, white);
			Text::RenderTexPivotCenter(myShader, "Left click  Fire     R  Reload     E  Flashlight     Esc  Pause", 800, 182, 0.38f, white);

			if (enterPressed || retry)
			{
				AudioManager::getInstance().playMusic("Game BG");
				Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
				Mix_Volume(-1, MIX_MAX_VOLUME * volume / 100);
				retry = false;
				Renderer::Init();
				lastTime = glfwGetTime();
				Player::ControlsEnabled = true;
				currentState = GameState::Game;
			}
		}
		else if (currentState == GameState::Game)
		{
			const double now = glfwGetTime();
			const float frameTime = static_cast<float>(now - lastTime);
			const float simulationTime = std::min(frameTime, 0.05f);
			lastTime = now;
			elapsed += frameTime;

			const int healthBeforeUpdate = Player::PlayerHealth;
			game.Update(simulationTime);
			const int wave = waves.CurrentWave;
			const bool restoreRelay = wave == 2 && relaysRestored == 0;
			const bool linkRelay = wave == 3 && relaysRestored == 1;
			const bool extract = wave == 3 && waves.EnemyRemaining == 0 && relaysRestored == 2;
			const int targetIndex = restoreRelay || (linkRelay && !linkArmed) ? 0 : linkRelay ? 1 : 2;
			const glm::vec3 target = objectives[targetIndex];
			const bool hasObjective = restoreRelay || linkRelay || extract;
			const glm::vec3 offset = target - Player::playerTransform->position;
			const float distance = glm::length(glm::vec2(offset.x, offset.z));
			const bool nearby = distance < 2.0f;
			beacon->intensity = hasObjective ? 6.0f : 0.0f;
			beaconObject->GetTransform()->position = target + glm::vec3(0, 0.9f, 0);

			if (linkArmed)
			{
				linkRemaining = std::max(0.0f, linkRemaining - simulationTime);
				if (linkRemaining == 0.0f)
				{
					linkArmed = false;
					relayProgress = 0.0f;
				}
			}

			if (!Player::IsplayerDead && hasObjective)
			{
				if (linkRelay && !linkArmed && targetIndex == 0 && nearby && interact)
				{
					linkArmed = true;
					linkRemaining = 35.0f;
					AudioManager::getInstance().playSound("ready");
				}
				else if (extract && nearby && interact)
				{
					Player::Score += 300;
					missionComplete = true;
				}
				else if (restoreRelay || (linkRelay && linkArmed && targetIndex == 1))
				{
					const bool holding = nearby && Input::GetKeys()[GLFW_KEY_F] &&
						Player::PlayerHealth >= healthBeforeUpdate;
					relayProgress = holding ? relayProgress + simulationTime : 0.0f;
					if (relayProgress >= (restoreRelay ? 3.0f : 2.0f))
					{
						++relaysRestored;
						linkArmed = false;
						relayProgress = 0.0f;
						beacon->intensity = 0.0f;
						Player::Score += 150;
						AudioManager::getInstance().playSound("ready");
					}
				}
			}

			const bool resupply = wave > 0 && wave < WaveSpawner::WaveCount &&
				waves.EnemyRemaining == 0 && resupplyWave != wave && !restoreRelay;
			if (!Player::IsplayerDead && resupply)
			{
				if (requestHealth && Player::PlayerHealth < 100)
				{
					Player::HealthBoost(35);
					resupplyWave = wave;
				}
				else if (requestAmmo)
				{
					Player::ExtraAmmo(24);
					resupplyWave = wave;
				}
			}

			if (Player::IsplayerDead || missionComplete)
			{
				Player::ControlsEnabled = false;
				Mix_FadeOutMusic(500);
				currentState = GameState::Results;
			}
			else
			{
				if (!restoreRelay)
					waves.WaveUpdate(simulationTime);
				Flicker::FlickLights(simulationTime);
			}

			Renderer::Render();
			Renderer::Flush();
			glDisable(GL_DEPTH_TEST);
			RenderCombatFeedback(combatShader, combatVao, showDamage);
			HUD::RenderHUD(myShader, Player::PlayerHealth, Player::Totalbullets,
				Player::Currentbullets, Player::Score, waves.CurrentWave, waves.EnemyRemaining, elapsed);

			if (hasObjective && !missionComplete && !Player::IsplayerDead)
			{
				const glm::vec3 view = glm::vec3(CameraController::GetCurrent()->camera.GetViewMatrix() *
					glm::vec4(target, 1.0f));
				const char* direction = view.z > 0.0f ? "BEHIND" :
					view.x < -0.45f * -view.z ? "LEFT" : view.x > 0.45f * -view.z ? "RIGHT" : "AHEAD";
				const char* objective = restoreRelay ? "RESTORE CENTRAL RELAY" : extract ? "RETURN TO EXTRACTION" :
					targetIndex == 0 ? "ARM THE CENTRAL RELAY" : "SYNC THE NORTHWEST RELAY";
				Text::RenderTexPivotCenter(myShader, objective, 800, 704, 0.52f, cyan);
				std::string guidance = std::to_string(static_cast<int>(std::ceil(distance))) + " m  " + direction;
				if (nearby)
				{
					if (extract || (linkRelay && targetIndex == 0))
						guidance = extract ? "F  EXTRACT" : "F  START 35 SECOND LINK";
					else
					{
						const int progress = static_cast<int>(100.0f * relayProgress / (restoreRelay ? 3.0f : 2.0f));
						guidance = "HOLD F  " + std::to_string(progress) + "%   Damage interrupts";
					}
				}
				Text::RenderTexPivotCenter(myShader, guidance, 800, 650, 0.43f, white);
				if (linkRelay && targetIndex == 1)
					Text::RenderTexPivotCenter(myShader, "LINK WINDOW  " +
						std::to_string(static_cast<int>(std::ceil(linkRemaining))) + " s", 800, 603, 0.42f, cyan);
			}
			else if (resupply && resupplyWave != waves.CurrentWave)
			{
				const char* choice = Player::PlayerHealth < 100 ?
					"CHOOSE RESUPPLY    H  HEALTH +35    J  AMMO +24" : "HEALTH FULL    J  AMMO +24";
				Text::RenderTexPivotCenter(myShader, choice, 800, 675, 0.43f, cyan);
			}
			else if (waves.CurrentWave == WaveSpawner::WaveCount && waves.EnemyRemaining > 0)
				Text::RenderTexPivotCenter(myShader, "HEAVY DRONE: FIRE DURING BLUE RECOVERY",
					800, 675, 0.36f, cyan);
		}
		else if (currentState == GameState::Paused)
		{
			int windowWidth, windowHeight, frameWidth, frameHeight;
			glfwGetWindowSize(handle, &windowWidth, &windowHeight);
			glfwGetFramebufferSize(handle, &frameWidth, &frameHeight);
			double cursorX, cursorY;
			glfwGetCursorPos(handle, &cursorX, &cursorY);
			const bool mouseMoved = cursorX != previousMouseX || cursorY != previousMouseY;
			previousMouseX = cursorX;
			previousMouseY = cursorY;
			const double x = cursorX * 1600.0 / std::max(1, windowWidth);
			const double y = 900.0 - cursorY * 900.0 / std::max(1, windowHeight);
			int hovered = -1;
			for (int i = 0; i < 6; ++i)
				if (rows[i].Contains(x, y))
					hovered = i;

			const int order[] = {0, 4, 5, 1, 2, 3};
			if (up || down)
			{
				const int index = static_cast<int>(std::find(order, order + 6, selection) - order);
				selection = order[(index + (down ? 1 : 5)) % 6];
			}
			else if (hovered >= 0 && (mouseMoved || clicked))
				selection = hovered;

			const bool activate = !justPaused && (enterPressed || (clicked && hovered >= 0));
			if (!mouseDown)
				dragging = -1;
			if (clicked && !justPaused && (hovered == 1 || hovered == 2))
				dragging = hovered;

			constexpr float trackX = 675.0f, trackWidth = 740.0f;
			const int adjustment = static_cast<int>(right) - static_cast<int>(left);
			const float fraction = static_cast<float>(std::clamp((x - trackX) / trackWidth, 0.0, 1.0));
			if ((selection == 1 && adjustment) || dragging == 1)
			{
				sensitivity = dragging == 1 ? 20 + static_cast<int>(std::round(fraction * 180)) :
					std::clamp(sensitivity + adjustment * 10, 20, 200);
				CameraController::GetCurrent()->rotateSpeed = 0.005f * sensitivity / 100.0f;
			}
			if ((selection == 2 && adjustment) || dragging == 2)
			{
				volume = dragging == 2 ? static_cast<int>(std::round(fraction * 100)) :
					std::clamp(volume + adjustment * 10, 0, 100);
				Mix_VolumeMusic(MIX_MAX_VOLUME * volume / 100);
				Mix_Volume(-1, MIX_MAX_VOLUME * volume / 100);
			}
			if (selection == 3 && (adjustment || activate))
				showDamage = !showDamage;

			const glm::vec3 muted(0.63f, 0.72f, 0.8f);
			glDisable(GL_DEPTH_TEST);
			glViewport(0, 0, frameWidth, frameHeight);
			glClearColor(0.025f, 0.042f, 0.065f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_SCISSOR_TEST);
			MenuRect{150, 706, 1300, 3}.Draw(frameWidth, frameHeight, cyan);
			for (int i = 0; i < 6; ++i)
			{
				rows[i].Draw(frameWidth, frameHeight, i == selection ?
					glm::vec3(0.10f, 0.23f, 0.30f) : glm::vec3(0.055f, 0.09f, 0.14f));
				if (i == selection)
					MenuRect{rows[i].x, rows[i].y, 4, rows[i].height}.Draw(frameWidth, frameHeight, cyan);
			}
			for (int i = 1; i <= 2; ++i)
			{
				const float fill = i == 1 ? (sensitivity - 20) / 180.0f : volume / 100.0f;
				const float trackY = rows[i].y + 22;
				MenuRect{trackX, trackY, trackWidth, 6}.Draw(frameWidth, frameHeight, glm::vec3(0.22f, 0.30f, 0.37f));
				MenuRect{trackX, trackY, trackWidth * fill, 6}.Draw(frameWidth, frameHeight, cyan);
				MenuRect{trackX + trackWidth * fill - 5, trackY - 4, 10, 14}.Draw(frameWidth, frameHeight, white);
			}
			MenuRect{1320, 299, 96, 45}.Draw(frameWidth, frameHeight,
				showDamage ? cyan : glm::vec3(0.23f, 0.31f, 0.39f));
			glDisable(GL_SCISSOR_TEST);

			Text::RenderText(myShader, "LAB LOCKDOWN", 150, 818, 0.35f, cyan);
			Text::RenderText(myShader, "Mission paused", 150, 747, 0.93f, white);
			Text::RenderText(myShader, "MISSION", 150, 644, 0.34f, muted);
			Text::RenderText(myShader, "SETTINGS", 644, 644, 0.34f, muted);
			Text::RenderText(myShader, "Resume", 180, 539, 0.5f, white);
			Text::RenderText(myShader, "Restart mission", 180, 437, 0.5f, white);
			Text::RenderText(myShader, "Quit game", 180, 335, 0.5f, white);
			Text::RenderText(myShader, "Mouse sensitivity", 675, 572, 0.45f, white);
			Text::RenderText(myShader, "Volume", 675, 452, 0.45f, white);
			Text::RenderTexPivotCenter(myShader, std::to_string(sensitivity) + "%", 1364, 582, 0.4f, cyan);
			Text::RenderTexPivotCenter(myShader, std::to_string(volume) + "%", 1364, 462, 0.4f, cyan);
			Text::RenderText(myShader, "Red damage effect", 675, 311, 0.45f, white);
			Text::RenderTexPivotCenter(myShader, showDamage ? "ON" : "OFF", 1368, 321, 0.32f,
				showDamage ? glm::vec3(0.025f, 0.042f, 0.065f) : white);
			Text::RenderText(myShader, "Mouse or arrows to navigate     Enter to select     Esc to resume", 150, 163, 0.35f, muted);
			if ((activate && selection == 0) || (escape && !justPaused))
			{
				lastTime = glfwGetTime();
				currentState = GameState::Game;
				glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				Player::ControlsEnabled = true;
				CameraController::GetCurrent()->enableRotating = true;
				Mix_Resume(-1);
				Mix_ResumeMusic();
			}
			if (activate && selection == 5)
				glfwSetWindowShouldClose(handle, true);
			if (activate && selection == 4)
				retryFailed = !RestartMission();
			if (retryFailed)
				Text::RenderText(myShader, "Could not restart.", 150, 105, 0.32f, white);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.035f, 0.055f, 0.08f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			Text::RenderTexPivotCenter(myShader, missionComplete ? "EXTRACTION COMPLETE" : "MISSION FAILED",
				800, 590, 1.0f, missionComplete ? cyan : glm::vec3(1.0f, 0.42f, 0.35f));
			Text::RenderTexPivotCenter(myShader, "Score  " + std::to_string(Player::Score) +
				"     Time  " + HUD::FormatTime(elapsed), 800, 460, 0.55f, white);
			Text::RenderTexPivotCenter(myShader, "ENTER  RETRY     ESC  QUIT", 800, 315, 0.45f, white);
			if (retryFailed)
				Text::RenderTexPivotCenter(myShader, "Could not restart. Press Enter to try again.", 800, 260, 0.32f, white);
			if (enterPressed)
				retryFailed = !RestartMission();
		}

		glfwPollEvents();

		mainWindow->SwapBuffers();
	}
	glDeleteVertexArrays(1, &combatVao);
	glDeleteProgram(combatShader.ID);
	AudioManager::getInstance().cleanup();
	return 0;
}
