#include "pch.h"
#include "HUD.h"
#include "Text.h"
#include "WaveSpawner.h"
#include "Components/Player.h"
#include "Components/Enemy.h"
#include "Components/Transform.h"
#include "Scene.h"

static std::string SectorName(const glm::vec3& position)
{
	const std::string depth = position.z < -30.0f ? "NORTH" : position.z < -16.0f ? "CENTRAL" : "SOUTH";
	return depth + (position.x < -7.0f ? " WEST" : position.x > 7.0f ? " EAST" : "");
}

std::string HUD::FormatTime(float elapsed)
{
	const int totalSeconds = static_cast<int>(elapsed);
	const int seconds = totalSeconds % 60;
	return std::to_string(totalSeconds / 60) + ":" +
		(seconds < 10 ? "0" : "") + std::to_string(seconds);
}

void HUD::RenderHUD(Shade shader, int health, int reserveAmmo, int magazineAmmo,
	int score, int wave, int enemiesRemaining, float elapsed)
{
	const glm::vec3 white(0.95f);
	const glm::vec3 amber(1.0f, 0.76f, 0.35f);
	const glm::vec3 red(1.0f, 0.42f, 0.35f);
	const glm::vec3 cyan(0.55f, 0.9f, 0.95f);

	const auto& feedback = Player::Feedback;
	const bool hitMarker = feedback.droneHit && feedback.shotAge < 0.22f;
	const float markerScale = hitMarker ? 0.36f + 0.22f * (1.0f - feedback.shotAge / 0.22f) : 0.35f;
	Text::RenderTexPivotCenter(shader, hitMarker ? "X" : "+", 800, 450, markerScale, hitMarker ? amber : white);
	Text::RenderText(shader, "HEALTH  " + std::to_string(health), 32, 52, 0.5f, health <= 25 ? red : white);
	Text::RenderTexPivotCenter(shader, std::to_string(magazineAmmo) + " / " + std::to_string(reserveAmmo),
		1450, 54, 0.6f, magazineAmmo <= 2 ? amber : white);
	if (Player::Equipped == Player::Equipment::Gun && magazineAmmo <= 2)
		Text::RenderTexPivotCenter(shader, reserveAmmo > 0 ? "R  RELOAD" : "FIND AMMO", 1450, 108, 0.3f, amber);

	Text::RenderText(shader, "WAVE  " + std::to_string(wave) + " / " + std::to_string(WaveSpawner::WaveCount),
		32, 850, 0.5f, cyan);
	Text::RenderText(shader, "Robots remaining: " + std::to_string(enemiesRemaining), 32, 810, 0.35f, white);
	Text::RenderTexPivotCenter(shader, FormatTime(elapsed), 800, 850, 0.5f, white);
	Text::RenderTexPivotCenter(shader, "SCORE  " + std::to_string(score), 1450, 850, 0.45f, white);

	const char* slots[] = {"1  GUN", "2  KNIFE", "3  EMPTY"};
	for (int i = 0; i < 3; ++i)
		Text::RenderTexPivotCenter(shader, slots[i], 660.0f + i * 140.0f, 48, 0.34f,
			static_cast<int>(Player::Equipped) == i ? cyan : white);

	if (Player::playerTransform)
		Text::RenderText(shader, "SECTOR  " + SectorName(Player::playerTransform->position), 32, 773, 0.32f, cyan);

	static float lastEnemySince = 0.0f;
	if (enemiesRemaining != 1)
		lastEnemySince = elapsed;
	else if (elapsed - lastEnemySince >= 6.0f)
	{
		std::vector<Enemy*> enemies;
		SceneManager::GetInstance().GetCurrentScene()->GetComponents(enemies);
		for (auto* enemy : enemies)
		{
			if (enemy->IsDead())
				continue;
			Text::RenderTexPivotCenter(shader, "LAST DRONE  " +
				SectorName(enemy->GetGameObject()->GetTransform()->position), 800, 775, 0.42f, amber);
			break;
		}
	}

	if (enemiesRemaining == 0 && wave < WaveSpawner::WaveCount &&
		(wave != 2 || WaveSpawner::getInstance().BreakRemaining() < WaveSpawner::BreakDuration))
	{
		const int seconds = static_cast<int>(std::ceil(WaveSpawner::getInstance().BreakRemaining()));
		Text::RenderTexPivotCenter(shader, "NEXT WAVE IN " + std::to_string(seconds), 800, 730, 0.55f, cyan);
	}
}
