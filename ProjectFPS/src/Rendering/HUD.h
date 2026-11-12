#pragma once
#include "Shade.h"

class HUD
{
public:
	static void RenderHUD(Shade shader, int health, int reserveAmmo, int magazineAmmo,
		int score, int wave, int enemiesRemaining, float elapsed);
	static std::string FormatTime(float elapsed);
};
