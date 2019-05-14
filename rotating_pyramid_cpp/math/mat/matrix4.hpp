#ifndef _RAD_DEFS
#define _RAD_DEFS

#define PI 3.14f
#define toRadians(x) ((x) * PI / 180.0f)
#define toDegrees(x) ((x) * 180f / PI)

#endif

#ifndef _MATRIX_4
#define _MATRIX_4

struct Matrix4f {
	float m[4][4];

	inline Matrix4f operator*(const Matrix4f&);
};

inline Matrix4f Matrix4f::operator*(const Matrix4f& rhs)
{
	Matrix4f ret;
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			ret.m[i][j] = 	m[i][0] * rhs.m[0][j] +
			     		m[i][1] * rhs.m[1][j] +
			      		m[i][2] * rhs.m[2][j] +
			      		m[i][3] * rhs.m[3][j];
		}

	}
	
	return ret;
}
#endif
