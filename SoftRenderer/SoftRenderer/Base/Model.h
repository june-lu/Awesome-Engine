#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include "Mesh.h"
#include "stb_image.h"
#include "RenderSystem/Shader.h"
#include "RenderSystem/RenderManager.h"


class Model
{
public:
	Model();
	Model(std::string path, RenderManager* renderManager);
	~Model();

	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	RenderManager* renderManager;

	void LoadModel(std::string path);
	void Draw(Shader* shader);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

	Texture TextureFromFile(const char *path, bool gamma = false);
};

