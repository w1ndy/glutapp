//
// GlutApp.h
// ---------
// A C++ wrapper for GL and GLUT.
//
// Copyright (c) 2013, skies457.
//

#ifndef __GLUTAPP_H__
#define __GLUTAPP_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif // __APPLE__

#include <memory>
#include <chrono>
#include "BaseType.h"

class GlutListener
{
public:
	virtual ~GlutListener() {};
	virtual void onInit() {};
	virtual void onIdle() {};
	virtual void onResize(int width, int height);
	virtual void onRender(unsigned int timeElapsed) = 0;

protected:
	void updatePerspectiveView(int width, int height,
			double fovy = 90.0);
	void updateOrthoView(int width, int height,
			double left = -1.0, double right = 1.0,
			double bottom = -1.0, double top = 1.0,
			double near = -1.0, double far = 1.0);
};

class DefaultGlutListener : public GlutListener
{
public:
	void onRender(unsigned int timeElapsed) {};
};

class GlutCamera
{
public:
	virtual ~GlutCamera() {};
	virtual void walk(float unit) 		= 0;
	virtual void fly(float unit) 		= 0;
	virtual void strafe(float unit)		= 0;
	virtual void roll(float radius)		= 0;
	virtual void yaw(float radius)		= 0;
	virtual void pitch(float radius)	= 0;
	virtual void update()				= 0;
};

class DefaultGlutCamera : public GlutCamera
{
private:
	Vector _forward, _up, _right, _loc;
	const float UnitScale = 10.0f;

public:
	DefaultGlutCamera();
	~DefaultGlutCamera() {};
	void walk(float unit);
	void fly(float unit);
	void strafe(float unit);
	void roll(float radius);
	void yaw(float radius);
	void pitch(float radius);
	void update();
};

struct GlutStartupParams
{
	GlutListener *Listener;

	const char *Title;
	unsigned int Width, Height;
	unsigned int PosX, PosY;

	Color ClearColor;

	bool isDoubleBuffered;		// GLUT_DOUBLE
	bool hasDetphBuffer;		// GLUT_DEPTH
	bool hasStencilBuffer;		// GLUT_STENCIL
	bool hasAccumBuffer;		// GLUT_ACCUM
	bool isIndexedColorMode;	// GLUT_INDEX

	mutable int argc;
	mutable char **argv;

	static const GlutStartupParams construct(
			GlutListener *listener,					// Event Listener
			const char *title = "GLUT Application",	// Application Title
			unsigned int width = 640,				// Window Width
			unsigned int height = 480,				// Window Height
			unsigned int x = 100, 					// Position X
			unsigned int y = 100,					// Position Y
			Color clearColor = Color::Black,		// Clear Color
			bool doubleBuffer = true,				// Enable Double Buffer
			bool depthBuffer = true,				// Enable Depth Buffer
			bool stencilBuffer = false,				// Enable Stencil Buffer
			bool accumBuffer = false,				// Enable Accumulation Buffer
			bool indexedColor = false,				// Enable Indexed Color Mode
			int argc = 0,							// Additional Parameters Num
			char **argv = NULL						// Additional Parameters
			);
};

class GlutApp
{
public:
	typedef std::shared_ptr<GlutApp> Ptr;

public:
	static Ptr construct(const GlutStartupParams &params);

	~GlutApp();

	inline void run() {
		glutMainLoop();
	}

	inline void setListener(GlutListener *newListener) {
		_listener = newListener;
	}

	inline GlutListener *getListener() const {
		if(_listener == NULL) return &_defaultListener;
		return _listener;
	}

private:
	GlutApp();
	void initialize(const GlutStartupParams &params);
	void dispatchRenderEvent();
	void dispatchReshapeEvent(int w, int h);
	void dispatchIdleEvent();

	inline unsigned int getElapsedTime(
			const std::chrono::high_resolution_clock::time_point &origin) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::high_resolution_clock::now() - origin
				).count();
	}

private:
	static Ptr _appInst;
	static DefaultGlutListener _defaultListener;

	GlutStartupParams 	_params;
	GlutListener*		_listener;

	unsigned 	_frameCount;
	unsigned	_clearFlag;

	std::chrono::system_clock::time_point _timeBegin;

public:
	// Forward hooks.
	static void _render_callback() {
		if(_appInst.get())
			_appInst->dispatchRenderEvent();
	}

	static void _reshape_callback(int w, int h) {
		if(_appInst.get())
			_appInst->dispatchReshapeEvent(w, h);
	}

	static void _idle_callback() {
		if(_appInst.get())
			_appInst->dispatchIdleEvent();
	}
};

#endif // __GLUTAPP_H__

