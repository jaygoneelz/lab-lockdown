//Handles the rendering of each mesh in an object
#include "pch.h"
#include "MeshRenderer.h"

#include "glm/gtx/string_cast.hpp"

#include "Transform.h"
#include "CameraController.h"
#include "Rendering/Renderer.h"

void MeshRenderer::Update(float deltaTime)
{
	if (enabled != false)
	{
		Draw();
	}	
	//Debug::Log("MeshRenderer", glm::to_string(GetGameObject()->GetTransform()->Position).c_str());
}

void MeshRenderer::Draw()
{
	if (meshes.empty())
		return;

	auto currentCamera = CameraController::GetCurrent();

	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i] == nullptr)
			continue;

		std::shared_ptr<Material> material = nullptr;
		if (i < materials.size() && materials[i] != nullptr)
			material = materials[i];

		auto t = GetGameObject()->GetTransform();
		Renderer::DrawMesh(meshes[i], material, glm::translate(glm::mat4(1.0f), animationOffset) * t->GetLocalToWorldMatrix(), currentCamera->camera, castShadows, recieveShadows);
	}
}
