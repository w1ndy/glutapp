//
// Application.h
// -------------
// This is a minimal example class using GlutApp wrapper.
//
// Copyright (c) 2013, skies457.
//

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "../GlutApp.h"
#include "../BaseType.h"
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
		Color::Green.apply();
		glRectf(-30.0f, 30.0f, 30.0f, -30.0f);
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

#endif // __APPLICATION_H__
