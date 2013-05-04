//
// BaseType.h
// ----------
// Base types for libraries (e.g. Color, Vector, etc).
//
// Copyright (C) 2013, skies457.
//

#ifndef __BASETYPE_H__
#define __BASETYPE_H__

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

#ifndef PI
#define PI	3.1415926535897932
#endif // PI

#ifdef BYTE
#undef BYTE
#endif // BYTE
#ifdef ULONG
#undef ULONG
#endif // ULONG

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
	static Color Black;
};

#define FLOAT_EPSILON		1e-6
#define FLOAT_EQUAL(f1, f2)	(fabs((f1) - (f2)) < FLOAT_EPSILON)
#define FLOAT_ZERO(f)		(FLOAT_EQUAL(f, 0))

class Vector
{
public:
	union {
		float _data[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
	};

public:
	enum VectorType
	{
		VectorType_Direction,
		VectorType_Position
	};
public:
	Vector(float _x, float _y, float _z, float _w = 1.0f)
		: x(_x), y(_y), z(_z), w(_w) {}

	inline void set(float _x, float _y, float _z, float _w = 1.0f) {
		x = _x, y = _y, z = _z, w = _w;
	}

	inline float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	inline void normalize() {
		float k = length();
		x /= k, y /= k, z /= k;
	}

	bool operator==(const Vector &v) const {
		return (FLOAT_EQUAL(x, v.x) &&
				FLOAT_EQUAL(y, v.y) &&
				FLOAT_EQUAL(z, v.z));
	}

	bool operator!=(const Vector &v) const {
		return (!FLOAT_EQUAL(x, v.x) ||
				!FLOAT_EQUAL(y, v.y) ||
				!FLOAT_EQUAL(z, v.z));
	}

	Vector &operator=(const Vector &v) {
		x = v.x, y = v.y, z = v.z, w = v.w;
		return *this;
	}

	const Vector operator+(const Vector &v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector& operator+=(const Vector &v) {
		x += v.x, y += v.y, z += v.z;
		return *this;
	}

	const Vector operator-(const Vector &v) const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector& operator-=(const Vector &v) {
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}

	const Vector operator*(const Vector &v) const {
		return Vector(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
	}

	Vector& operator*=(const Vector &v) {
		float 	xt = y * v.z - z * v.y,
				yt = z * v.x - x * v.z,
				zt = x * v.y - y * v.x;
		x = xt, y = yt, z = zt;
		return *this;
	}

	Vector& operator*=(float k) {
		x *= k, y *= k, z *= k;
		return *this;
	}

	Vector& operator*=(const class Matrix &m);

	friend const Vector operator*(float k, const Vector &v) {
		return Vector(v.x * k, v.y * k, v.z * k);
	}
};

class Matrix
{
private:
	float _data[4][4];

public:
	Matrix() { memset(_data, 0, sizeof(_data)); }
	Matrix(	float a1, float a2, float a3, float a4,
			float b1, float b2, float b3, float b4,
			float c1, float c2, float c3, float c4,
			float d1, float d2, float d3, float d4)
	{
		_data[0][0] = a1, _data[0][1] = a2, _data[0][2] = a3, _data[0][3] = a4;
		_data[1][0] = b1, _data[1][1] = b2, _data[1][2] = b3, _data[1][3] = b4;
		_data[2][0] = c1, _data[2][1] = c2, _data[2][2] = c3, _data[2][3] = c4;
		_data[3][0] = d1, _data[3][1] = d2, _data[3][2] = d3, _data[3][3] = d4;
	}

	void inverse();
	void inverse(const Matrix &m);

	inline float *raw() const {
		return (float *)_data;
	}

	inline void transpose() {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j <= i; j++) {
				std::swap(_data[i][j], _data[j][i]);
			}
		}
	}

