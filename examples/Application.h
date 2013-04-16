//
// Application.cpp
// ---------------
// This is a simple example using GlutApp wrapper. Main program primarily
// declared in class Application, as well as a listener for GLUT events.
//
// Copyright (c) 2013, skies457.
//

#include "../GlutApp.h"
#include <iostream>
#include <memory>
using namespace std;

class Application : public GlutListener
{
private:
	GlutApp::Ptr core;

public:
	Application();
	~Application();

	// Rendering call-back function.
	void onRender(unsigned int timeElapsed) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glRectf(-10.0f, 10.0f, 10.0f, -10.0f);
	}
	void run() { core->run(); }

	const char *name() const {
		return "Minimal GLUT Application";
	}
};

Application::Application()
{
	// Construct a GlutApp instance using default parameters.
	core = GlutApp::construct(GlutStartupParams::construct(this));
}

Application::~Application()
{
}

