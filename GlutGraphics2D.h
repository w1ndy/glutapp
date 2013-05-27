//
// GlutGraphics2D.h
//

#ifndef __GLUTGRAPHICS2D_H__
#define __GLUTGRAPHICS2D_H__

#include "BaseType.h"
#include "GlutSceneManager2D.h"

class BorderedObject : public SceneObject2D
{
protected:
	Color 	_colorBorder, _colorFill;
	bool 	_bBorder, _bFill;
	int		_borderWidth;

public:
	BorderedObject(bool FillEnabled, Color FillColor,
			bool BorderEnabled = false, Color BorderColor = Color::White, int BorderWidth = 1) {
		_colorBorder = BorderColor;
		_colorFill = FillColor;
		_bBorder = BorderEnabled;
		_bFill = FillEnabled;
		_borderWidth = BorderWidth;
	}

	virtual ~BorderedObject() {}

	void setFill(bool f) {
		_bFill = f;
	}

	bool getFill() const {
		return _bFill;
	}

	void setBorder(bool f) {
		_bBorder = f;
	}

	bool getBorder() const {
		return _bBorder;
	}

	void setFillColor(Color c) {
		_colorFill = c;
	}

	Color getFillColor() const {
		return _colorFill;
	}

	void setBorderColor(Color c) {
		_colorBorder = c;
	}

	Color getBorderColor() const {
		return _colorBorder;
	}

	void setBorderWidth(int width) {
		_borderWidth = width;
	}

	int getBorderWidth() const {
		return _borderWidth;
	}
};

class Rectangle : public BorderedObject
{
private:
	BoundingCircle _bc;
	float _x1, _y1, _x2, _y2;

private:
	void _updateBounds()
	{
		_bc._x = (_x1 + _x2) / 2;
		_bc._y = (_y1 + _y2) / 2;
		_bc._r = sqrt((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2)) / 2;
	}

public:
	Rectangle(int x, int y, int width, int height, Color c = Color::White)
			: BorderedObject(true, c), _x1(x), _y1(y), _x2(x + width), _y2(y + height) {
		_updateBounds();
	}
	~Rectangle() {};

	const Bounds2D &getBounds() const { return _bc; }
	float getX() const { return _bc._x; }
	float getY() const { return _bc._y; }

	void draw(unsigned int timeElapsed) {
		glPushAttrib(GL_CURRENT_BIT);
		if(_bFill) {
			_colorFill.apply();
			glRectf(_x1, _y1, _x2, _y2);
		}
		if(_bBorder) {
			_colorBorder.apply();
			glLineWidth(_borderWidth);
			glBegin(GL_LINE_LOOP);
			glVertex3f(_x1, _y1, 0);
			glVertex3f(_x1, _y2, 0);
			glVertex3f(_x2, _y2, 0);
			glVertex3f(_x2, _y1, 0);
			glEnd();
		}
		glPopAttrib();
	}
};

class Circle : public BorderedObject
{
private:
	BoundingCircle _bc;
	float _x, _y, _r;
	int _smooth;

public:
	Circle(float x, float y, float radius, Color c = Color::White, int smooth = 360)
		:  BorderedObject(true, c), _bc(x, y, radius), _x(x), _y(y), _r(radius), _smooth(smooth) {};
	~Circle() {};

	const Bounds2D &getBounds() const { return _bc; }
	float getX() const { return _bc._x; }
	float getY() const { return _bc._y; }

	void draw(unsigned int timeElapsed) {
		glPushAttrib(GL_CURRENT_BIT);
		if(_bFill) {
			_colorFill.apply();
			glBegin(GL_POLYGON);
			float delta = 2 * 3.1415926535897932 / _smooth;
			for(float i = 0; i <= 2 * 3.1415926535897932; i += delta) {
				glVertex3f(_x + _r * cosf(i), _y + _r * sinf(i), 0);
			}
			glEnd();
		}
		if(_bBorder) {
			_colorBorder.apply();
			glLineWidth(_borderWidth);
			glBegin(GL_LINE_LOOP);
			float delta = 2 * 3.1415926535897932 / _smooth;
			for(float i = 0; i <= 2 * 3.1415926535897932; i += delta) {
				glVertex3f(_x + _r * cosf(i), _y + _r * sinf(i), 0);
			}
			glEnd();
		}
		glPopAttrib();
	}
};

#endif // __GLUTGRAPHICS2D_H__
