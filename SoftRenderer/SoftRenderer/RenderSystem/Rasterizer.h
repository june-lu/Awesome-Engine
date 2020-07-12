#pragma once
#include <functional>
#include "Base/Color.h"
#include "RenderContext.h"
#include "Base/Texture.h"
#include "Base/Vertex.h"
#include "Math/Matrix.hpp"
#include "RenderSystem/Shader.hpp"
#include <omp.h>

class Rasterizer
{
public:
	Rasterizer();
	Rasterizer(RenderContext* renderContext);
	~Rasterizer();

	void DrawLine(Color color, int x0, int y0, int x1, int y1);
	void DrawLine(Color color, Vector2f start, Vector2f end);
	void DrawTriangleByLineSweeping(Color color, Vector3f* pts, ShadedMode shadedMode = ShadedMode::Shaded);
	void DrawTriangleByWireframe(Color color, Vector3f* pts);
	void DrawTriangleByBarycentricCoordinates();

	void SetModel(const Matrix4f& m);
	void SetView(const Matrix4f& v);
	void SetProjection(const Matrix4f& p);
	void SetViewport(const Matrix4f& vp);

	int SaveTexture(std::vector<Texture> textures) {
		int textureID = renderContext->allTextures.size();
		renderContext->allTextures.push_back(textures);
		return textureID;
	}

	void SetPixel(Color &color, const int posX, const int posY);
	bool Cull(Vector3f* coords, Vector3f cameraPos);
	std::vector<Vertex> ClippingByPlane(std::vector<Vertex> projectionCoords, Vector3f planePos, Vector3f planeNormal);
	std::vector<Vertex> ClippingBySixPlane(std::vector<Vertex> projectionCoords);
	Vertex InterpolateVertex(Vertex startVertex, Vertex endVertex, float t);
	void MSAA(Triangle &triangle, const int posX, const int posY);
	bool insideTriangle(float x, float y, Vector3f* _v);
	Color GetColorByFragment(Triangle &triangle, const Vector3f bcCoord);
	RenderContext* renderContext;

};

