#pragma once

#include "Component.h"
#include "Mesh.h"

class MeshRenderer : public Component
{
public:
	glm::vec3 animationOffset{0.0f};
	void Update(float deltaTime) override;

	void Draw();

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Material>> materials;

	bool castShadows = true;
	bool recieveShadows = true;
};

