#include "GlutApp.h"
#include <cstdio>
#include <cstring>

std::shared_ptr<GlutApp> GlutApp::_appInst;
DefaultGlutListener GlutApp::_defaultListener;

void GlutListener::onResize(int width, int height)
{
	updateOrthoView(width, height,
			-(double)width / 2, (double)width / 2,
			-(double)height / 2, (double)height / 2);
}

void GlutListener::updatePerspectiveView(int width, int height, double fovy)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)width / height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void GlutListener::updateOrthoView(int width, int height,
		double left, double right, double bottom, double top,
		double near, double far)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
}

const GlutStartupParams GlutStartupParams::construct(GlutListener *listener,
		const char *title, unsigned int width, unsigned int height,
		unsigned int x, unsigned int y, Color clearColor,
		bool doubleBuffer, bool depthBuffer, bool stencilBuffer,
		bool accumBuffer, bool indexedColor,
		int argc, char **argv)
{
	GlutStartupParams p;
	static char *null_argv[1] = { NULL };

	p.Listener = listener;
	p.Title = title;
	p.Width = width;
	p.Height = height;
	p.PosX = x;
	p.PosY = y;
	p.ClearColor = clearColor;
	p.isDoubleBuffered = doubleBuffer;
	p.hasDetphBuffer = depthBuffer;
	p.hasStencilBuffer = stencilBuffer;
	p.hasAccumBuffer = accumBuffer;
	p.isIndexedColorMode = indexedColor;

	p.argc = argc;
	p.argv = (argv == NULL) ? null_argv : argv;

	return p;
}

DefaultGlutCamera::DefaultGlutCamera()
	: _forward(0.0f, 0.0f, -1.0f, 0.0f),
	  _up(0.0f, 1.0f, 0.0f, 0.0f),
	  _right(1.0f, 0.0f, 0.0f, 0.0f),
	  _loc(0.0f, 0.0f, 0.0f, 0.0f) {};

void DefaultGlutCamera::walk(float unit)
{
	_loc += unit * UnitScale * _forward;
}

void DefaultGlutCamera::strafe(float unit)
{
	_loc += unit * UnitScale * _right;
}

void DefaultGlutCamera::fly(float unit)
{
	_loc += unit * UnitScale * _up;
}

void DefaultGlutCamera::pitch(float radius)
{
}

void DefaultGlutCamera::yaw(float radius)
{
}

void DefaultGlutCamera::roll(float radius)
{
}

void DefaultGlutCamera::update()
{
}

GlutApp::GlutApp()
	: _listener(NULL), _frameCount(0),
	  _clearFlag(0), _timeBegin(std::chrono::high_resolution_clock::now())
{
}

GlutApp::~GlutApp()
{
}

GlutApp::Ptr GlutApp::construct(const GlutStartupParams &params)
{
	if(_appInst.get() != NULL) {
		return _appInst;
	}

	_appInst.reset(new GlutApp());
	_appInst->initialize(params);

	return _appInst;
}

void GlutApp::initialize(const GlutStartupParams &params)
{
	int flag = 0;

	glutInit(&(params.argc), params.argv);
	_listener = params.Listener;

	if(params.isDoubleBuffered)
		flag |= GLUT_DOUBLE;
	else
		flag |= GLUT_SINGLE;

	if(params.isIndexedColorMode)
		flag |= GLUT_INDEX;
	else
		flag |= GLUT_RGBA;

	_clearFlag |= GL_COLOR_BUFFER_BIT;
	if(params.hasDetphBuffer) {
		flag |= GLUT_DEPTH;
		_clearFlag |= GL_DEPTH_BUFFER_BIT;
	}
	if(params.hasStencilBuffer) {
		flag |= GLUT_STENCIL;
		_clearFlag |= GL_STENCIL_BUFFER_BIT;
	}
	if(params.hasAccumBuffer) {
		flag |= GLUT_ACCUM;
		_clearFlag |= GL_ACCUM_BUFFER_BIT;
	}

	glutInitDisplayMode(flag);
	glutInitWindowPosition(params.PosX, params.PosY);
	glutInitWindowSize(params.Width, params.Height);
	glutCreateWindow(params.Title);

	glutDisplayFunc(_render_callback);
	glutReshapeFunc(_reshape_callback);
	glutIdleFunc(_idle_callback);

	glClearColor(
			params.ClearColor.r,
			params.ClearColor.g,
			params.ClearColor.b,
			params.ClearColor.a);
	getListener()->onInit();
	_params = params;
}

void GlutApp::dispatchRenderEvent()
{
	// update camera, etc.
	_frameCount++;
	glClear(_clearFlag);
	unsigned int duration = getElapsedTime(_timeBegin);
	getListener()->onRender(duration);
	if(_params.isDoubleBuffered)
		glutSwapBuffers();
	else
		glFlush();
}

void GlutApp::dispatchReshapeEvent(int w, int h)
{
	getListener()->onResize(w, h);
}

void GlutApp::dispatchIdleEvent()
{
	static auto lastTime = _timeBegin;
	int delta;
	if((delta = getElapsedTime(lastTime)) > 1000) {
		float fps = (float)(_frameCount * 1000) / delta;
		_frameCount = 0;
		lastTime = std::chrono::high_resolution_clock::now();

		char fpsInfo[128];
		snprintf(fpsInfo, 128, "%s - %5.2f fps",
				_params.Title, fps);
		glutSetWindowTitle(fpsInfo);
	}
	getListener()->onIdle();
	glutPostRedisplay();
}
