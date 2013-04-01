//
// BaseType.h
// ----------
// Base types for libraries (e.g. Color, Vector, etc).
//
// Copyright (C) 2013, skies457.
//

#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#include <cmath>

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

	inline float getRed() const { return r; }
	inline float getGreen() const { return g; }
	inline float getBlue() const { return b; }
	inline float getAlpha() const { return a; }

	inline BYTE getRedByte() const { return FLOAT_TO_BYTE(r); }
	inline BYTE getGreenByte() const { return FLOAT_TO_BYTE(g); }
	inline BYTE getBlueByte() const { return FLOAT_TO_BYTE(b); }
	inline BYTE getAlphaByte() const { return FLOAT_TO_BYTE(a); }

	inline ULONG getARGB() const { return COMPONENT_TO_ARGB(r, g, b, a); }
	inline ULONG getRGB() const { return COMPONENT_TO_RGB(r, g, b); }

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
	Vector()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};
	Vector(float _x, float _y, float _z, float _w = 1.0f)
		: x(_x), y(_y), z(_z), w(_w) {};

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

	Vector& operator*(const Vector &v) {
		float 	xt = y * v.z - z * v.y,
				yt = z * v.x - x * v.z,
				zt = x * v.y - y * v.x;
		x = xt, y = yt, z = zt;
		return *this;
	}

	const Vector operator*(float k) const {
		return Vector(x * k, y * k, z * k);
	}

	Vector& operator*(float k) {
		x *= k, y *= k, z *= k;
		return *this;
	}

	friend const Vector operator*(float k, const Vector &v) const {
		return Vector(v.x * k, v.y * k, v.z * k);
	}
};

#endif // __BASETYPE_H__
