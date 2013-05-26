//
// GlutBounds2D.h
//

#ifndef __GLUTBOUNDS2D_H__
#define __GLUTBOUNDS2D_H__

enum Bounds2DType
{
	Bounds2DType_Circle
};

class Bounds2D
{
public:
	virtual Bounds2DType getType() const 		= 0;
	virtual bool isIntersected(const Bounds2D &b) = 0;
};

class BoundingCircle : public Bounds2D
{
public:
	float _x, _y, _r;

public:
	BoundingCircle() : _x(0.0f), _y(0.0f), _r(0.0f) {};
	BoundingCircle(float x, float y, float radius) : _x(x), _y(y), _r(radius) {};
	~BoundingCircle() {};

	inline float getX() const
	{
		return _x;
	}

	inline float getY() const
	{
		return _y;
	}

	inline float getRadius() const
	{
		return _r;
	}

	Bounds2DType getType() const
	{
		return Bounds2DType_Circle;
	}

	bool isIntersected(const Bounds2D &b)
	{
		switch(b.getType())
		{
		case Bounds2DType_Circle:
			return isIntersected(dynamic_cast<const BoundingCircle &>(b));
		}
		return false;
	}

	inline bool isIntersected(const BoundingCircle &c)
	{
		return sqrt((c._x - _x) * (c._x - _x) + (c._y - _y) * (c._y - _y)) < (_r + c._r);
	}
};

#endif // __GLUTBOUNDS2D_H__
