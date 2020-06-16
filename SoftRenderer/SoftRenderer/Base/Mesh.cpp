#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices,
	std::vector<Texture> _textures) :vertices(_vertices), indices(_indices), textures(_textures)
{
}


Mesh::~Mesh()
{
}

void Mesh::Draw(Shader* shader, RenderManager* renderManager)
{
	renderManager->DrawMesh(vertices, indices, textures);
}