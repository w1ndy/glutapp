#include "GlutApp.h"
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

	void onRender(unsigned int timeElapsed) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glRectf(-10.0f, 10.0f, 10.0f, -10.0f);
	}
	void run() { core->run(); }
};

Application::Application()
{
	core = GlutApp::construct(GlutStartupParams::construct(this));
}

Application::~Application()
{
}

int main()
{
	Application app;
	cout << "Application initialized." << endl;
	app.run();
	return 0;
}
