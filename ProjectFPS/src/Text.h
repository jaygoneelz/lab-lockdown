#pragma once
#include<pch.h>
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<shade.h>

class Text
{
public:

	static void LoadFont(const std::string& fontPath, int fontSize);
	static void SetupTextRendering();
	static void RenderText(Shade shader, std::string text, float x, float y, float scale, glm::vec3 color);
	static void RenderTextCentered(Shade& shade, std::string text, float screenWidth, float screenHeight, float scale, glm::vec3 color);
	static void RenderTexPivotCenter(Shade& shade, std::string text, float X, float Y, float scale, glm::vec3 color);

};
