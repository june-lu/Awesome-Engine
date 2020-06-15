#pragma once
#include <functional>
#include "Base/Color.h"
#include "RenderContext.h"
#include "Base/Texture.h"
#include "Base/Vertex.h"
#include "Math/Matrix.hpp"
#include "RenderSystem/Shader.h"

enum ShaderMode
{
	Shaded,
	Wireframe,
	ShadedWireframe
};

class Rasterizer
{
public:
	Rasterizer();
	Rasterizer(RenderContext* renderContext);
	~Rasterizer();

	void DrawPixel(Color color, int posX, int poxY);
	void DrawLine(Color color, int x0, int y0, int x1, int y1);
	void DrawLine(Color color, Vector2f start, Vector2f end);
	void DrawTriangleByLineSweeping(Color color, Vector3f* pts, ShaderMode shaderMode = ShaderMode::Shaded);
	void DrawTriangleByWireframe(Color color, Vector3f* pts);
	void DrawTriangleByBarycentricCoordinates(Color color, Vector3f* pts, ShaderMode shaderMode = ShaderMode::Shaded);
	void DrawTriangleByBarycentricCoordinates(std::vector<Texture> textures, Vertex* vertexs, ShaderMode shaderMode = ShaderMode::Shaded);

	void set_model(const Matrix& m);
	void set_view(const Matrix& v);
	void set_projection(const Matrix& p);

	void set_texture(Texture tex) { renderContext->texture = tex; }

	void set_vertex_shader(std::function<Vector3f(vertex_shader_payload)> vert_shader);
	void set_fragment_shader(std::function<Vector3f(fragment_shader_payload)> frag_shader);

	void SetPixel(const Vector2i &point, const Color &color);

	RenderContext* renderContext;

};

