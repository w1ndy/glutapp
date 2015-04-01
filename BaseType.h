//
// BaseType.h
// ----------
// Base types for libraries (e.g. Color, Vector, etc).
//
// Copyright (C) 2013, skies457.
//

#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif // __APPLE__

#include <iostream>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <cstring>
#include <cassert>
#include <limits>

#ifndef PI
#define PI	3.1415926535897932
#endif // PI

#ifdef BYTE
#undef BYTE
#endif // BYTE
#ifdef ULONG
#undef ULONG
#endif // ULONG

#define FLOAT_SIGN(f) ((f) > 0 ? 1 : -1)

#define ALPHA_MASK 	0xFF000000
#define RED_MASK	0x00FF0000
#define BLUE_MASK	0x0000FF00
#define GREEN_MASK	0x000000FF

typedef unsigned char BYTE;
typedef unsigned long ULONG;
#define FLOAT_TO_ULONG(f) 	((ULONG)((f) * 255) & 0xFF)
#define FLOAT_TO_BYTE(f) 	(BYTE)(FLOAT_TO_ULONG(f))
#define BYTE_TO_FLOAT(b)	((float)((b) & 0xFF) / 255.0f)

#define _FTOB FLOAT_TO_BYTE
#define _FTOU FLOAT_TO_ULONG
#define _BTOF BYTE_TO_FLOAT
#define COMPONENT_TO_RGB(r, g, b) \
	((_FTOU(r) << 16) + (_FTOU(g) << 8) + _FTOU(b))
#define COMPONENT_TO_ARGB(r, g, b, a) \
	((_FTOU(a) << 24) + (_FTOU(r) << 16) + (_FTOU(g) << 8) + _FTOU(b))

#define BITWISE_MASK_AND_SHIFT_RIGHT(n, m, s) \
	(((n) & (m)) >> (s))
#define _BMASR BITWISE_MASK_AND_SHIFT_RIGHT
#define ARGB_TO_ALPHA_FLOAT(argb) 	_BTOF(_BMASR(argb, ALPHA_MASK, 24))
#define ARGB_TO_RED_FLOAT(argb)		_BTOF(_BMASR(argb,   RED_MASK, 16))
#define ARGB_TO_GREEN_FLOAT(argb)	_BTOF(_BMASR(argb, GREEN_MASK,  8))
#define ARGB_TO_BLUE_FLOAT(argb)  	_BTOF(_BMASR(argb,  BLUE_MASK,  0))

#define RGB_TO_RED_FLOAT 	ARGB_TO_RED_FLOAT
#define RGB_TO_GREEN_FLOAT	ARGB_TO_GREEN_FLOAT
#define RGB_TO_BLUE_FLOAT	ARGB_TO_BLUE_FLOAT

template<typename T>
struct primitive_type_to_gl_enum {};

template<>
struct primitive_type_to_gl_enum<float>
{
	static constexpr int value = GL_FLOAT;
};

template<>
struct primitive_type_to_gl_enum<double>
{
	static constexpr int value = GL_DOUBLE;
};

template<>
struct primitive_type_to_gl_enum<unsigned char>
{
	static constexpr int value = GL_UNSIGNED_BYTE;
};

template<>
struct primitive_type_to_gl_enum<unsigned short>
{
	static constexpr int value = GL_UNSIGNED_SHORT;
};

template<>
struct primitive_type_to_gl_enum<unsigned int>
{
	static constexpr int value = GL_UNSIGNED_INT;
};

template<typename T>
struct floating_point_type_properties {};

template<>
struct floating_point_type_properties<float>
{
	static constexpr float zero = 0.0f;
	static constexpr float one = 1.0f;
	static constexpr float epsilon = 1e-6;
};

template<>
struct floating_point_type_properties<double>
{
	static constexpr double zero = 0.0;
	static constexpr double one = 1.0;
	static constexpr double epsilon = 1e-8;
};

