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

#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>

#include "BaseType.h"
#include "GlutCamera.h"
#include "GlutListener.h"

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
			char **argv = NULL);					// Additional Parameters
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

	inline void installListener(GlutListener *l) {
		_listener.push_back(l);
	}

	inline bool uninstallListener(GlutListener *l) {
		return _removeElementFromContainer(_listener, l,
				"Listener to be uninstalled does not exist.");
	}

	inline void uninstallAllListener() {
		_listener.clear();
	}

	inline void installCamera(GlutCamera *c, const std::string &name = "DefaultCamera") {
		_camera[name] = c;
		if(!_currentCam) _currentCam = c;
	}

	inline bool uninstallCamera(const std::string &name) {
		auto i = _camera.find(name);
		if(i == _camera.end()) {
			std::cerr << "Camera to be uninstalled does not exist." << std::endl;
			return false;
		} else {
			if(i->second == _currentCam) {
				i = _camera.erase(i);
				if(i == _camera.end()) {
					_currentCam = NULL;
				} else {
					_currentCam = i->second;
				}
			} else {
				_camera.erase(i);
			}
		}
		return true;
	}

	inline void uninstallAllCamera() {
		_camera.clear();
		_currentCam = NULL;
	}

	inline GlutCamera *getCameraByName(const std::string &name) {
		auto i = _camera.find(name);
		if(i == _camera.end()) {
			return NULL;
		} else {
			return i->second;
		}
	}

	inline GlutCamera *getCurrentCamera() {
		return _currentCam;
	}

	inline bool switchCamera(const std::string &name) {
		auto i = _camera.find(name);
		if(i == _camera.end()) {
			return false;
		} else {
			_currentCam = i->second;
		}
		return true;
	}

	inline bool switchToFirstCamera() {
		if(_camera.empty()) return false;
		_currentCam = _camera.begin()->second;
		return true;
	}

private:
	GlutApp();

	void _initialize(const GlutStartupParams &params);
	void _dispatchInitEvent();
	void _dispatchRenderEvent();
	void _dispatchReshapeEvent(int w, int h);
	void _dispatchIdleEvent();
	void _dispatchMouseEvent(int button, int state, int x, int y);
	void _dispatchMouseMoveEvent(int x, int y);
	//void _dispatchCameraChangedEvent();

	template<typename T>
	inline bool _removeElementFromContainer(std::vector<T> &container, T &elem, const char *errMsg) {
		auto i = find(container.begin(), container.end(), elem);
		if(i == container.end()) {
			std::cerr << errMsg << std::endl;
			return false;
		} else {
			container.erase(i);
		}
		return true;
	}

	inline unsigned int _getElapsedTime(const std::chrono::high_resolution_clock::time_point &origin) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::high_resolution_clock::now() - origin
				).count();
	}

private:
	static Ptr					_appInst;
	static DefaultGlutListener 	_defaultListener;

	GlutStartupParams 					_params;
	std::vector<GlutListener*>			_listener;
	std::map<std::string, GlutCamera*>	_camera;

	GlutCamera*	_currentCam;
	unsigned 	_frameCount;
	unsigned	_clearFlag;

	std::chrono::system_clock::time_point _timeBegin;

public:
	// Forward hooks.
	static void _render_callback() {
		if(_appInst.get())
			_appInst->_dispatchRenderEvent();
	}

	static void _reshape_callback(int w, int h) {
		if(_appInst.get())
			_appInst->_dispatchReshapeEvent(w, h);
	}

	static void _idle_callback() {
		if(_appInst.get())
			_appInst->_dispatchIdleEvent();
	}

	static void _mouse_callback(int button, int state, int x, int y) {
		if(_appInst.get())
			_appInst->_dispatchMouseEvent(button, state, x, y);
	}

	static void _mousemove_callback(int x, int y) {
		if(_appInst.get())
			_appInst->_dispatchMouseMoveEvent(x, y);
	}
};

#endif // __GLUTAPP_H__

