#pragma once
#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include "RenderSystem/Shader.hpp"
#include "RenderSystem/RenderManager.h"

class Mesh
{
public:
	

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices,
		std::vector<Texture> _textures);
	~Mesh();

	void Draw(Shader* shader, RenderManager* renderManager);
	uint32_t VAO, VBO, EBO;
};