class Color
{
public:
	union {
		float _data[4];
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};

public:
	Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {};
	Color(ULONG rgb, bool withAlphaChannel = false)
		: r(ARGB_TO_RED_FLOAT(rgb)),
		  g(ARGB_TO_GREEN_FLOAT(rgb)),
		  b(ARGB_TO_BLUE_FLOAT(rgb))
		{
			if(withAlphaChannel) a = ARGB_TO_ALPHA_FLOAT(rgb);
			else a = 1.0f;
		};
	Color(float _r, float _g, float _b, float _a = 1.0f)
		: r(_r), g(_g), b(_b), a(_a) {};

	inline void set(float red, float green, float blue, float alpha = 1.0f) {
		r = red, g = green, b = blue, a = alpha;
	}
	inline float getRed() const { return r; }
	inline float getGreen() const { return g; }
	inline float getBlue() const { return b; }
	inline float getAlpha() const { return a; }

	inline BYTE getRedByte() const { return FLOAT_TO_BYTE(r); }
	inline BYTE getGreenByte() const { return FLOAT_TO_BYTE(g); }
	inline BYTE getBlueByte() const { return FLOAT_TO_BYTE(b); }
	inline BYTE getAlphaByte() const { return FLOAT_TO_BYTE(a); }

	inline void setARGB(ULONG argb) {
		r = ARGB_TO_RED_FLOAT(argb);
		g = ARGB_TO_GREEN_FLOAT(argb);
		b = ARGB_TO_BLUE_FLOAT(argb);
		a = ARGB_TO_ALPHA_FLOAT(argb);
	}

	inline void setRGB(ULONG rgb) {
		r = RGB_TO_RED_FLOAT(rgb);
		g = RGB_TO_GREEN_FLOAT(rgb);
		b = RGB_TO_BLUE_FLOAT(rgb);
	}

	inline ULONG getARGB() const { return COMPONENT_TO_ARGB(r, g, b, a); }
	inline ULONG getRGB() const { return COMPONENT_TO_RGB(r, g, b); }
	inline void apply() const { glColor4f(r, g, b, a); }

public:
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color White;
	static Color LightGray;
	static Color Gray;
	static Color DarkGray;
	static Color Black;
};

template<typename T> class Matrix;

template<typename T>
class Vector
{
public:
	union {
		T _data[4];
		struct {
			float x, y, z, w;
		};
	};

private:
	static constexpr T N0 = floating_point_type_properties<T>::zero;
	static constexpr T N1 = floating_point_type_properties<T>::one;

	bool _equal(T _a, T _b)
	{
		return (abs(_a - _b) < floating_point_type_properties<T>::epsilon);
	}

	bool _iszero(T _a) {
		return _equal(_a, N0);
	}

public:
	Vector()
	{
		memset(_data, 0, sizeof(_data));
	}

	Vector(T _x, T _y, T _z, T _w = N0)
		: x(_x), y(_y), z(_z), w(_w) {}

	void set(T _x, T _y, T _z, T _w = N0)
	{
		x = _x, y = _y, z = _z, w = _w;
	}

	T *raw() const {
		return (T *)_data;
	}

