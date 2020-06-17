#include "Mathf.h"

template<typename T>
class Matrix2
{
public:
	static Matrix2<T> Identity;
	Matrix2<T>(T m00, T m10, T m01, T m11) {
		m[0][0] = m00;
		m[1][0] = m10;
		m[0][1] = m01;
		m[1][1] = m11;
	}
	~Matrix2<T>() {};

	Matrix2<T> rotate2(const double)
	{
	}
	Matrix2<T> shear2X(const double)
	{
	}
	Matrix2<T> shear2Y(const double)
	{
	}

private:
	T m[2][2];
};

typedef Matrix2<int> Matrix2i;
typedef Matrix2<float> Matrix2f;
typedef Matrix2<double> Matrix2d;

Matrix2d Matrix2d::Identity(
	1, 0,
	0, 1);

template<typename T>
class Matrix4
{
public:
	static Matrix4<T> Identity;
	Matrix4<T>(Vector4d row1, Vector4d row2, Vector4d row3, Vector4d row4)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m[i][j] = row1[j];
			}
		}
	}
	Matrix4<T>(T m00, T m10, T m20, T m30,
		T m01, T m11, T m21, T m31,
		T m02, T m12, T m22, T m32,
		T m03, T m13, T m23, T m33)
	{
		m[0][0] = m00, m[1][0] = m10, m[2][0] = m20, m[3][0] = m30,
			m[0][1] = m01; m[1][1] = m11, m[2][1] = m21, m[3][1] = m31,
			m[0][2] = m02; m[1][2] = m12, m[2][2] = m22, m[3][2] = m32,
			m[0][3] = m03; m[1][3] = m13, m[2][3] = m23, m[3][3] = m33;
	}
	Matrix4<T>(const Matrix4<T>& mat)
	{
		memcpy(m, mat.m, sizeof(T) * 16);
	}

	~Matrix4<T>() {};

	Matrix4<T>& operator=(const Matrix4<T>& mat)
	{
		memcpy(m, mat.m, sizeof(T) * 16);
		return *this;
	}

	bool operator==(const Matrix4<T>& mat)const
	{
		return memcmp(m, mat.m, sizeof(T) * 16);
	}

	bool operator!=(const Matrix4<T>& mat)const
	{
		return !operator==(mat);
	}

	Matrix4<T> operator*(const Matrix4<T> mat)
	{
		Matrix4<T> res;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				res.m[i][j] = 0;
				for (int k = 0; k < 4; k++)
				{
					res.m[i][j] += res.m[i][k] * mat.m[k][j];
				}
			}
		}
		return res;
	}

	Matrix4<T>& operator*=(const Matrix4<T>& mat)
	{
		return *this = operator*(mat);
	}

	//用余子式、代数余子式和伴随来求逆矩阵
	//https://www.shuxuele.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
	Matrix4<T> inverse(const Matrix4<T> &mat)
	{
		double inv[16];

		inv[0] = mat._m[5] * mat._m[10] * mat._m[15] -
			mat._m[5] * mat._m[11] * mat._m[14] -
			mat._m[9] * mat._m[6] * mat._m[15] +
			mat._m[9] * mat._m[7] * mat._m[14] +
			mat._m[13] * mat._m[6] * mat._m[11] -
			mat._m[13] * mat._m[7] * mat._m[10];

		inv[4] = -mat._m[4] * mat._m[10] * mat._m[15] +
			mat._m[4] * mat._m[11] * mat._m[14] +
			mat._m[8] * mat._m[6] * mat._m[15] -
			mat._m[8] * mat._m[7] * mat._m[14] -
			mat._m[12] * mat._m[6] * mat._m[11] +
			mat._m[12] * mat._m[7] * mat._m[10];

		inv[8] = mat._m[4] * mat._m[9] * mat._m[15] -
			mat._m[4] * mat._m[11] * mat._m[13] -
			mat._m[8] * mat._m[5] * mat._m[15] +
			mat._m[8] * mat._m[7] * mat._m[13] +
			mat._m[12] * mat._m[5] * mat._m[11] -
			mat._m[12] * mat._m[7] * mat._m[9];

		inv[12] = -mat._m[4] * mat._m[9] * mat._m[14] +
			mat._m[4] * mat._m[10] * mat._m[13] +
			mat._m[8] * mat._m[5] * mat._m[14] -
			mat._m[8] * mat._m[6] * mat._m[13] -
			mat._m[12] * mat._m[5] * mat._m[10] +
			mat._m[12] * mat._m[6] * mat._m[9];

		inv[1] = -mat._m[1] * mat._m[10] * mat._m[15] +
			mat._m[1] * mat._m[11] * mat._m[14] +
			mat._m[9] * mat._m[2] * mat._m[15] -
			mat._m[9] * mat._m[3] * mat._m[14] -
			mat._m[13] * mat._m[2] * mat._m[11] +
			mat._m[13] * mat._m[3] * mat._m[10];

		inv[5] = mat._m[0] * mat._m[10] * mat._m[15] -
			mat._m[0] * mat._m[11] * mat._m[14] -
			mat._m[8] * mat._m[2] * mat._m[15] +
			mat._m[8] * mat._m[3] * mat._m[14] +
			mat._m[12] * mat._m[2] * mat._m[11] -
			mat._m[12] * mat._m[3] * mat._m[10];

		inv[9] = -mat._m[0] * mat._m[9] * mat._m[15] +
			mat._m[0] * mat._m[11] * mat._m[13] +
			mat._m[8] * mat._m[1] * mat._m[15] -
			mat._m[8] * mat._m[3] * mat._m[13] -
			mat._m[12] * mat._m[1] * mat._m[11] +
			mat._m[12] * mat._m[3] * mat._m[9];

		inv[13] = mat._m[0] * mat._m[9] * mat._m[14] -
			mat._m[0] * mat._m[10] * mat._m[13] -
			mat._m[8] * mat._m[1] * mat._m[14] +
			mat._m[8] * mat._m[2] * mat._m[13] +
			mat._m[12] * mat._m[1] * mat._m[10] -
			mat._m[12] * mat._m[2] * mat._m[9];

		inv[2] = mat._m[1] * mat._m[6] * mat._m[15] -
			mat._m[1] * mat._m[7] * mat._m[14] -
			mat._m[5] * mat._m[2] * mat._m[15] +
			mat._m[5] * mat._m[3] * mat._m[14] +
			mat._m[13] * mat._m[2] * mat._m[7] -
			mat._m[13] * mat._m[3] * mat._m[6];

		inv[6] = -mat._m[0] * mat._m[6] * mat._m[15] +
			mat._m[0] * mat._m[7] * mat._m[14] +
			mat._m[4] * mat._m[2] * mat._m[15] -
			mat._m[4] * mat._m[3] * mat._m[14] -
			mat._m[12] * mat._m[2] * mat._m[7] +
			mat._m[12] * mat._m[3] * mat._m[6];

		inv[10] = mat._m[0] * mat._m[5] * mat._m[15] -
			mat._m[0] * mat._m[7] * mat._m[13] -
			mat._m[4] * mat._m[1] * mat._m[15] +
			mat._m[4] * mat._m[3] * mat._m[13] +
			mat._m[12] * mat._m[1] * mat._m[7] -
			mat._m[12] * mat._m[3] * mat._m[5];

		inv[14] = -mat._m[0] * mat._m[5] * mat._m[14] +
			mat._m[0] * mat._m[6] * mat._m[13] +
			mat._m[4] * mat._m[1] * mat._m[14] -
			mat._m[4] * mat._m[2] * mat._m[13] -
			mat._m[12] * mat._m[1] * mat._m[6] +
			mat._m[12] * mat._m[2] * mat._m[5];

		inv[3] = -mat._m[1] * mat._m[6] * mat._m[11] +
			mat._m[1] * mat._m[7] * mat._m[10] +
			mat._m[5] * mat._m[2] * mat._m[11] -
			mat._m[5] * mat._m[3] * mat._m[10] -
			mat._m[9] * mat._m[2] * mat._m[7] +
			mat._m[9] * mat._m[3] * mat._m[6];

		inv[7] = mat._m[0] * mat._m[6] * mat._m[11] -
			mat._m[0] * mat._m[7] * mat._m[10] -
			mat._m[4] * mat._m[2] * mat._m[11] +
			mat._m[4] * mat._m[3] * mat._m[10] +
			mat._m[8] * mat._m[2] * mat._m[7] -
			mat._m[8] * mat._m[3] * mat._m[6];

		inv[11] = -mat._m[0] * mat._m[5] * mat._m[11] +
			mat._m[0] * mat._m[7] * mat._m[9] +
			mat._m[4] * mat._m[1] * mat._m[11] -
			mat._m[4] * mat._m[3] * mat._m[9] -
			mat._m[8] * mat._m[1] * mat._m[7] +
			mat._m[8] * mat._m[3] * mat._m[5];

		inv[15] = mat._m[0] * mat._m[5] * mat._m[10] -
			mat._m[0] * mat._m[6] * mat._m[9] -
			mat._m[4] * mat._m[1] * mat._m[10] +
			mat._m[4] * mat._m[2] * mat._m[9] +
			mat._m[8] * mat._m[1] * mat._m[6] -
			mat._m[8] * mat._m[2] * mat._m[5];

		double det = mat._m[0] * inv[0] + mat._m[1] * inv[4] + mat._m[2] * inv[8] + mat._m[3] * inv[12];

		if (det == 0) { return Matrix::Identity; }

		Matrix4<T> ret;
		det = 1.0 / det;
		for (int i = 0; i < 16; ++i)
			ret._m[i] = inv[i] * det;
		return ret;
	}

	Matrix4<T> RotateX(const double angle)
	{
		double rad = angle * Mathf::Deg2Rad;
		double cos = Mathf::Cos(rad);
		double sin = Mathf::Sin(rad);

		return Matrix4<T>(
			1,   0,    0, 0,
			0, cos, -sin, 0,
			0, sin,  cos, 0,
			0,   0,    0, 1);
	}

	Matrix4<T> RotateY(const double angle)
	{
		double rad = angle * Mathf::Deg2Rad;
		double cos = Mathf::Cos(rad);
		double sin = Mathf::Sin(rad);

		return Matrix4<T>(
			cos, 0, sin, 0,
			0,   1,   0, 0,
			-sin, 0, cos, 0,
			0,   0,   0, 1);
	}

	Matrix4<T> RotateZ(const double angle)
	{
		double rad = angle * Mathf::Deg2Rad;
		double cos = Mathf::Cos(rad);
		double sin = Mathf::Sin(rad);

		return Matrix4<T>(
			cos, -sin, 0, 0,
			sin,  cos, 0, 0,
			0,      0, 1, 0,
			0,      0, 0, 1);
	}

	Matrix4<T> Transform(const Vector3<T> &v)
	{
		return Matrix4<T>(
			1, 0, 0, v.x,
			0, 1, 0, v.y,
			0, 0, 1, v.z,
			0, 0, 0, 1);
	} 

	Matrix4<T> scale(const double x, const double y, const double z)
	{
		return Matrix4<T>(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}
	Matrix4<T> perspective(const double, const double, const double)
	{

	}

private:
	
	union {
		T m[4][4];
		T _m[16];
	};
};

typedef Matrix4<double> Matrix4d;

Matrix4d Matrix4d::Identity(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);