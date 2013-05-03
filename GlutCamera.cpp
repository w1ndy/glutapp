//
// GlutCamera.cpp
//

#include "GlutCamera.h"

DefaultGlutCamera::DefaultGlutCamera()
	: _forward(0.0f, 0.0f, -1.0f, 0.0f),
	  _up(0.0f, 1.0f, 0.0f, 0.0f),
	  _right(1.0f, 0.0f, 0.0f, 0.0f),
	  _loc(0.0f, 0.0f, 0.0f, 0.0f) {};

void DefaultGlutCamera::walk(float unit)
{
	_loc += unit * UnitScale * _forward;
}

void DefaultGlutCamera::strafe(float unit)
{
	_loc += unit * UnitScale * _right;
}

void DefaultGlutCamera::fly(float unit)
{
	_loc += unit * UnitScale * _up;
}

void DefaultGlutCamera::pitch(float radius)
{
}

void DefaultGlutCamera::yaw(float radius)
{
}

void DefaultGlutCamera::roll(float radius)
{
}

void DefaultGlutCamera::update()
{
}
