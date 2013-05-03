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

public:
	Perspective() {
		core = GlutApp::construct(GlutStartupParams::construct(this));
	};

	~Perspective() {};

	// View changed event call-back function.
	void onResize(int width, int height) {
		updatePerspectiveView(width, height);
	}

	// Rendering call-back function.
	void onRender(unsigned int timeElapsed) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glLoadMatrixf(Matrix::buildTranslateMatrix(0.0f, 0.0f, -5.0f).raw());
		glutWireCube(1.0);
		//glRectf(-10.0f, 10.0f, 10.0f, -10.0f);
	}

	void run() { core->run(); }

	const char *name() const {
		return "GL Perspective View";
	}
};

#endif // __PERSPECTIVE_H__
