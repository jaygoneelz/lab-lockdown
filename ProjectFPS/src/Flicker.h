#pragma once
#include<Components/Light.h>

class Flicker
{
public:
	static Light* light1;
	static float FlickerTimer;

	static void LoadFlickerLights();
	static void FlickLights(float deltatime);
};
