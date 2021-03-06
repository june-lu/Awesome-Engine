#include "RenderManager.h"



RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
	delete sdlInterface;
	delete	renderContext;
}

Matrix4f GetModelMatrix(Vector3f angle, Vector3f scale, Vector3f transform)
{
	Matrix4f model(Matrix4f::Identity);
	model.scale(scale);
	model.Transform(transform);
	model.RotateX(angle.x);
	model.RotateY(angle.y);
	model.RotateZ(angle.z);

	return model;
}

Matrix4f GetViewMatrix(Camera camera)
{
	Matrix4f view(Matrix4f::Identity);
	Vector3f up = camera.GetCameraUpDirection();
	Vector3f forward = camera.GetCameraForwardDirection();
	Vector3f right = camera.GetCameraRightDirection();
	Vector3f cameraPos = camera.GetCameraPosition();
	
	Matrix4f viewMat(
		right.x, up.x, -forward.x, -cameraPos.x,
		right.y, up.y, -forward.y, -cameraPos.y,
		right.z, up.z, -forward.z, -cameraPos.z,
		0, 0, 0, 1);

	//Matrix4f viewMat(
	//	right.x, right.y, right.z, 0,
	//	up.x, up.y, up.z, 0,
	//	-forward.x, -forward.y, -forward.z, 0,
	//	0, 0, 0, 1);

	//view.Transform(-1 * cameraPos);
	view = viewMat * view;
	
	return viewMat;
}

Matrix4f GetProjectionMatrix(float eye_fov, float aspect_ratio, float n, float f)
{
	Matrix4f projection(Matrix4f::Identity);
	float tanFov = Mathf::Tan(eye_fov * Mathf::Deg2Rad * 0.5f);
	float t = tanFov * n;
	float b = -t;

	float r = t * aspect_ratio;
	float l = -r;

	Matrix4f matTran = Matrix4f(
		1, 0, 0, -(r + l) *0.5,
		0, 1, 0, -(t + b) *0.5,
		0, 0, 1, -(n + f) *0.5,
		0, 0, 0, 1);

	Matrix4f matScale = Matrix4f(
		2 / (r - l), 0, 0, 0,
		0, 2 / (t - b), 0, 0,
		0, 0, 2 / (f - n), 0,
		0, 0, 0, 1);

	projection = matScale * matTran;

	/*projection = Matrix4f(
		2 * n / (r - l), 0, (r + l) / (r - l), 0,
		0, 2 * n / (t - b), (t + b) / (t - b), 0,
		0, 0, (n + f) / (n - f), (2 * n * f) / (n - f),
		0, 0, 1, 0);*/

	Matrix4f persp2orthoMat(
		n, 0, 0, 0,
		0, n, 0, 0,
		0, 0, n + f, n * f,
		0, 0, 1, 0);


	projection *= persp2orthoMat;
	//std::cout << projection << std::endl;
	//projection.scale(Vector3f(1.0 / left, 1.0 / bottom, 2.0 / (zFar - zNear)));
	//std::cout << projection << std::endl;
	/*Vector3f originPos(1, 1, 1);
	std::cout << projection(originPos) << std::endl;*/
	return projection;
}

Matrix4f GetViewPortMatrix(int width, int height, float zNear, float zFar)
{
	width--;
	height--;
	Matrix4f viewport(
		width * 0.5, 0, 0, width * 0.5,
		0, height * 0.5, 0, height * 0.5,
		0, 0, (zFar - zNear) / 2.0, (zFar + zNear) / 2.0,
		0, 0, 0, 1);
	return viewport;
}

RenderManager::RenderManager(const char* _windowName, int _width, int _height)
{
	int samplingNum = 2;
	sdlInterface = new SDLInterface(_windowName, _width, _height);
	renderContext = new RenderContext();
	renderContext->width = _width;
	renderContext->height = _height;
	renderContext->frameBuffer.resize(_width * _height);
	renderContext->MASSFrameBuffer.resize(_width * _height * samplingNum * samplingNum);
	renderContext->depthBuffer.resize(_width * _height);
	renderContext->MASSDepthBuffer.resize(_width * _height * samplingNum * samplingNum);

	std::fill(renderContext->frameBuffer.begin(), renderContext->frameBuffer.end(), std::numeric_limits<unsigned int>::max());
	std::fill(renderContext->MASSFrameBuffer.begin(), renderContext->MASSFrameBuffer.end(), std::numeric_limits<unsigned int>::max());
	std::fill(renderContext->depthBuffer.begin(), renderContext->depthBuffer.end(), std::numeric_limits<float>::max());
	std::fill(renderContext->MASSDepthBuffer.begin(), renderContext->MASSDepthBuffer.end(), std::numeric_limits<float>::max());

	rasterizer = new Rasterizer(renderContext);

	Vector3f angle(0, 0, 0);
	Vector3f scale(1, 1, 1);
	Vector3f transform = Vector3f(0, -8, 0);
	float zNear = 0.1;
	float zFar = 100;

	rasterizer->SetModel(GetModelMatrix(angle, scale, transform));

	rasterizer->SetProjection(GetProjectionMatrix(60, _width / _height, zNear, zFar));
	rasterizer->SetViewport(GetViewPortMatrix(_width, _height, zNear, zFar));
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
	Color color = Color::black;
	Uint32 col = color.GetUintA() << 24 | color.GetUintB() << 16 | color.GetUintG() << 8 | color.GetUintR() << 0;
	std::fill(renderContext->frameBuffer.begin(), renderContext->frameBuffer.end(), col);
	std::fill(renderContext->MASSFrameBuffer.begin(), renderContext->MASSFrameBuffer.end(), col);
	std::fill(renderContext->depthBuffer.begin(), renderContext->depthBuffer.end(), std::numeric_limits<float>::max());
	std::fill(renderContext->MASSDepthBuffer.begin(), renderContext->MASSDepthBuffer.end(), std::numeric_limits<float>::max());
}
void RenderManager::DrawMesh(Shader* shader, std::vector<Vertex> vertices, std::vector<uint32_t> indices, int texturesID, ShadedMode shadedMode)
{
	renderContext->shadedMode = shadedMode;
	renderContext->shader = shader;

	for (uint32_t i = 0; i < indices.size() / 3; i++)
	{
		Triangle triangle;
		for (int j = 0; j < 3; j++)
		{
			Vertex vertex = vertices[indices[3 * i + j]];

			triangle.SetVertex(j, vertex.position);
			triangle.SetNormal(j, vertex.normal);
			triangle.SetColor(j, vertex.color);
			triangle.SetTexCoord(j, vertex.texCoords);
			triangle.texturesID = texturesID;
		}

		renderContext->triangles.push_back(triangle);
	}
	rasterizer->DrawTriangleByBarycentricCoordinates();
}

void RenderManager::DrawMesh(Shader* shader, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, ShadedMode shadedMode)
{
	int texturesID = rasterizer->SaveTexture(textures);
	DrawMesh(shader, vertices, indices, texturesID, shadedMode);
}

void RenderManager::SetCamera(Camera& camera)
{
	renderContext->camera = camera;
	rasterizer->SetView(GetViewMatrix(camera));
}

void RenderManager::Draw()
{
	rasterizer->DrawTriangleByBarycentricCoordinates();
}