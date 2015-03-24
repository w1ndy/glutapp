//
// Application.h
// -------------
// This is a minimal example class using GlutApp wrapper.
//
// Copyright (c) 2013, skies457.
//

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "../ExampleApplication.h"

class Application : public ExampleApplication
{
public:
	Application() {};
	virtual ~Application() {};

	// Rendering call-back function.
	virtual void onRender(unsigned int timeElapsed) {
		glDisable(GL_CULL_FACE);
		//Matrix::buildTranslateMatrix(100.0f, 100.0f, 0.0f).apply();
		glLoadIdentity();
		Matrix::buildTranslationMatrix(0, 0, -1.0f).apply();
		//Matrix::buildRotationMatrixAroundX(PI / 6).apply();
		Color::Green.apply();
		glRectf(-30.0f, 30.0f, 30.0f, -30.0f);
	}

	void run() { core->run(); }

	virtual const char *name() const {
		return "Minimal GLUT Application";
	}
};

#endif // __APPLICATION_H__
