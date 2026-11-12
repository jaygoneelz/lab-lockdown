//Handles the text renedring

#include<pch.h>

#include<ft2build.h>
#include FT_FREETYPE_H
#include<Text.h>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Character {
	GLuint TextureID;  
	glm::ivec2 Size;   
	glm::ivec2 Bearing;
	GLuint Advance;    
};

std::map<char, Character> Characters;

GLuint VAO, VBO;

//load the font to use it for rendering
void Text::LoadFont(const std::string& fontPath, int fontSize) {
	
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font at " << fontPath << std::endl;
		exit(EXIT_FAILURE);
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++) { 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load Glyph " << c << std::endl;
			continue;
		}

		
		GLuint texture;
		glGenTextures(1, &texture);
		if (texture == 0) {
			std::cerr << "ERROR::TEXTURE::Failed to generate texture for character: " << c << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED, 
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store character metrics
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// Cleanup FreeType resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

//Setting for rendering the text in screen
void Text::SetupTextRendering() {
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//Actual text render Happens here
void Text::RenderText(Shade shader, std::string text, float x, float y, float scale, glm::vec3 color) {

	glm::mat4 projection = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f, -1.0f, 1.0f);

	shader.use();
	shader.setVec3("textColor", color);
	shader.setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through each character in the string
	for (char c : text) {
		Character ch = Characters[c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// Update VBO for each character quad
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		// Render the text
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//to have the pivot of whole text to the centre instead of top left
void Text::RenderTexPivotCenter(Shade& shade, std::string text, float X, float Y, float scale, glm::vec3 color) {
	// Calculate the width and height of the entire string
	float textWidth = 0.0f;
	float textHeight = 0.0f;

	for (char c : text) {
		Character ch = Characters[c];
		textWidth += (ch.Advance >> 6) * scale;
		textHeight = std::max(textHeight, ch.Size.y * scale); 
	}

	float startX = X - (textWidth / 2);
	float startY = Y - (textHeight / 2);

	RenderText(shade, text, startX, startY, scale, color);
}

//to render the text in the centre of the screen
void Text::RenderTextCentered(Shade& shade, std::string text, float screenWidth, float screenHeight, float scale, glm::vec3 color) {

	float textWidth = 0.0f;
	float textHeight = 0.0f;

	for (char c : text) {
		Character ch = Characters[c];
		textWidth += (ch.Advance >> 6) * scale; 
		textHeight = std::max(textHeight, ch.Size.y * scale); 
	}

	// Calculate starting position to center the text
	float startX = (screenWidth - textWidth) / 2.0f;
	float startY = (screenHeight - textHeight) / 2.0f;

	RenderText(shade, text, startX, startY, scale, color);
}


