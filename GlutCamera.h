//
// GlutCamera.h
//

#ifndef __GLUTCAMERA_H__
#define __GLUTCAMERA_H__

#include "BaseType.h"

class GlutCamera
{
public:
	virtual ~GlutCamera() {};
	virtual void walk(float unit) 		= 0;
	virtual void fly(float unit) 		= 0;
	virtual void strafe(float unit)		= 0;
	virtual void roll(float radius)		= 0;
	virtual void yaw(float radius)		= 0;
	virtual void pitch(float radius)	= 0;
	virtual void update()				= 0;
};

class DefaultGlutCamera : public GlutCamera
{
private:
	Vector _forward, _up, _right, _loc;
	const float UnitScale = 10.0f;

public:
	DefaultGlutCamera();
	~DefaultGlutCamera() {};
	void walk(float unit);
	void fly(float unit);
	void strafe(float unit);
	void roll(float radius);
	void yaw(float radius);
	void pitch(float radius);
	void update();
};

#endif // __GLUTCAMERA_H__
