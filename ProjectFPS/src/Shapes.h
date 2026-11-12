#pragma once
#include <Mesh.h>

// Cube
class Shapes
{
public:
	static std::shared_ptr<Mesh> CreateSkybox(const glm::vec3& size, bool reverseOrder = true);
	static std::shared_ptr<Mesh> CreateSimple(const glm::vec3& size, bool reverseOrder = true);
	static std::shared_ptr<Mesh> CreateCube();
	static std::shared_ptr<Mesh> CreatePyramid();
	static std::shared_ptr<Mesh> CreatePrism();
	static std::shared_ptr<Mesh> CreateTriPyramid();
};