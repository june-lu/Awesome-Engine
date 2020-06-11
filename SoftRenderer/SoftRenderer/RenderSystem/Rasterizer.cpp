#include "Rasterizer.h"

Rasterizer::Rasterizer() {}

Rasterizer::Rasterizer(RenderContext* _renderContext) :renderContext(_renderContext) {}

Rasterizer::~Rasterizer()
{
}


void Rasterizer::DrawPixel(Color* color, int posX, int posY)
{
	/*SetDrawColor(color);
	SDL_RenderDrawPoint(renderer, posX, posY);*/
	renderContext->frameBuffer[posX + posY * renderContext->width] = (color->GetUintA() << 24) | (color->GetUintB() << 16)
		| (color->GetUintG() << 8) | (color->GetUintR() << 0);
}

//Bresenham’s Line Drawing Algorithm
void Rasterizer::DrawLine(Color* color, int x0, int y0, int x1, int y1)
{
	//1 attempt
	/*for (float t = 0.0; t < 1.0; t += 0.01)
	{
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		DrawPixel(color, x, y);
	}*/

	//2 attempt
	/*for (int x = x0; x <= x1; x++)
	{
		float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;
		DrawPixel(color, x, y);
	}*/



	//3 attempt
	bool steep = false;
	if (Mathf::Abs(x1 - x0) < Mathf::Abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	//4 attempt
	//通过误差提高运行效率
	int dyError = Mathf::Abs(dy * 2);
	int error = 0;

	float y = y0;
	for (int x = x0; x <= x1; x++)
	{
		/*float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;*/

		if (steep)
			DrawPixel(color, y, x);
		else
			DrawPixel(color, x, y);

		error += dyError;
		if (error > dx)
		{
			y += (y1 > y0) ? 1 : -1;
			error -= 2 * dx;
		}
	}
}

void Rasterizer::DrawLine(Color* color, Vector2f start, Vector2f end)
{
	Rasterizer::DrawLine(color, start.x, start.y, end.x, end.y);
}

//Draw Triangle by line sweeping
void Rasterizer::DrawTriangleByLineSweeping(Color* color, Vector3f* pts, ShaderMode shaderMode)
{
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	Vector2f pos1 = pts[0];
	Vector2f pos2 = pts[1];
	Vector2f pos3 = pts[2];


	if (pos1.y > pos2.y) std::swap(pos1, pos2);
	if (pos1.y > pos3.y) std::swap(pos1, pos3);
	if (pos2.y > pos3.y) std::swap(pos2, pos3);

	if (shaderMode == ShaderMode::Wireframe || shaderMode == ShaderMode::ShadedWireframe)
	{
		Rasterizer::DrawLine(color, pos1, pos2);
		Rasterizer::DrawLine(color, pos2, pos3);
		Rasterizer::DrawLine(color, pos3, pos1);
	}

	if (shaderMode == ShaderMode::Shaded || shaderMode == ShaderMode::ShadedWireframe)
	{

		int total_height = pos3.y - pos1.y;
		int segment_height; float beta; Vector2f B;
		for (int y = pos1.y; y <= pos3.y; y++)
		{
			bool second_half = y > pos2.y - pos1.y || pos2.y == pos1.y;
			float alpha = (float)(y - pos1.y) / total_height;
			Vector2f A = pos1 + (pos3 - pos1)*alpha;
			segment_height = second_half ? pos3.y - pos2.y : pos2.y - pos1.y;
			beta = (float)(y - (second_half ? pos2.y : pos1.y)) / segment_height;
			B = (second_half ? pos2 + (pos3 - pos2)*beta : pos1 + (pos2 - pos1)*beta);

			if (A.x > B.x) std::swap(A, B);
			for (int x = A.x; x <= B.x; x++)
			{
				DrawPixel(color, x, y);
			}
		}
	}
}

void Rasterizer::DrawTriangleByBarycentricCoordinates(Color* color, Vector3f* pts, ShaderMode shaderMode)
{
	if (shaderMode == ShaderMode::Wireframe || shaderMode == ShaderMode::ShadedWireframe)
	{
		DrawTriangleByWireframe(color, pts);
	}

	if (shaderMode == ShaderMode::Shaded || shaderMode == ShaderMode::ShadedWireframe)
	{
		Vector2f boxMin(1000000000.0, 1000000000.0);
		Vector2f boxMax(-1000000000.0, -1000000000.0);
		Vector2f clamp(renderContext->width - 1, renderContext->height - 1);

		//std::cout << "boxMin : " <<boxMin << std::endl;
		//std::cout << "boxMax : " << boxMax << std::endl;

		for (int i = 0; i < 3; i++)
		{
			boxMin.x = Mathf::Max(0, Mathf::Min(boxMin.x, pts[i].x));
			boxMin.y = Mathf::Max(0, Mathf::Min(boxMin.y, pts[i].y));

			boxMax.x = Mathf::Min(clamp.x, Mathf::Max(boxMax.x, pts[i].x));
			boxMax.y = Mathf::Min(clamp.y, Mathf::Max(boxMax.y, pts[i].y));
		}

		Vector3f pos;
		for (pos.x = boxMin.x; pos.x <= boxMax.x; pos.x++)
		{
			for (pos.y = boxMin.y; pos.y <= boxMax.y; pos.y++)
			{
				Vector3f bcCoord = Vector3f::Barycentric(pts, pos);
				if (bcCoord.x < 0 || bcCoord.y < 0 || bcCoord.z < 0)continue;
				pos.z = 0;
				pos.z = pts[0].z * bcCoord.x + pts[1].z * bcCoord.y + pts[2].z * bcCoord.z;
				int index = (int)(pos.x + pos.y * renderContext->width);
				//std::cout << "pos.z : " << pos.z << std::endl;
				if (renderContext->depthBuffer[index] - pos.z > 500)
				{
					renderContext->depthBuffer[index] = pos.z;
					DrawPixel(color, pos.x, pos.y);
				}
			}
		}
	}
}

void Rasterizer::DrawTriangleByWireframe(Color* color, Vector3f* pts)
{
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 

	Vector2f clamp(renderContext->width - 1, renderContext->height - 1);
	for (int i = 0; i < 3; i++)
	{
		pts[i].x = Mathf::Max(0, pts[i].x);
		pts[i].y = Mathf::Max(0, pts[i].y);

		pts[i].x = Mathf::Min(clamp.x, pts[i].x);
		pts[i].y = Mathf::Min(clamp.y, pts[i].y);
	}

	Vector2f pos1 = pts[0];
	Vector2f pos2 = pts[1];
	Vector2f pos3 = pts[2];

	if (pos1.y > pos2.y) std::swap(pos1, pos2);
	if (pos1.y > pos3.y) std::swap(pos1, pos3);
	if (pos2.y > pos3.y) std::swap(pos2, pos3);

	Rasterizer::DrawLine(color, pos1, pos2);
	Rasterizer::DrawLine(color, pos2, pos3);
	Rasterizer::DrawLine(color, pos3, pos1);
}