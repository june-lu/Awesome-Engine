#include "Rasterizer.h"

Rasterizer::Rasterizer() {}

Rasterizer::Rasterizer(RenderContext* _renderContext) :renderContext(_renderContext) {}

Rasterizer::~Rasterizer()
{
}

static Vector3f interpolate(Vector3f bcCoord, const Vector3f* verts, float weight)
{
	return (verts[0] * bcCoord.x + verts[1] * bcCoord.y + verts[2] * bcCoord.z) / weight;
}

static Vector2f interpolate(Vector3f bcCoord, const Vector2f* verts, float weight)
{
	return (verts[0] * bcCoord.x + verts[1] * bcCoord.y + verts[2] * bcCoord.z) / weight;
}

void Rasterizer::SetPixel(Color& color, const int posX, const int posY)
{
	renderContext->frameBuffer[posX + posY * renderContext->width] = color.GetUintA() << 24 | color.GetUintB() << 16 | color.GetUintG() << 8 | color.GetUintR() << 0;
}

//Bresenham’s Line Drawing Algorithm
void Rasterizer::DrawLine(Color color, int x0, int y0, int x1, int y1)
{
	//1 attempt
	/*for (float t = 0.0; t < 1.0; t += 0.01)
	{
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		SetPixel(color, x, y);
	}*/

	//2 attempt
	/*for (int x = x0; x <= x1; x++)
	{
		float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;
		SetPixel(color, x, y);
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
			SetPixel(color, y, x);
		else
			SetPixel(color, x, y);

		error += dyError;
		if (error > dx)
		{
			y += (y1 > y0) ? 1 : -1;
			error -= 2 * dx;
		}
	}
}

void Rasterizer::DrawLine(Color color, Vector2f start, Vector2f end)
{
	Rasterizer::DrawLine(color, start.x, start.y, end.x, end.y);
}

//Draw Triangle by line sweeping
void Rasterizer::DrawTriangleByLineSweeping(Color color, Vector3f* pts, ShadedMode shaderMode)
{
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	Vector2f pos1 = pts[0];
	Vector2f pos2 = pts[1];
	Vector2f pos3 = pts[2];


	if (pos1.y > pos2.y) std::swap(pos1, pos2);
	if (pos1.y > pos3.y) std::swap(pos1, pos3);
	if (pos2.y > pos3.y) std::swap(pos2, pos3);

	if (shaderMode == ShadedMode::Wireframe || shaderMode == ShadedMode::ShadedWireframe)
	{
		Rasterizer::DrawLine(color, pos1, pos2);
		Rasterizer::DrawLine(color, pos2, pos3);
		Rasterizer::DrawLine(color, pos3, pos1);
	}

	if (shaderMode == ShadedMode::Shaded || shaderMode == ShadedMode::ShadedWireframe)
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
				SetPixel(color, x, y);
			}
		}
	}
}

void Rasterizer::DrawTriangleByBarycentricCoordinates(Color color, Vector3f* screenPos, ShadedMode shadedMode)
{
	if (shadedMode == ShadedMode::Shaded || shadedMode == ShadedMode::ShadedWireframe)
	{
		Vector2f boxMin(renderContext->width - 1, renderContext->width - 1);
		Vector2f boxMax(0, 0);
		Vector2f clamp(renderContext->width - 1, renderContext->height - 1);

		for (int i = 0; i < 3; i++)
		{
			boxMin.x = Mathf::Max(0, Mathf::Min(boxMin.x, screenPos[i].x));
			boxMax.x = Mathf::Min(clamp.x, Mathf::Max(boxMax.x, screenPos[i].x));

			boxMin.y = Mathf::Max(0, Mathf::Min(boxMin.y, screenPos[i].y));
			boxMax.y = Mathf::Min(clamp.y, Mathf::Max(boxMax.y, screenPos[i].y));
		}

		Vector3f pos;
		for (pos.x = boxMin.x; pos.x <= boxMax.x; pos.x++)
		{
			for (pos.y = boxMin.y; pos.y <= boxMax.y; pos.y++)
			{
				Vector3f bcCoord = Vector3f::Barycentric(screenPos, pos);
				if (bcCoord.x < 0 || bcCoord.y < 0 || bcCoord.z < 0)continue;
				pos.z = 0;
				pos.z += screenPos[0].z * bcCoord.x;
				pos.z += screenPos[1].z * bcCoord.y;
				pos.z += screenPos[2].z * bcCoord.z;
				int index = (int)(pos.x + pos.y * renderContext->width);
				if (renderContext->depthBuffer[index] - pos.z > 1)
				{
					renderContext->depthBuffer[index] = pos.z;
					SetPixel(color, pos.x, pos.y);
				}
			}
		}
	}

	if (shadedMode == ShadedMode::Wireframe || shadedMode == ShadedMode::ShadedWireframe)
	{
		DrawTriangleByWireframe(Color::white, screenPos);
	}
}

void Rasterizer::DrawTriangleByBarycentricCoordinates()
{
	ShadedMode shadedMode = renderContext->shadedMode;
	Triangle triangle;
	Vector3f screen_coords[3];
	Vector2f texCoords[3];

	//std::cout << renderContext->model << std::endl;

	if (shadedMode == ShadedMode::Shaded || shadedMode == ShadedMode::ShadedWireframe)
	{

		omp_set_num_threads(12);
#pragma omp parallel 
		{
#pragma omp for private(triangle) private(screen_coords) private(texCoords)
			for (int i = 0; i < renderContext->triangles.size(); i++)
			{
				triangle = renderContext->triangles[i];
				std::vector<Texture> textures = renderContext->allTextures[triangle.texturesID];

				for (int j = 0; j < 3; j++)
				{
					//todo MVP变换
					Vector4f modelCoord = renderContext->model(triangle.pos[j]);
					Vector4f viewCoord = renderContext->view(modelCoord);
					Vector4f projectionCoord = renderContext->projection(viewCoord);

					//Homogeneous division
					projectionCoord /= projectionCoord.w;

					//todo 视口变换到屏幕坐标
					Vector3f screen_coord = renderContext->viewport(projectionCoord);
					screen_coords[j] = screen_coord;

					texCoords[j] = triangle.texCoords[j];
				}

				//光栅化
				Vector2f boxMin(renderContext->width - 1, renderContext->width - 1);
				Vector2f boxMax(0, 0);
				Vector2f clamp(renderContext->width - 1, renderContext->height - 1);

				for (int i = 0; i < 3; i++)
				{
					boxMin.x = Mathf::Max(0, Mathf::Min(boxMin.x, screen_coords[i].x));
					boxMax.x = Mathf::Min(clamp.x, Mathf::Max(boxMax.x, screen_coords[i].x));

					boxMin.y = Mathf::Max(0, Mathf::Min(boxMin.y, screen_coords[i].y));
					boxMax.y = Mathf::Min(clamp.y, Mathf::Max(boxMax.y, screen_coords[i].y));
				}

				int x, y, z = 0;
				for (x = boxMin.x; x <= boxMax.x; x++)
				{
					for (y = boxMin.y; y <= boxMax.y; y++)
					{
						Vector3f bcCoord = Vector3f::Barycentric(screen_coords, Vector3f(x, y, z));
						if (bcCoord.x < 0 || bcCoord.y < 0 || bcCoord.z < 0)continue;
						Vector3f interpolated_screencoords = interpolate(bcCoord, screen_coords, 1.0);
						Vector3f interpolated_texcoords = interpolate(bcCoord, texCoords, 1.0);
						Color color = Color::black;//textures[0].Sample(interpolated_texcoords.x, interpolated_texcoords.y);
						//std::cout<< textures[k].Sample(interpolated_texcoords.x, interpolated_texcoords.y)>>std::endl;
						for (int k = 1; k < textures.size(); k++)
						{
							color += textures[k].Sample(interpolated_texcoords.x, interpolated_texcoords.y);
						}
						float depth = interpolated_screencoords.z;
						int index = x + y * renderContext->width;

						if (renderContext->depthBuffer[index] > depth)
						{
							renderContext->depthBuffer[index] = depth;
							SetPixel(color, x, y);
						}
					}
				}
			}
		}
	}
}

void Rasterizer::DrawTriangleByWireframe(Color color, Vector3f* pts)
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

void Rasterizer::set_model(const Matrix4f& m)
{
	renderContext->model = m;
}
void Rasterizer::set_view(const Matrix4f& v)
{
	renderContext->view = v;
}
void Rasterizer::set_projection(const Matrix4f& p)
{
	renderContext->projection = p;
}
void Rasterizer::set_viewport(const Matrix4f& vp)
{
	renderContext->viewport = vp;
}