#ifndef __EXAMPLE_APPLICATION_H__
#define __EXAMPLE_APPLICATION_H__

#include <GlutApp.h>
#include <BaseType.h>
#include <iostream>
#include <memory>
using namespace std;

class ExampleApplication : public GlutListener
{
protected:
    GlutApp::Ptr core;

public:
    // Do not use variable 'core' in the constructor
    // Class may haven't been initialized
    ExampleApplication();
    virtual ~ExampleApplication();

    // Rendering call-back function.
    virtual void onRender(unsigned int timeElapsed) = 0;

    int run() { return core->run(); }

    virtual const char *name() const = 0;
};

ExampleApplication::ExampleApplication()
{
    // Construct a GlutApp instance using default parameters.
    core = GlutApp::construct(GlutStartupParams::construct(this));
}

ExampleApplication::~ExampleApplication()
{
}

#endif // __EXAMPLE_APPLICATION_H__
