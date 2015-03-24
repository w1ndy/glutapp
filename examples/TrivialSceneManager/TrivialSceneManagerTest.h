//
// TrivialSceneManagerTest.h
//

#ifndef __TRIVIALSCENEMANAGERTEST_H__
#define __TRIVIALSCENEMANAGERTEST_H__

#include <GlutApp.h>
#include <GlutGraphics2D.h>
#include <GlutSceneManager2D.h>

class TrivialSceneManagerTest : public GlutListener
{
private:
	GlutApp::Ptr _core;

	TrivialSceneManager2D	*_scenemgr;
	SceneObject2D 			*_object;

public:
	TrivialSceneManagerTest() {
		_core = GlutApp::construct(
				GlutStartupParams::construct(this));

		_scenemgr = new TrivialSceneManager2D();
		_object = new Rectangle(-70.0f, 50.0f, 70.0f, -50.0f, Color::Blue);
		((Rectangle *)_object)->setBorder(true);
		((Rectangle *)_object)->setBorderWidth(3);
		_scenemgr->addObject(_object);

		_core->installListener(_scenemgr);
	}

	~TrivialSceneManagerTest() {
		delete _object;
		delete _scenemgr;
	}

	void run() { _core->run(); }

	void onRender(unsigned int timeElapsed) {}

	const char *name() const {
		return "Trivial SceneManager Test";
	}
};

#endif // __TRIVIALSCENEMANAGERTEST_H__
