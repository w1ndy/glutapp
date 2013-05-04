//
// Perspective.h
// -------------
// This is an example using perspective projection of GLUT.
//
// Copyright (C) 2013, skies457.
//

#ifndef __PERSPECTIVE_H__
#define __PERSPECTIVE_H__

#include "../GlutApp.h"
#include "../BaseType.h"

class Perspective : public Application
{
private:
	GlutApp::Ptr core;
	GlutCamera camera;

public:
	Perspective() {
		core = GlutApp::construct(GlutStartupParams::construct(this));
		core->installCamera(&camera);
		camera.pitch(PI / 4);
		camera.yaw(PI / 4);
		camera.walk(-30.0f);
	};

	~Perspective() {};

	// View changed event call-back function.
	void onResize(int width, int height) {
		updatePerspectiveView(width, height);
	}

	// Rendering call-back function.
	void onRender(unsigned int timeElapsed) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireTeapot(10.0f);

		// Draw x (red), y (green), z (blue) axis.
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-100.0f, 0.0f, 0.0f);
			glVertex3f(100.0f, 0.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, -100.0f, 0.0f);
			glVertex3f(0.0f, 100.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, -100.0f);
			glVertex3f(0.0f, 0.0f, 100.0f);
		glEnd();
	}

	void run() { core->run(); }

	const char *name() const {
		return "GL Perspective View";
	}
};

#endif // __PERSPECTIVE_H__
