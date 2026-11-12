#pragma once
#include<GL/glew.h>
#include <glm/glm.hpp>  // For glm::vec3 and glm::mat4

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shade {
public:
    // Shader Program ID
    GLuint ID;

    // Constructor that reads and builds the shader
    Shade(const char* vertexPath, const char* fragmentPath);

    // Use the shader program
    void use();

    // Utility functions for setting uniforms
    void setVec3(const std::string& name, glm::vec3 value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
};