	inline void transpose(const Matrix &m) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				_data[i][j] = m(j, i);
			}
		}
	}

	float &operator()(unsigned int i, unsigned int j) {
		if(i >= 4 || j >= 4) {
			std::cerr << "Matrix subscript (" << i << "," << j <<
					") out of range, return (0,0) as default." << std::endl;
			return _data[0][0];
		}
		return _data[i][j];
	}

	const float operator()(unsigned int i, unsigned int j) const {
		if(i >= 4 || j >= 4) {
			std::cerr << "Matrix subscript (" << i << "," << j <<
					") out of range, return (0,0) as default." << std::endl;
			return _data[0][0];
		}
		return _data[i][j];
	}

	bool operator==(const Matrix &m) const {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(!FLOAT_EQUAL(_data[i][j], m._data[i][j]))
					return false;
			}
		}
		return true;
	}
	bool operator!=(const Matrix &m) const {
		return !(*this == m);
	}

	Matrix &operator=(const Matrix &m) {
		if(this == &m) return *this;
		memcpy(_data, m._data, sizeof(_data));
		return *this;
	}

	const Matrix operator+(const Matrix &m) const {
		Matrix rm;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				rm(i, j) = _data[i][j] + m(i, j);
			}
		}
		return rm;
	}

	Matrix &operator+=(const Matrix &m) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				_data[i][j] += m(i, j);
			}
		}
		return *this;
	}

	const Matrix operator*(float f) const {
		Matrix rm;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				rm(i, j) = _data[i][j] * f;
			}
		}
		return rm;
	}

	Matrix &operator*=(float f) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				_data[i][j] *= f;
			}
		}
		return *this;
	}

	const Matrix operator*(const Matrix &m) const {
		Matrix rm;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 4; k++) {
					rm(i, j) += _data[i][k] * m(k, j);
				}
			}
		}
		return rm;
	}

	Matrix &operator*=(const Matrix &m) {
		Matrix rm = *this;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				_data[i][j] = 0;
				for(int k = 0; k < 4; k++) {
					_data[i][j] += rm(i, k) * m(k, j);
				}
			}
		}
		return *this;
	}

	friend const Vector operator*(const Vector &v, const Matrix &m) {
		Vector rv(
				m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z + m(0,3) * v.w,
				m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z + m(1,3) * v.w,
				m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z + m(2,3) * v.w,
				m(3,0) * v.x + m(3,1) * v.y + m(3,2) * v.z + m(3,3) * v.w);
		if(!FLOAT_ZERO(rv.w)) {
			rv.x /= rv.w, rv.y /= rv.w, rv.z /= rv.w;
		}
		rv.w = 1.0f;
		return rv;
	}

	static const Matrix buildIdentityMatrix() {
		return Matrix(
				1.0f,	0.0f,	0.0f,	0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f);
	}

	static const Matrix buildScalingMatrix(float s) {
		return Matrix(
				s,		0.0f,	0.0f,	0.0f,
				0.0f,	s,		0.0f,	0.0f,
				0.0f,	0.0f,	s,		0.0f,
				0.0f,	0.0f,	0.0f,	1.0f);
	}

	static const Matrix buildTranslationMatrix(float dx, float dy, float dz) {
		return Matrix(
				1.0f,	0.0f,	0.0f,	dx,
				0.0f,	1.0f,	0.0f,	dy,
				0.0f,	0.0f,	1.0f,	dz,
				0.0f,	0.0f,	0.0f,	1.0f);
	}

	static const Matrix buildRotationMatrixAroundX(float radius) {
		float fc = cosf(radius), fs = sinf(radius);
		return Matrix(
				1.0f,	0.0f, 	0.0f,	0.0f,
				0.0f,	fc, 	fs, 	0.0f,
				0.0f, 	-fs, 	fc, 	0.0f,
				0.0f, 	0.0f,	0.0f, 	1.0f);
	}

	static const Matrix buildRotationMatrixAroundY(float radius) {
		float fc = cosf(radius), fs = sinf(radius);
		return Matrix(
				fc,		0.0f,	-fs,	0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				fs,		0.0f,	fc,		0.0f,
				0.0f,	0.0f,	0.0f,	1.0f);
	}

	static const Matrix buildRotationMatrixAroundZ(float radius) {
		float fc = cosf(radius), fs = sinf(radius);
		return Matrix(
				fc,		fs,		0.0f,	0.0f,
				-fs,	fc,		0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f);
	}

	static const Matrix buildRotationMatrix(const Vector &v, float radius) {
		float fc = cosf(radius), fs = sinf(radius), d = 1.0f - fc;
		float xfs = v.x * fs, yfs = v.y * fs, zfs = v.z * fs;

		return Matrix(
				v.x * v.x * d + fc,		v.x * v.y * d + zfs,	v.x * v.z * d - yfs,	0.0f,
				v.y * v.x * d - zfs,	v.y * v.y * d + fc,		v.y * v.z * d + xfs,	0.0f,
				v.z * v.x * d + yfs,	v.z * v.y * d - xfs,	v.z * v.z * d + fc,		0.0f,
				0.0f,					0.0f,					0.0f,					1.0f);
		/*
				 *	x*x*d+cos(a)	x*y*d-z*sin(a)	x*z*d+y*sin(a)	0
				 *	y*x*d+z*sin(a)	y*y*d+cos(a)	y*z*d-x*sin(a)	0
				 *	z*x*d-y*sin(a)	z*y*d+x*sin(a)	z*z*d+cos(a)	0 */
	}

	void apply() const {
		glMultTransposeMatrixf((const float *)_data);
	}

	void applyExclusively() const {
		glLoadTransposeMatrixf((const float *)_data);
	}
};

#endif // __BASETYPE_H__
