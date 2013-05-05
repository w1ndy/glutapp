//
// GlutCamera.cpp
//

#include "GlutCamera.h"

GlutCamera::GlutCamera()
	: _forward(0.0f, 0.0f, -1.0f, 0.0f),
	  _up(0.0f, 1.0f, 0.0f, 0.0f),
	  _right(1.0f, 0.0f, 0.0f, 0.0f),
	  _loc(0.0f, 0.0f, 0.0f, 0.0f),
	  _transform(Matrix::buildIdentityMatrix()),
	  _renew(false)
{
	_transform(2,2) = -1.0f;
};

void GlutCamera::lookAt(Vector eye, Vector at, Vector up)
{
	_loc = eye;
	_forward = at - eye;
	_forward.normalize();
	_right = _forward * _up;
	_right.normalize();
	_up = _right * _forward;
	_up.normalize();
	_recompute();
}

void GlutCamera::_recompute()
{
	_transform(0,0) = _right.x;
	_transform(0,1) = _right.y;
	_transform(0,2) = _right.z;

	_transform(1,0) = _up.x;
	_transform(1,1) = _up.y;
	_transform(1,2) = _up.z;

	_transform(2,0) = _forward.x;
	_transform(2,1) = _forward.y;
	_transform(2,2) = _forward.z;

	_transform(0,3) = -_right.dot(_loc);
	_transform(1,3) = -_up.dot(_loc);
	_transform(2,3) = _forward.dot(_loc);

	_renew = false;
}

