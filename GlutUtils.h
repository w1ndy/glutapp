#ifndef __GLUTUTILS_H__
#define __GLUTUTILS_H__

#include <iostream>
#include "BaseType.h"

template<class T>
int invoke()
{
    T app;
    std::cout << app.name() << " initialized." << std::endl;
    return app.run();
}

void drawAxis(float limit)
{
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-limit, 0.0f, 0.0f);
        glVertex3f(limit, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -limit, 0.0f);
        glVertex3f(0.0f, limit, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -limit);
        glVertex3f(0.0f, 0.0f, limit);
    glEnd();
}

#endif // __GLUTUTILS_H__