	T length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	T dot(Vector<T> const &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	void normalize()
	{
		T k = length();
		if(_iszero(k)) return;
		x /= k, y /= k, z /= k;
	}

	void negate()
	{
		x = -x, y = -y, z = -z;
	}

	bool operator==(Vector<T> const &v) const
	{
		return (_equal(x, v.x) &&
				_equal(y, v.y) &&
				_equal(z, v.z));
	}

	bool operator!=(Vector<T> const &v) const
	{
		return (!_equal(x, v.x) ||
				!_equal(y, v.y) ||
				!_equal(z, v.z));
	}

	Vector<T> &operator=(Vector<T> const &v)
	{
		x = v.x, y = v.y, z = v.z, w = N0;
		return *this;
	}

	Vector<T> operator+(Vector<T> const &v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector<T> &operator+=(Vector<T> const &v)
	{
		x += v.x, y += v.y, z += v.z;
		return *this;
	}

	Vector<T> operator-() const
	{
		return Vector<T>(-x, -y, -z);
	}

	Vector<T> operator-(Vector<T> const &v) const
	{
		return Vector<T>(x - v.x, y - v.y, z - v.z);
	}

	Vector<T> &operator-=(Vector<T> const &v)
	{
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}

	Vector<T> operator*(Vector<T> const &v) const
	{
		return Vector<T>(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
	}

	Vector<T> &operator*=(Vector<T> const &v)
	{
		T 	xt = y * v.z - z * v.y,
			yt = z * v.x - x * v.z,
			zt = x * v.y - y * v.x;
		x = xt, y = yt, z = zt;
		return *this;
	}

	Vector<T> &operator*=(T k)
	{
		x *= k, y *= k, z *= k;
		return *this;
	}

	Vector<T> &operator*=(Matrix<T> const &m)
	{
		*this = *this * m;
		return *this;
	}

	friend Vector<T> const operator*(T k, Vector<T> const &v)
	{
		return Vector<T>(v.x * k, v.y * k, v.z * k);
	}

	friend std::ostream &operator<<(std::ostream &out, Vector<T> const &v)
	{
		out << "(" << v.x << "," << v.y << "," << v.z << ")";
		return out;
	}
};

typedef Vector<float> Vectorf;
typedef Vector<double> Vectord;

template<typename T>
class Point : Vector<T>
{
public:
	Point()
	{
		this->w = Vector<T>::N1;
	}

	Point(T _x, T _y, T _z) :
		Vector<T>(_x, _y, _z, Vector<T>::N1) {}

	void set(T _x, T _y, T _z, T _w = Vector<T>::N1)
	{
		this->x = _x, this->y = _y, this->z = _z, this->w = _w;
	}

	Point<T> &operator=(const Point<T> &p)
	{
		this->x = p.x, this->y = p.y, this->z = p.z, this->w = Vector<T>::N1;
		return *this;
	}
};

template<typename T>
class Matrix
{
private:
	T _data[4][4];

	static constexpr T N0 = floating_point_type_properties<T>::zero;
	static constexpr T N1 = floating_point_type_properties<T>::one;

public:
	Matrix() { memset(_data, 0, sizeof(_data)); }
	Matrix(	T a1, T a2, T a3, T a4,
			T b1, T b2, T b3, T b4,
			T c1, T c2, T c3, T c4,
			T d1, T d2, T d3, T d4)
	{
		_data[0][0] = a1, _data[0][1] = a2, _data[0][2] = a3, _data[0][3] = a4;
		_data[1][0] = b1, _data[1][1] = b2, _data[1][2] = b3, _data[1][3] = b4;
		_data[2][0] = c1, _data[2][1] = c2, _data[2][2] = c3, _data[2][3] = c4;
		_data[3][0] = d1, _data[3][1] = d2, _data[3][2] = d3, _data[3][3] = d4;
	}

	void inverse()
	{
		inverse(*this);
	}

	void inverse(Matrix<T> const &m)
	{
		T factor[6];
		Matrix cof;
		cof.transpose(m);

		factor[0] = cof(2,2) * cof(3,3) - cof(2,3) * cof(3,2);
		factor[1] = cof(2,1) * cof(3,3) - cof(2,3) * cof(3,1);
		factor[2] = cof(2,1) * cof(3,2) - cof(2,2) * cof(3,1);
		factor[3] = cof(2,0) * cof(3,3) - cof(2,3) * cof(3,0);
		factor[4] = cof(2,2) * cof(3,0) - cof(2,0) * cof(3,2);
		factor[5] = cof(2,0) * cof(3,1) - cof(2,1) * cof(3,0);

		_data[0][0]	 = cof(1,1) * factor[0] - cof(1,2) * factor[1] + cof(1,3) * factor[2];
		_data[0][1]  = -cof(1,0) * factor[0] + cof(1,2) * factor[3] + cof(1,3) * factor[4];
		_data[0][2]  = cof(1,0) * factor[1] - cof(1,1) * factor[3] + cof(1,3) * factor[5];
		_data[0][3]  = -cof(1,0) * factor[2] - cof(1,1) * factor[4] - cof(1,2) * factor[5];

		_data[1][0]  = -cof(0,1) * factor[0] + cof(0,2) * factor[1] - cof(0,3) * factor[2];
		_data[1][1]  = cof(0,0) * factor[0] - cof(0,2) * factor[3] - cof(0,3) * factor[4];
		_data[1][2]  = -cof(0,0) * factor[1] + cof(0,1) * factor[3] - cof(0,3) * factor[5];
		_data[1][3]  = cof(0,0) * factor[2] + cof(0,1) * factor[4] + cof(0,2) * factor[5];

		factor[0] = cof(0,2) * cof(1,3) - cof(0,3) * cof(1,2);
		factor[1] = cof(0,1) * cof(1,3) - cof(0,3) * cof(1,1);
		factor[2] = cof(0,1) * cof(1,2) - cof(0,2) * cof(1,1);
		factor[3] = cof(0,0) * cof(1,3) - cof(0,3) * cof(1,0);
		factor[4] = cof(0,2) * cof(1,0) - cof(0,0) * cof(1,2);
		factor[5] = cof(0,0) * cof(1,1) - cof(0,1) * cof(1,0);

		_data[2][0]	 = factor[0] * cof(3,1) - factor[1] * cof(3,2) + factor[2] * cof(3,3);
		_data[2][1]  = -factor[0] * cof(3,0) + factor[3] * cof(3,2) + factor[4] * cof(3,3);
		_data[2][2]  = factor[1] * cof(3,0) - factor[3] * cof(3,1) + factor[5] * cof(3,3);
		_data[2][3]  = -factor[2] * cof(3,0) - factor[4] * cof(3,1) - factor[5] * cof(3,2);

		_data[3][0]  = -factor[0] * cof(2,1) + factor[1] * cof(2,2) - factor[2] * cof(2,3);
		_data[3][1]  = factor[0] * cof(2,0) - factor[3] * cof(2,2) - factor[4] * cof(2,3);
		_data[3][2]  = -factor[1] * cof(2,0) + factor[3] * cof(2,1) - factor[5] * cof(2,3);
		_data[3][3]  = factor[2] * cof(2,0) + factor[4] * cof(2,1) + factor[5] * cof(2,2);

		float inv_det = 1.0f / (
			_data[0][0] * cof(0,0) +
			_data[0][1] * cof(0,1) +
			_data[0][2] * cof(0,2) +
			_data[0][3] * cof(0,3));
		*this *= inv_det;
		return ;
	}

	T *raw() const
	{
		return (T *)_data;
	}

	void transpose()
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < i; j++)
				std::swap(_data[i][j], _data[j][i]);
	}

	void transpose(const Matrix<T> &m)
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				_data[i][j] = m(j, i);
	}

