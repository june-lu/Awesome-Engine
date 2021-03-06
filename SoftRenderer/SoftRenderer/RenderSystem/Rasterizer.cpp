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
	renderContext->frameBuffer[posX + posY * renderContext->width] = (color.GetUintA() << 24) | (color.GetUintB() << 16) | (color.GetUintG() << 8) | (color.GetUintR() << 0);
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

void Rasterizer::DrawTriangleByBarycentricCoordinates()
{
	ShadedMode shadedMode = renderContext->shadedMode;
	Triangle triangle;
	std::vector<Vertex> projectionCoords;
	//omp_set_num_threads(12);
//#pragma omp parallel 
	{
		//#pragma omp for private(triangle) private(projectionCoords)

		for (int i = 0; i < renderContext->triangles.size(); i++)
		{
			triangle = renderContext->triangles[i];

			Triangle newTriangle;
			newTriangle.texturesID = triangle.texturesID;
			projectionCoords.clear();
			for (int j = 0; j < 3; j++)
			{
				//todo MVP变换
				Vector4f modelCoord = renderContext->model(triangle.pos[j]);
				Vector4f viewCoord = renderContext->view(modelCoord);
				newTriangle.viewCoords[j] = viewCoord;
			}

			if (Cull(newTriangle.viewCoords, renderContext->camera.GetCameraPosition())) continue;

			for (int j = 0; j < 3; j++)
			{
				Vector4f projectionCoord = renderContext->projection(newTriangle.viewCoords[j]);
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

			//projectionCoords = ClippingBySixPlane(projectionCoords);

			int len = projectionCoords.size() - 2;

			//std::cout << len << std::endl;
			for (int j = 0; j < len; j++)
			{
				//todo 视口变换到屏幕坐标
				newTriangle.screenCoords[0] = renderContext->viewport(projectionCoords[0].position);
				newTriangle.texCoords[0] = projectionCoords[0].texCoords;
				newTriangle.normal[0] = projectionCoords[0].normal;
				newTriangle.color[0] = projectionCoords[0].color;
				for (int k = 1; k <= 2; k++)
				{
					newTriangle.screenCoords[k] = renderContext->viewport(projectionCoords[j + k].position);
					newTriangle.texCoords[k] = projectionCoords[j + k].texCoords;
					newTriangle.normal[k] = projectionCoords[j + k].normal;
					newTriangle.color[k] = projectionCoords[j + k].color;
				}
				if (shadedMode == ShadedMode::Shaded || shadedMode == ShadedMode::ShadedWireframe)
				{
					//光栅化
					Vector2f boxMin(renderContext->width - 1, renderContext->width - 1);
					Vector2f boxMax(0, 0);
					Vector2f clamp(renderContext->width - 1, renderContext->height - 1);

					for (int i = 0; i < 3; i++)
					{
						boxMin.x = Mathf::Max(0, Mathf::Min(boxMin.x, newTriangle.screenCoords[i].x));
						boxMax.x = Mathf::Min(clamp.x, Mathf::Max(boxMax.x, newTriangle.screenCoords[i].x));

						boxMin.y = Mathf::Max(0, Mathf::Min(boxMin.y, newTriangle.screenCoords[i].y));
						boxMax.y = Mathf::Min(clamp.y, Mathf::Max(boxMax.y, newTriangle.screenCoords[i].y));
					}

					int x, y, z = 0;
					for (x = boxMin.x; x <= boxMax.x; x++)
					{
						for (y = boxMin.y; y <= boxMax.y; y++)
						{
							MSAA(newTriangle, x, y);
						}
					}
				}

				////绘制线框
				//if (shadedMode == ShadedMode::Wireframe || shadedMode == ShadedMode::ShadedWireframe)
				//{

				//	DrawTriangleByWireframe(Color::black, newTriangle.screenCoords);
				//}
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

	Rasterizer::DrawLine(color, pos1, pos2);
	Rasterizer::DrawLine(color, pos2, pos3);
	Rasterizer::DrawLine(color, pos3, pos1);
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

bool Rasterizer::Cull(Vector3f* screenCoords, Vector3f cameraPos)
{
	Vector3f v01 = screenCoords[1] - screenCoords[0];
	Vector3f v02 = screenCoords[2] - screenCoords[0];
	Vector3f cross = Cross(v01, v02);
	Vector3f centerPos = (screenCoords[0] + screenCoords[1] + screenCoords[2]) / 3.0;
	Vector3f view = centerPos - cameraPos;

	if (Dot(cross, view) >= 0)
	{
		return false;
	}
	return true;
}
void Rasterizer::MSAA(Triangle &triangle, const int posX, const int posY)
{
	float sampingNum = 2;
	Vector3f bcCoord1 = Vector3f::Barycentric(triangle.screenCoords, Vector3f(posX, posY, 0));
	if (bcCoord1.x < 0 || bcCoord1.y < 0 || bcCoord1.z < 0) return;
	Color newColor = GetColorByFragment(triangle, bcCoord1);
	Color color = Color::black;
	int coverNum = 0;
	for (int i = 0; i < sampingNum; i++)
	{
		for (int j = 0; j < sampingNum; j++)
		{

			float tempX = posX + (i - 1.0 / (float)sampingNum) * 0.5;
			float tempY = posY + (j - 1.0 / (float)sampingNum) * 0.5;

			float indX = posX * sampingNum + i;
			float indY = posY * sampingNum + j;
			Vector3f bcCoord = Vector3f::Barycentric(triangle.screenCoords, Vector3f(tempX, tempY, 0));
			//if (bcCoord.x < 0 || bcCoord.y < 0 || bcCoord.z < 0) continue;
			//std::cout << "bcCoord : " << bcCoord << std::endl;
			Vector3f interpolatedScreenCoords = interpolate(bcCoord, triangle.screenCoords, 1.0);
			float depth = interpolatedScreenCoords.z;
			int index = indX + indY * renderContext->width;
			if (bcCoord.x >= 0 && bcCoord.y >= 0 && bcCoord.z >= 0)
				//	if (insideTriangle(tempX, tempY, triangle.screenCoords))
			{
				if (renderContext->MASSDepthBuffer[index] > depth)
				{
					if (renderContext->MASSDepthBuffer[index] < std::numeric_limits<unsigned int>::max())
					{
						Color curColor = renderContext->MASSFrameBuffer[index];
						curColor *= 0.5; curColor += newColor * 0.5;
						renderContext->MASSFrameBuffer[index] = curColor.ToUint32();
						color += curColor * 0.25;
					}
					else
					{
						renderContext->MASSFrameBuffer[index] = newColor.ToUint32();
						color += newColor * 0.25;

					}
					renderContext->MASSDepthBuffer[index] = depth;
				}
				else
				{
					Color curColor = renderContext->MASSFrameBuffer[index];
					color += curColor * 0.25;
				}
				coverNum++;
			}
		}
	}

	int index = posX + posY * renderContext->width;
	float samplingColorRatio = (float)coverNum / (sampingNum * sampingNum);

	if (samplingColorRatio > 0 && samplingColorRatio < 1)
	{
		SetPixel(color, posX, posY);
	}
	else
	{
		Color curColor = renderContext->frameBuffer[index];
		color = newColor * samplingColorRatio;
		color += curColor * (1.0f - samplingColorRatio);
		SetPixel(color, posX, posY);
	}
}

bool Rasterizer::insideTriangle(float x, float y, Vector3f* _v)
{
	Vector3f point = Vector3f(x, y, 0);
	Vector3f vec_0_1 = _v[1] - _v[0];
	Vector3f vec_1_2 = _v[2] - _v[1];
	Vector3f vec_2_0 = _v[0] - _v[2];

	Vector3f vec_0_p = point - _v[0];
	Vector3f vec_1_p = point - _v[1];
	Vector3f vec_2_p = point - _v[2];


	if (Cross(vec_0_1, vec_0_p).z <= 0 &&
		Cross(vec_1_2, vec_1_p).z <= 0 &&
		Cross(vec_2_0, vec_2_p).z <= 0)
	{

		return true;
	}

	return false;
}

Color Rasterizer::GetColorByFragment(Triangle &triangle, const Vector3f bcCoord)
{
	Vector3f colors[3];
	std::vector<Texture> textures;


	for (int i = 0; i < 3; i++)
	{
		colors[i] = triangle.color[i].ToVec3();
	}

	if (triangle.texturesID >= 0)
	{
		textures = renderContext->allTextures[triangle.texturesID];
	}
	Vector3f interpolated_texcoords = interpolate(bcCoord, triangle.texCoords, 1.0);
	Vector3f interpolatedNormal = interpolate(bcCoord, triangle.normal, 1.0);
	Vector3f interpolatedViewCoords = interpolate(bcCoord, triangle.viewCoords, 1.0);
	Vector3f interpolatedColor = interpolate(bcCoord, colors, 1.0);
	Color color = Color::black;
	FragmentShader fragShader;

	if (renderContext->shader != NULL && renderContext->shader->fragmentShader != NULL)
	{
		fragShader.color = Color(interpolatedColor.x, interpolatedColor.y, interpolatedColor.z, 1.0);
		fragShader.normal = interpolatedNormal;
		fragShader.texCoords = interpolated_texcoords;
		fragShader.textures = textures;
		fragShader.viewPos = interpolatedViewCoords;
		fragShader.eyePos = renderContext->camera.GetCameraPosition();
	}

	Vector3f piexlColor = renderContext->shader->fragmentShader(fragShader);
	color = Color(piexlColor.x, piexlColor.y, piexlColor.z, 1.0);
	return color;
}