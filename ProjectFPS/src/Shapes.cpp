//Basic data for basic shapes
#include "pch.h"
#include "Shapes.h"

//cube with 8 vertices for the skybox
std::shared_ptr<Mesh> Shapes::CreateSkybox(const glm::vec3& size, bool reverseOrder)
{
	glm::vec3 halfSize = size * 0.5f;

	std::vector<glm::vec3> vertices {
		{ -halfSize.x, -halfSize.y, -halfSize.z}, { halfSize.x, -halfSize.y, -halfSize.z },
		{ halfSize.x, halfSize.y, -halfSize.z }, { -halfSize.x, halfSize.y, -halfSize.z	},
		{ -halfSize.x, -halfSize.y, halfSize.z }, { halfSize.x, -halfSize.y, halfSize.z },
		{ halfSize.x, halfSize.y, halfSize.z }, { -halfSize.x, halfSize.y, halfSize.z }
	};

	std::vector<unsigned int> indices {
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	if (reverseOrder)
		std::reverse(indices.begin(), indices.end());

	return std::make_shared<Mesh>(vertices, indices);
}

//cube with 8 vertices for the colliders
std::shared_ptr<Mesh> Shapes::CreateSimple(const glm::vec3& size, bool reverseOrder)
{
	glm::vec3 halfSize = size * 0.5f;

	std::vector<glm::vec3> vertices{
		{ -halfSize.x, -halfSize.y, -halfSize.z}, { halfSize.x, -halfSize.y, -halfSize.z },
		{ halfSize.x, halfSize.y, -halfSize.z }, { -halfSize.x, halfSize.y, -halfSize.z	},
		{ -halfSize.x, -halfSize.y, halfSize.z }, { halfSize.x, -halfSize.y, halfSize.z },
		{ halfSize.x, halfSize.y, halfSize.z }, { -halfSize.x, halfSize.y, halfSize.z }
	};

	std::vector<unsigned int> indices{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	if (reverseOrder)
		std::reverse(indices.begin(), indices.end());

	return std::make_shared<Mesh>(vertices, indices);
}

//cube with 24 vertices better handling of normal maps and textures
std::shared_ptr<Mesh> Shapes::CreateCube()
{

	std::vector<glm::vec3> vertices{
		// Front face
	{ 0.5,  0.5,  0.5},  
	{-0.5,  0.5,  0.5},  
	{-0.5, -0.5,  0.5},  
	{ 0.5, -0.5,  0.5},

	// Back face
	{ 0.5,  0.5, -0.5},
	{-0.5,  0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{ 0.5, -0.5, -0.5},

	// Left face
	{-0.5,  0.5,  0.5},
	{-0.5,  0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{-0.5, -0.5,  0.5},

	// Right face
	{ 0.5,  0.5,  0.5},
	{ 0.5,  0.5, -0.5},
	{ 0.5, -0.5, -0.5},
	{ 0.5, -0.5,  0.5},

	// Top face
	{ 0.5,  0.5,  0.5},
	{-0.5,  0.5,  0.5},
	{-0.5,  0.5, -0.5},
	{ 0.5,  0.5, -0.5},

	// Bottom face
	{ 0.5, -0.5,  0.5},
	{-0.5, -0.5,  0.5},
	{-0.5, -0.5, -0.5},
	{ 0.5, -0.5, -0.5},
	};

	std::vector<uint32_t> indices{
		// Front face
		0, 1, 2, 0, 2, 3,
		// Back face
		4, 5, 6, 4, 6, 7,
		// Left face
		8, 9, 10, 8, 10, 11,
		// Right face
		12, 13, 14, 12, 14, 15,
		// Top face
		16, 17, 18, 16, 18, 19,
		// Bottom face
		20, 21, 22, 20, 22, 23
	};

	std::vector<glm::vec3> normals = {
		// Front face
		{ 0,  0,  1}, { 0,  0,  1}, { 0,  0,  1}, { 0,  0,  1},
		// Back face
		{ 0,  0, -1}, { 0,  0, -1}, { 0,  0, -1}, { 0,  0, -1},
		// Left face
		{-1,  0,  0}, {-1,  0,  0}, {-1,  0,  0}, {-1,  0,  0},
		// Right face
		{ 1,  0,  0}, { 1,  0,  0}, { 1,  0,  0}, { 1,  0,  0},
		// Top face
		{ 0,  1,  0}, { 0,  1,  0}, { 0,  1,  0}, { 0,  1,  0},
		// Bottom face
		{ 0, -1,  0}, { 0, -1,  0}, { 0, -1,  0}, { 0, -1,  0}
	};

	std::vector<glm::vec3> uv0 = {
		// Front face
		{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
		// Back face
		{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
		// Left face
		{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
		// Right face
		{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
		// Top face
		{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
		// Bottom face
		{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0}
	};

	auto mesh = std::make_shared<Mesh>(vertices, indices);
	mesh->SetNormals(normals);
	mesh->SetUV0(uv0);
	mesh->SendData();

	return mesh;
}

std::shared_ptr<Mesh> Shapes::CreatePyramid()
{
	std::vector<glm::vec3> vertices{
		// Base 
		{-0.3f,  0.0f, -0.3f},  
		{ 0.3f,  0.0f, -0.3f},  
		{ 0.3f,  0.0f,  0.3f},  
		{-0.3f,  0.0f,  0.3f},  

		 // Side 1 
		{-0.3f,  0.0f, -0.3f},  
		{0.3f,  0.0f, -0.3f},  
		{0.0f,  0.3f,  0.0f},  

		// Side 2
		{0.3f,  0.0f, -0.3f},  
		{0.3f,  0.0f,  0.3f},  
		{0.0f,  0.3f,  0.0f},  

		// Side 3
		{0.3f,  0.0f,  0.3f},  
		{-0.3f,  0.0f,  0.3f},  
		{0.0f,  0.3f,  0.0f},  

		// Side 4 
		{-0.3f,  0.0f,  0.3f},  
		{-0.3f,  0.0f, -0.3f},  
		{0.0f,  0.3f,  0.0f}   
	};

	std::vector<glm::vec3> normals = {
		// Base normals
		{ 0.0f, -1.0f,  0.0f},  
		{ 0.0f, -1.0f,  0.0f},  
		{ 0.0f, -1.0f,  0.0f},  
		{ 0.0f, -1.0f,  0.0f},  

		// Side 1 normals
		{ 0.0f,  0.707f, -0.707f},  
		{ 0.0f,  0.707f, -0.707f},  
		{ 0.0f,  0.707f, -0.707f},  

		// Side 2 normals
		{ 0.707f,  0.707f,  0.0f},  
		{ 0.707f,  0.707f,  0.0f},  
		{ 0.707f,  0.707f,  0.0f},  

		// Side 3 normals
		{ 0.0f,  0.707f,  0.707f},  
		{ 0.0f,  0.707f,  0.707f},  
		{ 0.0f,  0.707f,  0.707f},  

		// Side 4 normals
		{-0.707f,  0.707f,  0.0f},  
		{-0.707f,  0.707f,  0.0f},  
		{-0.707f,  0.707f,  0.0f}   
	};

	std::vector<glm::vec3> uv0 = {
		// Base UVs
	   { 0.0f, 0.0f, 0.0f},  
	   { 1.0f, 0.0f, 0.0f},  
	   { 1.0f, 1.0f, 0.0f},  
	   { 0.0f, 1.0f, 0.0f},  

	   // Side 1 UVs
	   { 0.0f, 0.0f, 0.0f},  
	   { 1.0f, 0.0f, 0.0f},  
	   { 0.5f, 1.0f, 0.0f},  

	   // Side 2 UVs
	   { 0.0f, 0.0f, 0.0f},  
	   { 1.0f, 0.0f, 0.0f},  
	   { 0.5f, 1.0f, 0.0f},  

	   // Side 3 UVs
	   { 0.0f, 0.0f, 0.0f}, 
	   { 1.0f, 0.0f, 0.0f},  
	   { 0.5f, 1.0f, 0.0f},  

	   // Side 4 UVs
	   { 0.0f, 0.0f, 0.0f},  
	   { 1.0f, 0.0f, 0.0f},  
	   { 0.5f, 1.0f, 0.0f}   
	};

	std::vector<uint32_t> indices{

		// Base
		 0, 1, 2,
		 2, 3, 0,

		 // Sides 
		 4, 5, 6,  
		 7, 8, 9,  
		 10, 11, 12,
		 13, 14, 15  
	};

	auto mesh = std::make_shared<Mesh>(vertices, indices);
	mesh->SetNormals(normals);
	mesh->SetUV0(uv0);
	mesh->SendData();

	return mesh;
}

std::shared_ptr<Mesh> Shapes::CreatePrism()
{
	std::vector<glm::vec3> vertices = {
		// Bottom triangle 
		{-0.3f,  0.0f, -0.3f},  
		{ 0.3f,  0.0f, -0.3f},  
		{ 0.0f,  0.0f,  0.3f}, 

		// Top triangle 
		{-0.3f,  0.3f, -0.3f},  
		{ 0.3f,  0.3f, -0.3f},  
		{ 0.0f,  0.3f,  0.3f},  

		// Side 1
		{-0.3f,  0.0f, -0.3f},  
		{-0.3f,  0.3f, -0.3f},  
		{ 0.0f,  0.3f,  0.3f},  
		{ 0.0f,  0.0f,  0.3f},  

		// Side 2 
		{ 0.3f,  0.0f, -0.3f}, 
		{ 0.3f,  0.3f, -0.3f},  
		{-0.3f,  0.3f, -0.3f},  
		{-0.3f,  0.0f, -0.3f},  

		// Side 3 
		{ 0.0f,  0.0f,  0.3f},  
		{ 0.0f,  0.3f,  0.3f},  
		{ 0.3f,  0.3f, -0.3f},  
		{ 0.3f,  0.0f, -0.3f}   
	};

	std::vector<glm::vec3> normals = {
		// Bottom triangle
		{ 0.0f, -1.0f,  0.0f},  
		{ 0.0f, -1.0f,  0.0f},  
		{ 0.0f, -1.0f,  0.0f},  

		// Top triangle (flat face)
		{ 0.0f,  1.0f,  0.0f},  
		{ 0.0f,  1.0f,  0.0f},  
		{ 0.0f,  1.0f,  0.0f},  

		// Side 1 (rectangular face)
		{ -0.707f,  0.0f,  0.707f}, 
		{ -0.707f,  0.0f,  0.707f},  
		{ -0.707f,  0.0f,  0.707f},  
		{ -0.707f,  0.0f,  0.707f},  

		// Side 2 (rectangular face)
		{ 0.0f,  0.0f, -1.0f},  
		{ 0.0f,  0.0f, -1.0f},  
		{ 0.0f,  0.0f, -1.0f},  
		{ 0.0f,  0.0f, -1.0f},  

		// Side 3 (rectangular face)
		{ 0.707f,  0.0f,  0.707f},  
		{ 0.707f,  0.0f,  0.707f},  
		{ 0.707f,  0.0f,  0.707f},  
		{ 0.707f,  0.0f,  0.707f}  
	};

	// UVs for the triangular prism
	std::vector<glm::vec3> uvs = {
		// Bottom triangle
		{ 0.0f, 0.0f, 0.0f},  
		{ 1.0f, 0.0f, 0.0f},  
		{ 0.5f, 1.0f, 0.0f}, 

		// Top triangle
		{ 0.0f, 0.0f, 0.0f},  
		{ 1.0f, 0.0f, 0.0f},  
		{ 0.5f, 1.0f, 0.0f},  

		// Side 1
		{ 0.0f, 0.0f, 0.0f}, 
		{ 0.0f, 1.0f, 0.0f},  
		{ 2.0f, 1.0f, 0.0f},  
		{ 2.0f, 0.0f, 0.0f},  

		// Side 2
		{ 0.0f, 0.0f, 0.0f},  
		{ 0.0f, 1.0f, 0.0f},  
		{ 2.0f, 1.0f, 0.0f}, 
		{ 2.0f, 0.0f, 0.0f}, 

		// Side 3
		{ 0.0f, 0.0f, 0.0f}, 
		{ 0.0f, 1.0f, 0.0f}, 
		{ 2.0f, 1.0f, 0.0f},  
		{ 2.0f, 0.0f, 0.0f} 
	};

	
	std::vector<unsigned int> indices = {
		// Bottom triangle
		0, 1, 2,

		// Top triangle 
		3, 4, 5,

		// Side 1 
		6, 7, 8,
		8, 9, 6,

		// Side 2 
		10, 11, 12,
		12, 13, 10,

		// Side 3 
		14, 15, 16,
		16, 17, 14
	};

	auto mesh = std::make_shared<Mesh>(vertices, indices);
	mesh->SetNormals(normals);
	mesh->SetUV0(uvs);
	mesh->SendData();

	return mesh;
}

std::shared_ptr<Mesh> Shapes::CreateTriPyramid()
{
	std::vector<glm::vec3> vertices = {
		// Base triangle 
		{ -0.3f, 0.0f, -0.3f },  
		{  0.3f, 0.0f, -0.3f },  
		{  0.0f, 0.0f,  0.3f },  

		// Triangular side 1
		{ -0.3f, 0.0f, -0.3f },  
		{  0.3f, 0.0f, -0.3f }, 
		{  0.0f, 0.3f,  0.0f },  

		// Triangular side 2
		{  0.3f, 0.0f, -0.3f },  
		{  0.0f, 0.0f,  0.3f },  
		{  0.0f, 0.3f,  0.0f },  

		// Triangular side 3
		{  0.0f, 0.0f,  0.3f }, 
		{ -0.3f, 0.0f, -0.3f },  
		{  0.0f, 0.3f,  0.0f }   
	};

	std::vector<glm::vec3> normals = {
		// Base normals (all the same)
		{  0.0f, -1.0f,  0.0f },  
		{  0.0f, -1.0f,  0.0f },  
		{  0.0f, -1.0f,  0.0f }, 

		// Side 1 normals
		{ -0.707f,  0.707f,  0.0f },  
		{ -0.707f,  0.707f,  0.0f },  
		{ -0.707f,  0.707f,  0.0f },  

		// Side 2 normals
		{  0.707f,  0.707f,  0.0f },  
		{  0.707f,  0.707f,  0.0f },  
		{  0.707f,  0.707f,  0.0f },  

		// Side 3 normals
		{  0.0f,  0.707f,  0.707f },  
		{  0.0f,  0.707f,  0.707f }, 
		{  0.0f,  0.707f,  0.707f }  
	};

	std::vector<glm::vec3> uvs = {
		// Base triangle UVs
		{ 0.0f, 0.0f, 0.0f },  
		{ 1.0f, 0.0f, 0.0f },  
		{ 0.5f, 1.0f, 0.0f },  

		// Side 1 UVs
		{ 0.0f, 0.0f, 0.0f }, 
		{ 1.0f, 0.0f, 0.0f },  
		{ 0.5f, 1.0f, 0.0f },  

		// Side 2 UVs
		{ 0.0f, 0.0f, 0.0f },  
		{ 1.0f, 0.0f, 0.0f },  
		{ 0.5f, 1.0f, 0.0f },  

		// Side 3 UVs
		{ 0.0f, 0.0f, 0.0f },  
		{ 1.0f, 0.0f, 0.0f },  
		{ 0.5f, 1.0f, 0.0f }   
	};

	std::vector<unsigned int> indices = {
		// Base  
		0, 1, 2,
		// Side 1 
		3, 4, 5,
		// Side 2 
		6, 7, 8,
		// Side 3
		9, 10, 11
	};

	auto mesh = std::make_shared<Mesh>(vertices, indices);
	mesh->SetNormals(normals);
	mesh->SetUV0(uvs);
	mesh->SendData();

	return mesh;
}