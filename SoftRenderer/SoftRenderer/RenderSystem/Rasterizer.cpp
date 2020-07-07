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
	//std::cout << "posX + posY * renderContext->width : " << posX + posY * renderContext->width << std::endl;
	renderContext->frameBuffer[posX + posY * renderContext->width] = (0 << 24) | (color.GetUintB() << 16) | (color.GetUintG() << 8) | (color.GetUintR() << 0);

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
	Vector3f screenCoords[3];
	Vector2f texCoords[3];
	Vector3f viewCoords[3];
	Vector3f normals[3];
	Vector3f colors[3];
	std::vector<Vertex> projectionCoords;

	omp_set_num_threads(12);
#pragma omp parallel 
	{
#pragma omp for private(triangle) private(screenCoords) private(texCoords) private(viewCoords) private(normals) private(projectionCoords)

		for (int i = 0; i < renderContext->triangles.size(); i++)
		{
			std::vector<Texture> textures;
			triangle = renderContext->triangles[i];
			if (triangle.texturesID > 0)
			{
				textures = renderContext->allTextures[triangle.texturesID];
			}

			for (int j = 0; j < 3; j++)
			{
				//todo MVP变换
				Vector4f modelCoord = renderContext->model(triangle.pos[j]);
				Vector4f viewCoord = renderContext->view(modelCoord);

				viewCoords[j] = viewCoord;
				Vector4f projectionCoord = renderContext->projection(viewCoord);
				//Homogeneous division
				projectionCoord /= projectionCoord.w;
				Vertex projectionVertex;
				projectionVertex.position = projectionCoord;

				if (triangle.texCoords != NULL)
				{
					projectionVertex.texCoords = triangle.texCoords[j];
				}
				projectionVertex.normal = triangle.normal[j];
				projectionVertex.color = triangle.color[j];
				projectionCoords.push_back(projectionVertex);
			}

			projectionCoords = ClippingBySixPlane(projectionCoords);

			int len = projectionCoords.size() - 2;
			for (int j = 0; j < len; j++)
			{
				//todo 视口变换到屏幕坐标
				screenCoords[0] = renderContext->viewport(projectionCoords[0].position);
				texCoords[0] = projectionCoords[0].texCoords;
				normals[0] = projectionCoords[0].normal;
				colors[0] = projectionCoords[0].color.ToVec3();
				for (int k = 1; k <= 2; k++)
				{
					screenCoords[k] = renderContext->viewport(projectionCoords[j + k].position);
					texCoords[k] = projectionCoords[j + k].texCoords;
					normals[k] = projectionCoords[j + k].normal;
					colors[k] = projectionCoords[j + k].color.ToVec3();
				}

				//绘制线框
				if (shadedMode == ShadedMode::Wireframe || shadedMode == ShadedMode::ShadedWireframe)
				{

					DrawTriangleByWireframe(Color::black, screenCoords);
				}

				if (shadedMode == ShadedMode::Shaded || shadedMode == ShadedMode::ShadedWireframe)
				{
					//光栅化
					Vector2f boxMin(renderContext->width - 1, renderContext->width - 1);
					Vector2f boxMax(0, 0);
					Vector2f clamp(renderContext->width - 1, renderContext->height - 1);

					for (int i = 0; i < 3; i++)
					{
						boxMin.x = Mathf::Max(0, Mathf::Min(boxMin.x, screenCoords[i].x));
						boxMax.x = Mathf::Min(clamp.x, Mathf::Max(boxMax.x, screenCoords[i].x));

						boxMin.y = Mathf::Max(0, Mathf::Min(boxMin.y, screenCoords[i].y));
						boxMax.y = Mathf::Min(clamp.y, Mathf::Max(boxMax.y, screenCoords[i].y));
					}

					int x, y, z = 0;
					for (x = boxMin.x; x <= boxMax.x; x++)
					{
						for (y = boxMin.y; y <= boxMax.y; y++)
						{
							Vector3f bcCoord = Vector3f::Barycentric(screenCoords, Vector3f(x, y, z));
							if (bcCoord.x < 0 || bcCoord.y < 0 || bcCoord.z < 0)continue;

							Vector3f interpolatedScreenCoords = interpolate(bcCoord, screenCoords, 1.0);
							float depth = interpolatedScreenCoords.z;
							int index = x + y * renderContext->width;

							if (renderContext->depthBuffer[index] > depth)
							{
								Vector3f interpolated_texcoords = interpolate(bcCoord, texCoords, 1.0);
								Vector3f interpolatedNormal = interpolate(bcCoord, normals, 1.0);
								Vector3f interpolatedViewCoords = interpolate(bcCoord, viewCoords, 1.0);
								Vector3f interpolatedColor = interpolate(bcCoord, colors, 1.0);

								Color color = Color::black;

								if (renderContext->shader != NULL && renderContext->shader->fragmentShader != NULL)
								{
									FragmentShader fragShader;
									fragShader.color = Color(interpolatedColor.x, interpolatedColor.y, interpolatedColor.z, 1.0);
									fragShader.normal = interpolatedNormal;
									fragShader.texCoords = interpolated_texcoords;
									fragShader.textures = textures;
									fragShader.viewPos = interpolatedViewCoords;
									fragShader.eyePos = renderContext->camera.GetCameraPosition();

									Vector3f piexlColor = renderContext->shader->fragmentShader(fragShader);
									color = Color(piexlColor.x, piexlColor.y, piexlColor.z, 1.0);
								}
								renderContext->depthBuffer[index] = depth;
								SetPixel(color, x, y);
							}
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
	//Vector2f clamp(renderContext->width - 1, renderContext->height - 1);
	//for (int i = 0; i < 3; i++)
	//{
	//	pts[i].x = Mathf::Max(0, pts[i].x);
	//	pts[i].y = Mathf::Max(0, pts[i].y);

	//	pts[i].x = Mathf::Min(clamp.x, pts[i].x);
	//	pts[i].y = Mathf::Min(clamp.y, pts[i].y);
	//}

	Vector2f pos1 = pts[0];
	Vector2f pos2 = pts[1];
	Vector2f pos3 = pts[2];


	if (pos1.y > pos2.y) std::swap(pos1, pos2);
	if (pos1.y > pos3.y) std::swap(pos1, pos3);
	if (pos2.y > pos3.y) std::swap(pos2, pos3);

	Rasterizer::DrawLine(Color::red, pos1, pos2);
	Rasterizer::DrawLine(Color::green, pos2, pos3);
	Rasterizer::DrawLine(Color::blue, pos3, pos1);
}

void Rasterizer::SetModel(const Matrix4f& m)
{
	renderContext->model = m;
}
void Rasterizer::SetView(const Matrix4f& v)
{
	renderContext->view = v;
}
void Rasterizer::SetProjection(const Matrix4f& p)
{
	renderContext->projection = p;
}
void Rasterizer::SetViewport(const Matrix4f& vp)
{
	renderContext->viewport = vp;
}
std::vector<Vertex> Rasterizer::ClippingByPlane(std::vector<Vertex> projectionCoords, Vector3f planePos, Vector3f planeNormal)
{
	std::vector<Vertex> inViewCoords;
	if (projectionCoords.size() <= 0) return inViewCoords;
	float iDot = Dot(projectionCoords[0].position - planePos, planeNormal);
	float preDot = iDot;
	if (iDot > 0)
		inViewCoords.push_back(projectionCoords[0]);

	//std::cout << projectionCoords[0] << std::endl;
	for (int i = 1; i < projectionCoords.size(); i++)
	{
		Vertex projectionCoord = projectionCoords[i];
		float curDot = Dot(projectionCoord.position - planePos, planeNormal);
		//有一个交点
		if (curDot * preDot <= 0)
		{
			Vertex intersection = InterpolateVertex(projectionCoords[i - 1], projectionCoords[i], preDot / (preDot - curDot));
			inViewCoords.push_back(intersection);
		}
		if (curDot > 0)
		{
			inViewCoords.push_back(projectionCoord);
		}
		preDot = curDot;
	}
	if (iDot * preDot <= 0)
	{
		Vertex intersection = InterpolateVertex(projectionCoords[projectionCoords.size() - 1], projectionCoords[0], preDot / (preDot - iDot));
		inViewCoords.push_back(intersection);
	}
	return inViewCoords;
}

std::vector<Vertex> Rasterizer::ClippingBySixPlane(std::vector<Vertex> projectionCoords)
{
	std::vector<Vertex> inViewCoords = projectionCoords;
	inViewCoords = ClippingByPlane(inViewCoords, Vector3f::up, Vector3f::down);
	inViewCoords = ClippingByPlane(inViewCoords, Vector3f::down, Vector3f::up);
	inViewCoords = ClippingByPlane(inViewCoords, Vector3f::left, Vector3f::right);
	inViewCoords = ClippingByPlane(inViewCoords, Vector3f::right, Vector3f::left);
	inViewCoords = ClippingByPlane(inViewCoords, Vector3f::front, Vector3f::back);
	inViewCoords = ClippingByPlane(inViewCoords, Vector3f::back, Vector3f::front);
	return inViewCoords;
}

Vertex Rasterizer::InterpolateVertex(Vertex startVertex, Vertex endVertex, float t)
{
	Vertex vertex;
	vertex.position = startVertex.position + (endVertex.position - startVertex.position) * t;
	vertex.normal = startVertex.normal + (endVertex.normal - startVertex.normal) * t;
	vertex.color = Color::Lerp(startVertex.color, endVertex.color, t);
	vertex.texCoords = startVertex.texCoords + (endVertex.texCoords - startVertex.texCoords) * t;
	return vertex;
}