	T &operator()(size_t i, size_t j)
	{
		assert(i < 4 && j < 4);
		return _data[i][j];
	}

	T operator()(size_t i, size_t j) const
	{
		assert(i < 4 && j < 4);
		return _data[i][j];
	}

	bool operator==(Matrix<T> const &m) const
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(!Vector<T>::_equal(_data[i][j], m._data[i][j]))
					return false;

		return true;
	}

	bool operator!=(Matrix<T> const &m) const
	{
		return !(*this == m);
	}

	Matrix<T> &operator=(Matrix<T> const &m)
	{
		if(this == &m)
			return *this;
		memcpy(_data, m._data, sizeof(_data));
		return *this;
	}

	Matrix<T> operator+(Matrix<T> const &m) const
	{
		Matrix<T> rm;

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				rm(i, j) = _data[i][j] + m(i, j);

		return rm;
	}

	Matrix<T> &operator+=(Matrix<T> const &m)
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				_data[i][j] += m(i, j);

		return *this;
	}

	Matrix<T> operator*(T f) const
	{
		Matrix<T> rm;

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				rm(i, j) = _data[i][j] * f;

		return rm;
	}

	Matrix<T> &operator*=(T f)
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				_data[i][j] *= f;

		return *this;
	}

	Matrix<T> operator*(Matrix<T> const &m) const
	{
		Matrix<T> rm;

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				for(int k = 0; k < 4; k++)
					rm(i, j) += _data[i][k] * m(k, j);

		return rm;
	}

	Matrix<T> &operator*=(Matrix<T> const &m)
	{
		Matrix<T> rm = *this;

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++) {
				_data[i][j] = 0;
				for(int k = 0; k < 4; k++)
					_data[i][j] += rm(i, k) * m(k, j);
			}

		return *this;
	}

	friend Vector<T> const operator*(Vector<T> const &v, Matrix<T> const &m)
	{
		Vector<T> rv(
			m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z + m(0,3) * v.w,
			m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z + m(1,3) * v.w,
			m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z + m(2,3) * v.w,
			m(3,0) * v.x + m(3,1) * v.y + m(3,2) * v.z + m(3,3) * v.w);

		return rv;
	}

	static Matrix<T> buildIdentityMatrix()
	{

		return Matrix<T>(
			N1,	N0,	N0,	N0,
			N0,	N1,	N0,	N0,
			N0,	N0,	N1,	N0,
			N0,	N0,	N0,	N1);
	}

	static Matrix<T> buildScalingMatrix(T s)
	{
		return Matrix<T>(
			s,	N0,	N0,	N0,
			N0,	s,	N0,	N0,
			N0,	N0,	s,	N0,
			N0,	N0,	N0,	N1);
	}

	static Matrix<T> buildTranslationMatrix(
		T dx, T dy, T dz)
	{
		return Matrix<T>(
			N1,	N0,	N0,	dx,
			N0,	N1,	N0,	dy,
			N0,	N0,	N1,	dz,
			N0,	N0,	N0,	N1);
	}

	static Matrix<T> buildRotationMatrixAroundX(T radius)
	{
		T fc = cos(radius), fs = sin(radius);
		return Matrix<T>(
			N1,	N0, N0,	N0,
			N0,	fc, fs, N0,
			N0, -fs, fc, N0,
			N0, N0,	N0, N1);
	}

	static Matrix<T> buildRotationMatrixAroundY(T radius)
	{
		T fc = cos(radius), fs = sin(radius);
		return Matrix<T>(
			fc,	N0,	-fs,N0,
			N0,	N1,	N0,	N0,
			fs,	N0,	fc,	N0,
			N0,	N0,	N0,	N1);
	}

	static Matrix<T> buildRotationMatrixAroundZ(T radius)
	{
		T fc = cos(radius), fs = sin(radius);
		return Matrix<T>(
			fc,	fs,	N0,	N0,
			-fs,fc,	N0,	N0,
			N0,	N0,	N1,	N0,
			N0,	N0,	N0,	N1);
	}

	static const Matrix<T> buildRotationMatrix(
		const Vector<T> &v, T radius)
	{
		T fc = cos(radius), fs = sin(radius), d = N1 - fc;
		T xfs = v.x * fs, yfs = v.y * fs, zfs = v.z * fs;

		return Matrix<T>(
			v.x * v.x * d + fc,	v.x * v.y * d + zfs,v.x * v.z * d - yfs,N0,
			v.y * v.x * d - zfs,v.y * v.y * d + fc,	v.y * v.z * d + xfs,N0,
			v.z * v.x * d + yfs,v.z * v.y * d - xfs,v.z * v.z * d + fc,	N0,
			N0,					N0,					N0,				N1);
		/*
				 *	x*x*d+cos(a)	x*y*d-z*sin(a)	x*z*d+y*sin(a)	0
				 *	y*x*d+z*sin(a)	y*y*d+cos(a)	y*z*d-x*sin(a)	0
				 *	z*x*d-y*sin(a)	z*y*d+x*sin(a)	z*z*d+cos(a)	0 */
	}

	void apply() const
	{
		glMultTransposeMatrixf(raw());
	}

	void applyExclusively() const
	{
		glLoadTransposeMatrixf(raw());
	}
};

typedef Matrix<float> Matrixf;
typedef Matrix<double> Matrixd;

#endif // __BASETYPE_H__
