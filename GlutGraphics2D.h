//
// GlutGraphics2D.h
//

#ifndef __GLUTGRAPHICS2D_H__
#define __GLUTGRAPHICS2D_H__

#include "BaseType.h"
#include "GlutSceneManager2D.h"

class Rectangle : public SceneObject2D
{
private:
	BoundingCircle _bc;
	float _x1, _y1, _x2, _y2;
	Color _c;

private:
	void _updateBounds()
	{
		_bc._x = (_x1 + _x2) / 2;
		_bc._y = (_y1 + _y2) / 2;
		_bc._r = sqrt((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2)) / 2;
	}

public:
	Rectangle() : _x1(0.0f), _y1(0.0f), _x2(0.0f), _y2(0.0f) {}
	Rectangle(int x, int y, int width, int height, Color c = Color::White)
		: _x1(x), _y1(y), _x2(x + width), _y2(y + height), _c(c) { _updateBounds(); }
	~Rectangle() {};

	const Bounds2D &getBounds() const { return _bc; }
	float getX() const { return _bc._x; }
	float getY() const { return _bc._y; }
	void draw(unsigned int timeElapsed) { _c.apply(); glRectf(_x1, _y1, _x2, _y2); }

	void setColor(Color c) { _c = c; }
};

#endif // __GLUTGRAPHICS2D_H__
