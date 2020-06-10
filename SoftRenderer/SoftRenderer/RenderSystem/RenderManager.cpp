#include "RenderManager.h"



RenderManager::RenderManager()
{
}

RenderManager::RenderManager(const char* _windowName, int _width, int _height)
{
	sdlInterface = new SDLInterface(_windowName, _width, _height);
	renderContext = new RenderContext();
	renderContext->width = sdlInterface->screenWidth;
	renderContext->height = sdlInterface->screenHeight;
	renderContext->frameBuffer = new uint32_t[sdlInterface->screenWidth * sdlInterface->screenHeight];
	renderContext->depthBuffer = new float[sdlInterface->screenWidth * sdlInterface->screenHeight];

	memset(renderContext->frameBuffer, 0, renderContext->width * renderContext->height * 4);
	memset(renderContext->depthBuffer, 0x4f, renderContext->width * renderContext->height * 4);


	rasterizer = new Rasterizer(renderContext);
}

RenderManager::~RenderManager()
{
}

void RenderManager::DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts, ShaderMode shadermodel)
{
	rasterizer->DrawTriangleByBarycentricCoordinates(color, pts, shadermodel);
}

void RenderManager::handleEvents()
{
	sdlInterface->handleEvents();
}

void RenderManager::SwapBuffer()
{
	sdlInterface->SwapBuffer(renderContext);
}
void RenderManager::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
{
	Vector3f scale(35, 35, 35);
	Vector3f transform = Vector3f::right * (renderContext->width / 2);
	float angle = 0;
	//std::cout << "indices.size(): " << indices.size() << std::endl;

	Texture textureDiffuse = textures[0];
	//Texture textureSpecular = textures[1];


	std::cout << "sizeof(textureDiffuse.colors) : " << sizeof(textureDiffuse.colors) << std::endl;
	/*std::cout << "textureDiffuse.path : " << textureDiffuse.path.C_Str() << std::endl;
	std::cout << "textureDiffuse.width : " << textureDiffuse.width << std::endl;
	std::cout << "textureDiffuse.height : " << textureDiffuse.height << std::endl;*/

	/*std::cout << "textureSpecular.path : " << textureSpecular.path.C_Str() << std::endl;
	std::cout << "textureSpecular.width : " << textureSpecular.width << std::endl;
	std::cout << "textureSpecular.height : " << textureSpecular.height << std::endl;*/

	for (uint32_t i = 0; i < indices.size(); i += 3)
	{

		//std::cout << "vertices[indices[i + 1]].position : " << vertices[indices[i + 1]].position << std::endl;
		//std::cout << "vertices[indices[i + 2]].position : " << vertices[indices[i + 2]].position << std::endl;

		/*std::cout << "i : " << i << std::endl;
		std::cout << "i + 1 : " << i + 1 << std::endl;
		std::cout << "i + 2: " << i + 2 << std::endl;*/


		Vector3f normal = vertices[indices[i]].normal.Normalize();
		Vector2f texCoord = vertices[indices[i]].texCoords;
		Vector3f t[3] = { Vector3f::up * renderContext->height - vertices[indices[i]].position.RotationY(angle) * scale + transform,
			Vector3f::up * renderContext->height - vertices[indices[i + 1]].position.RotationY(angle) * scale + transform,
			Vector3f::up * renderContext->height - vertices[indices[i + 2]].position.RotationY(angle) * scale + transform };

		float dotResult = Vector3f::Dot(normal, Vector3f(1, 0, 0));
		if (dotResult <= 0) dotResult = 0;

		//Color color(normal.x < 0?0:normal.x, normal.y < 0 ? 0 : normal.y, normal.z < 0 ? 0 : normal.z, 1.0);
		Color colorDiffuse = textureDiffuse.colors[(int)(texCoord.x * textureDiffuse.width + texCoord.y * textureDiffuse.height)];
		//Color colorSpecular = textureSpecular.colors[(int)(texCoord.x * textureSpecular.width + texCoord.y * textureSpecular.height)];
		Color color = Color::black;
		//color += colorDiffuse;
		//color += colorSpecular;
		/*std::cout << "color.r : " << color.r << std::endl;
		std::cout << "color.g : " << color.g << std::endl;
		std::cout << "color.b : " << color.b << std::endl;*/
		DrawTriangleByBarycentricCoordinates(&colorDiffuse, t, ShaderMode::Shaded);
	}
}