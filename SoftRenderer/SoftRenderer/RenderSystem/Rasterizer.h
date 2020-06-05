#pragma once

#include "Base/Color.h"
#include "Base/Vector2.h"
#include "Base/Vector3.h"
#include "RenderContext.h"

class Rasterizer
{
public:
	Rasterizer();
	Rasterizer(RenderContext* renderContext);
	~Rasterizer();

	void DrawPixel(Color* color, int posX, int poxY);
	void DrawLine(Color* color, int x0, int y0, int x1, int y1);
	void DrawLine(Color* color, Vector2f start, Vector2f end);
	void DrawTriangleByLineSweeping(Color* color, Vector2f* pts);
	void DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts);

	RenderContext* renderContext;

};

