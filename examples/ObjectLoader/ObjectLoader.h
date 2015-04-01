#ifndef __OBJECT_LOADER_H__
#define __OBJECT_LOADER_H__

#include "../ExampleApplication.h"

#include <GlutCamera.h>
#include <GlutMesh.h>
#include <GlutUtils.h>

#include <string>

class ObjectLoader : public ExampleApplication
{
private:
    GlutCamera _cam;
    GlutHighPrecisionMesh _mesh;

    std::string _objfname;

public:
    ObjectLoader(const char *fname) : _objfname(fname) {}
    virtual ~ObjectLoader() {};

    virtual void onResize(int width, int height) {
        updatePerspectiveView(width, height);
    }

    virtual void onInit() {
        glShadeModel(GL_SMOOTH);

        float specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

        float diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

        float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    }

    virtual void onRender(unsigned int timeElapsed) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        float position[] = { -1.5f, 1.0f, 4.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        Matrixf::buildScalingMatrix(5.0f).apply();
        Matrixf::buildRotationMatrixAroundX(PI / 2).apply();
        float mat[] = { 0.7f, 0.7f, 0.7f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);

        _cam.reset();
        _cam.yaw((float)timeElapsed / 1000.0f);
        _cam.walk(-10.0f);

        _mesh.draw();

        // uncommenting this will draw vertex normals.
        //_mesh.drawNormals();
    }

    int run() {
        if(!_mesh.fromObjFile(_objfname.c_str()))
            return -255;

        core->installCamera(&_cam);

        return core->run();
    }

    virtual const char *name() const {
        return "GL Object Loader";
    }
};

#endif // __OBJECT_LOADER_H__
