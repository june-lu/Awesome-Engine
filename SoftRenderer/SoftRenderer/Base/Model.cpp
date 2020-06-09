#include "Model.h"

Model::Model()
{

}

Model::Model(std::string path, RenderManager* renderManager)
{
	LoadModel(path);
	this->renderManager = renderManager;
}


Model::~Model()
{
}

void Model::Draw(Shader* shader)
{
	for (uint32_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader, renderManager);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_ConvertToLeftHanded);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		//node->mMeshes[i]表示在scene Mesh中的mesh index
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	//process vertices
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vector3f position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		Vector3f normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		Vector2f texCoords;
		if (mesh->mTextureCoords[0])
		{
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			texCoords = Vector2f(0, 0);
		}

		Vertex vertex(position, normal, texCoords);
		vertices.push_back(vertex);
	}

	//process indices
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//process textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
	std::string typeName)
{
	std::vector<Texture> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (uint32_t j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.colors = TextureFromFile(str.C_Str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

Color* Model::TextureFromFile(const char *path, bool gamma)
{
	std::string fileName = std::string(path);
	fileName = directory + "/" + fileName;
	Color* colors = new Color();
	int width, height, nrComponents;
	unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
	for (uint32_t i = 0; i < width * height * nrComponents; i += nrComponents)
	{
		colors[i%nrComponents] = Color((float)data[i] / 255.0, (float)data[i + 1] / 255.0, (float)data[i + 2] / 255.0, (float)data[i + 3] / 255.0);
	}

	return colors;
}
