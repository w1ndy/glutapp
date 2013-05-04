//
// GlutListener.h
//

#ifndef __GLUTLISTENER_H__
#define __GLUTLISTENER_H__

class GlutListener
{
public:
	virtual ~GlutListener() {};
	virtual void onInit() {};
	virtual void onIdle() {};
	virtual void onResize(int width, int height);
	virtual void onRender(unsigned int timeElapsed) = 0;

	virtual const char *name() const {
		return "<Default GlutListener Name>";
	}

protected:
	void updatePerspectiveView(int width, int height,
			double fovy = 60.0);
	void updateOrthoView(int width, int height,
			double left = -1.0, double right = 1.0,
			double bottom = -1.0, double top = 1.0,
			double near = -1.0, double far = 1.0);
};

class DefaultGlutListener : public GlutListener
{
public:
	void onRender(unsigned int timeElapsed) {};
};

#endif // __GLUTLISTENER_H__
