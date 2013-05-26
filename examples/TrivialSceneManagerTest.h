//
// TrivialSceneManagerTest.h
//

#ifndef __TRIVIALSCENEMANAGERTEST_H__
#define __TRIVIALSCENEMANAGERTEST_H__

#include "../GlutApp.h"
#include "../GlutGraphics2D.h"
#include "../GlutSceneManager2D.h"

class TrivialSceneManagerTest : public GlutListener
{
private:
	GlutApp::Ptr _core;

	TrivialSceneManager2D* _scenemgr;
	Rectangle *_rect;

public:
	TrivialSceneManagerTest() {
		_core = GlutApp::construct(
				GlutStartupParams::construct(this));

		_scenemgr = new TrivialSceneManager2D();
		_rect = new Rectangle(-10.0, 10.0f, 20.0f, 20.0f, Color::Red);
		_scenemgr->addObject(_rect);

		_core->installListener(_scenemgr);
	}

	~TrivialSceneManagerTest() {
		delete _rect;
		delete _scenemgr;
	}

	void run() { _core->run(); }

	void onRender(unsigned int timeElapsed) {}

	const char *name() const {
		return "Trivial SceneManager Test";
	}
};

#endif // __TRIVIALSCENEMANAGERTEST_H__
