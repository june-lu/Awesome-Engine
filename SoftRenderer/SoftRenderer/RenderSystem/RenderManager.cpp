#include "RenderManager.h"



RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

Matrix4f GetModelMatrix(Vector3f angle, Vector3f scale, Vector3f transform)
{
	Matrix4f model(Matrix4f::Identity);

	model.Transform(transform);
	model.scale(scale);
	model.RotateX(angle.x);
	model.RotateY(angle.y);
	model.RotateZ(angle.z);
	return model;
}

Matrix4f GetViewMatrix(Vector3f eyePos)
{
	Matrix4f view(Matrix4f::Identity);
	view.Transform(-1 * eyePos);
	return view;
}

Matrix4f GetProjectionMatrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
	Matrix4f projection(Matrix4f::Identity);
	float tanFov = Mathf::Tan(eye_fov * Mathf::Deg2Rad * 0.5f);
	float t = zNear * tanFov;
	float l = t * aspect_ratio;

	Matrix4f persp2orthoMat(
		zNear, 0, 0, 0,
		0, zNear, 0, 0,
		0, 0, zNear + zFar, -zNear * zFar,
		0, 0, 1, 0);

	projection *= persp2orthoMat;
	std::cout << projection << std::endl;
	projection.scale(Vector3f(1.0 / l, 1.0 / t, 2.0 / (zFar - zNear)));
	std::cout << projection << std::endl;
	Vector3f originPos(1,1,1);
	std::cout << projection(originPos) << std::endl;
	return projection;
}

Matrix4f GetViewPortMatrix(int width, int height)
{
	Matrix4f viewport(
		width * 0.5, 0, 0, width * 0.5,
		0, height * 0.5, 0, height * 0.5,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return viewport;
}

RenderManager::RenderManager(const char* _windowName, int _width, int _height)
{
	sdlInterface = new SDLInterface(_windowName, _width, _height);
	renderContext = new RenderContext();
	renderContext->width = _width;
	renderContext->height = _height;
	renderContext->frameBuffer.resize(_width * _height);
	renderContext->depthBuffer.resize(_width * _height);

	std::fill(renderContext->frameBuffer.begin(), renderContext->frameBuffer.end(), std::numeric_limits<unsigned int>::max());
	std::fill(renderContext->depthBuffer.begin(), renderContext->depthBuffer.end(), std::numeric_limits<float>::infinity());

	rasterizer = new Rasterizer(renderContext);

	Vector3f angle(0, 0, 0);
	Vector3f scale(100, 100, 100);
	Vector3f transform = Vector3f::left * (_width / 2);
	Vector3f eye_pos = { 0, 0, 10 };

	rasterizer->set_model(GetModelMatrix(angle, scale, transform));
	rasterizer->set_view(GetViewMatrix(eye_pos));
	rasterizer->set_projection(GetProjectionMatrix(45.0, 1, 0.1, 50));
	rasterizer->set_viewport(GetViewPortMatrix(_width, _height));
}



void RenderManager::DrawTriangleByBarycentricCoordinates(Color color, Vector3f* pts, ShadedMode shadedmodel)
{
	rasterizer->DrawTriangleByBarycentricCoordinates(color, pts, shadedmodel);
}

void RenderManager::handleEvents()
{
	sdlInterface->handleEvents();
}

void RenderManager::SwapBuffer()
{
	sdlInterface->SwapBuffer(renderContext);
}

void RenderManager::RenderClear()
{

	sdlInterface->RenderClear(&Color::white);
}

void RenderManager::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, ShadedMode shadedMode)
{
	renderContext->shadedMode = shadedMode;
	renderContext->textures = textures;
	for (uint32_t i = 0; i < indices.size() / 3; i++)
	{
		Triangle triangle;
		for (int j = 0; j < 3; j++)
		{
			Vertex vertex = vertices[indices[3 * i + j]];

			triangle.SetVertex(j, vertex.position);
			triangle.SetNormal(j, vertex.normal);
			triangle.SetTexCoord(j, vertex.texCoords);
		}

		renderContext->triangles.push_back(triangle);
	}
	rasterizer->DrawTriangleByBarycentricCoordinates();
}