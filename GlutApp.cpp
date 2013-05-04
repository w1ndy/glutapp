#include "GlutApp.h"
#include <cstdio>
#include <cstring>

std::shared_ptr<GlutApp> GlutApp::_appInst;
DefaultGlutListener GlutApp::_defaultListener;

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

GlutApp::GlutApp()
	:  _currentCam(0), _frameCount(0), _clearFlag(0),
	  _timeBegin(std::chrono::high_resolution_clock::now())
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
	_appInst->_initialize(params);

	return _appInst;
}

void GlutApp::_initialize(const GlutStartupParams &params)
{
	int flag = 0;

	glutInit(&(params.argc), params.argv);
	installListener(params.Listener);
	//_listener = params.Listener;

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
	_params = params;
	_dispatchInitEvent();
}

void GlutApp::_dispatchInitEvent()
{
	for(GlutListener *l : _listener)
		l->onInit();
}

void GlutApp::_dispatchRenderEvent()
{
	if(_currentCam) _currentCam->update();

	_frameCount++;
	glClear(_clearFlag);
	unsigned int duration = _getElapsedTime(_timeBegin);
	for(GlutListener *l : _listener)
		l->onRender(duration);
	if(_params.isDoubleBuffered)
		glutSwapBuffers();
	else
		glFlush();
}

void GlutApp::_dispatchReshapeEvent(int w, int h)
{
	for(GlutListener *l : _listener)
		l->onResize(w, h);
}

void GlutApp::_dispatchIdleEvent()
{
	static auto lastTime = _timeBegin;
	int delta;
	if((delta = _getElapsedTime(lastTime)) > 1000) {
		float fps = (float)(_frameCount * 1000) / delta;
		_frameCount = 0;
		lastTime = std::chrono::high_resolution_clock::now();

		char fpsInfo[128];
		snprintf(fpsInfo, 128, "%s - %5.2f fps",
				_params.Title, fps);
		glutSetWindowTitle(fpsInfo);
	}
	for(GlutListener *l : _listener)
		l->onIdle();
	glutPostRedisplay();
}
