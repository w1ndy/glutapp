//
// Perspective.h
// -------------
// This is an example using perspective projection of GLUT.
//
// Copyright (C) 2013, skies457.
//

#ifndef __PERSPECTIVE_H__
#define __PERSPECTIVE_H__

#include <GlutUtils.h>
#include "../ExampleApplication.h"

class Perspective : public ExampleApplication
{
private:
	GlutCamera camera;

public:
	Perspective() {};

	virtual ~Perspective() {};

	// View changed event call-back function.
	void onResize(int width, int height) {
		updatePerspectiveView(width, height);
	}

	// Rendering call-back function.
	virtual void onRender(unsigned int timeElapsed) {
		camera.reset();
		camera.yaw((float)timeElapsed / 1000.0f);
		camera.pitch(asinf(1.0f/sqrt(3)));
		camera.walk(-60.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireTeapot(10.0f);

		drawAxis(100.0f);
	}

	int run() {
		core->installCamera(&camera);
		camera.yaw(PI / 4);
		camera.pitch(asinf(1.0f/sqrt(3)));
		camera.walk(-60.0f);
		return core->run();
	}

	virtual const char *name() const {
		return "GL Perspective View";
	}
};

#endif // __PERSPECTIVE_H__
