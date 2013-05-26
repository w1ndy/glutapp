//
// GlutSceneManager.cpp
//

#include "GlutSceneManager2D.h"
#include <algorithm>

TrivialSceneManager2D::TrivialSceneManager2D()
{
}

TrivialSceneManager2D::~TrivialSceneManager2D()
{
}

bool TrivialSceneManager2D::addObject(SceneObject2D *object)
{
	_objs.push_back(object);
	return true;
}

void TrivialSceneManager2D::removeObject(SceneObject2D *object)
{
	auto i = find(_objs.begin(), _objs.end(), object);
	if(i != _objs.end()) {
		_objs.erase(i);
	}
}

void TrivialSceneManager2D::removeAllObjects()
{
	_objs.clear();
}

void TrivialSceneManager2D::onRender(unsigned int timeElapsed)
{
	for(SceneObject2D *object : _objs) {
		object->draw(timeElapsed);
	}
}
