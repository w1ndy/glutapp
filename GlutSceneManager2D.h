//
// GlutSceneManager2D.h
//

#ifndef __GLUTSCENEMANAGER_H__
#define __GLUTSCENEMANAGER_H__

#include <iostream>
#include <list>

#include "BaseType.h"
#include "GlutBounds2D.h"
#include "GlutListener.h"

class SceneObject2D
{
public:
	virtual ~SceneObject2D() {};

	virtual const Bounds2D &getBounds() const	= 0;
	virtual void draw(unsigned int timeElapsed)	= 0;

	virtual float getX() const			=0;
	virtual float getY() const			=0;
};

class SceneManager2D : public GlutListener
{
public:
	virtual ~SceneManager2D() {};

	virtual bool addObject(SceneObject2D *object) 		= 0;
	virtual void removeObject(SceneObject2D *object) 	= 0;
	virtual void removeAllObjects()						= 0;

	virtual void onRender(unsigned int timeElapsed)		= 0;
};

class TrivialSceneManager2D : public SceneManager2D
{
private:
	std::list<SceneObject2D *> _objs;

public:
	TrivialSceneManager2D();
	~TrivialSceneManager2D();

	bool addObject(SceneObject2D *object);
	void removeObject(SceneObject2D *object);
	void removeAllObjects();

	void onRender(unsigned int timeElapsed);
};

class QuadtreeSceneManager2D : public SceneManager2D
{
private:
	struct _quadtree {
		union {
			struct {
				float x, y;
			} position;
			SceneObject2D *object;
		} data;

		bool leaf;

		_quadtree *leftupper;
		_quadtree *leftlower;
		_quadtree *rightlower;
		_quadtree *rightupper;
	} *root;

private:
	bool _quadtree_init();
	void _quadtree_destroy();
	void _quadtree_add_object(SceneObject2D *object);
	void _quadtree_remove_object(SceneObject2D *object);

public:
	bool addObject(SceneObject2D *object);
	void removeObject(SceneObject2D *object);
	void removeAllObjects();

	void onRender(unsigned int timeElapsed);
};

#endif // __GLUTSCENEMANAGER_H__
