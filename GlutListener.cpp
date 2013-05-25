//
// GlutListener.cpp
//

#include "GlutListener.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif // __APPLE__

void GlutListener::onResize(int width, int height)
{
	updateOrthoView(width, height,
			-(double)width / 2, (double)width / 2,
			-(double)height / 2, (double)height / 2);
}

void GlutListener::updatePerspectiveView(int width, int height, double fovy)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(fovy, (double)width / (float)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void GlutListener::updateOrthoView(int width, int height,
		double left, double right, double bottom, double top,
		double near, double far)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(left, right, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
}
