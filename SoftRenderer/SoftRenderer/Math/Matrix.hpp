template<typename T>
class Matrix2
{
public:
	static Matrix2<T> Identity;
	Matrix2<T>(T m00, T m10, T m01, T m11) {
		m_[0][0] = m00;
		m_[1][0] = m10;
		m_[0][1] = m01;
		m_[1][1] = m11;
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