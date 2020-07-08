#pragma once
#include <functional>
#include "Base/Vector.hpp"
#include "Base/Texture.h"
#include "Base/Color.h"
#include "Light/PointLight.hpp"

struct FragmentShader
{
	FragmentShader() {}
	FragmentShader(const Color& col, const Vector3f& nor, const Vector2f& tc, std::vector<Texture> tex) :
		color(col), normal(nor), texCoords(tc), textures(tex) {}


	Vector3f viewPos;
	Vector3f eyePos;
	Color color;
	Vector3f normal;
	Vector2f texCoords;
	std::vector<Texture> textures;
};

struct VertexShader
{
	Vector3f position;
};

class Shader
{
public:
	Shader() {}
	Shader(std::function<Vector3f(FragmentShader)> _fragmentShader, std::function<Vector3f(VertexShader)> _vertexShader)
		:fragmentShader(_fragmentShader), vertexShader(_vertexShader) {}
	~Shader() {};

	void SetVertexShader(std::function<Vector3f(VertexShader)> _vertexShader) {
		vertexShader = _vertexShader;
	}
	void SetFragmentShader(std::function<Vector3f(FragmentShader)> _fragmentShader) {
		fragmentShader = _fragmentShader;
	}

	std::function<Vector3f(FragmentShader)> fragmentShader;
	std::function<Vector3f(VertexShader)> vertexShader;
};


class DefaultShader : public Shader
{
public:
	DefaultShader()
	{
		fragmentShader = TextureFragShader;
		vertexShader = VertShader;
	}

	static Vector3f VertShader(VertexShader inputData)
	{
		return inputData.position;
	}

	static Vector3f FragShader(FragmentShader inputData)
	{
		return inputData.color.ToVec3();
	}

	static Vector3f TextureFragShader(FragmentShader inputData)
	{
		//std::cout << "is come" << std::endl;
		Color textureColor = inputData.textures[0].Sample(inputData.texCoords.x, inputData.texCoords.y);

		//Color textureColor = Color::black;
		//for (int k = 0; k < inputData.textures.size(); k++)
		//{
		//	textureColor += inputData.textures[k].Sample(inputData.texCoords.x, inputData.texCoords.y);
		//}
		//
		//return textureColor.ToVec3();

		Vector3f viewPos = inputData.viewPos;
		Vector3f eyePos = inputData.eyePos;

		Vector3f ka(0.005, 0.005, 0.005);
		Vector3f kd = textureColor.ToVec3();
		Vector3f ks(0.005, 0.005, 0.005);

		if (inputData.textures.size() > 1)
		{
			ks = inputData.textures[1].Sample(inputData.texCoords.x, inputData.texCoords.y).ToVec3();
		}

		Vector3f ambientLightIntensity(10, 10, 10);
		PointLight pointLight(Color::red, Vector3f(0, 0, -50));
		pointLight.SetAttenuationPara(1.0f, 0.09f, 0.032f);

		//float attenuation = pointLight.GetAttenuation(viewPos);
		Vector3f attenuation(2000, 2000, 2000);
		float p = 150;
		Vector3f lightDir = pointLight.position - viewPos;
		Vector3f eyeDir = eyePos - pointLight.position;
		Vector3f halfL2E = (eyeDir + lightDir).Normalized();

		Vector3f lightDirNormal = (pointLight.position - viewPos).Normalized();
		float r2 = pow(lightDir.x, 2.0) + pow(lightDir.y, 2.0) + pow(lightDir.z, 2.0);
		attenuation /= r2;

		Vector3f ambient = ka * ambientLightIntensity;
		Vector3f diffuse = kd * attenuation * Mathf::Max(0.0, Dot(lightDirNormal, inputData.normal.Normalized()));
		Vector3f specular = ks * attenuation * pow(Mathf::Max(0.0f, Dot(halfL2E, inputData.normal.Normalized())), p);

		return ambient + diffuse + specular;
	}

};



