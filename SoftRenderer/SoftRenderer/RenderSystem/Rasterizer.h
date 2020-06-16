#pragma once
#include <functional>
#include "Base/Color.h"
#include "RenderContext.h"
#include "Base/Texture.h"
#include "Base/Vertex.h"
#include "Math/Matrix.hpp"
#include "RenderSystem/Shader.h"


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
	void DrawTriangleByBarycentricCoordinates(Color color, Vector3f* pts, ShadedMode shadedMode = ShadedMode::Shaded);
	void DrawTriangleByBarycentricCoordinates();

	void set_model(const Matrix& m);
	void set_view(const Matrix& v);
	void set_projection(const Matrix& p);

	void set_texture(Texture tex) { renderContext->texture = tex; }

	void set_vertex_shader(std::function<Vector3f(vertex_shader_payload)> vert_shader);
	void set_fragment_shader(std::function<Vector3f(fragment_shader_payload)> frag_shader);

	void SetPixel(Color &color, const int posX, const int posY);

	RenderContext* renderContext;

};

