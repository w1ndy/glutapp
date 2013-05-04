//
// GlutCamera.cpp
//

#include "GlutCamera.h"

GlutCamera::GlutCamera()
	: _forward(0.0f, 0.0f, -1.0f, 0.0f),
	  _up(0.0f, 1.0f, 0.0f, 0.0f),
	  _right(1.0f, 0.0f, 0.0f, 0.0f),
	  _loc(0.0f, 0.0f, 0.0f, 0.0f),
	  _transform(Matrix::buildIdentityMatrix())
{
	_transform(2,3) = -1.0f;
};

void GlutCamera::_recompute()
{
	PendingOperation po;
	while(!_operation.empty()) {
		po = _operation.top();
		_operation.pop();

		switch(po.type)
		{
		case OperationType_Walk: _do_walk(po.param); break;
		case OperationType_Strafe: _do_strafe(po.param); break;
		case OperationType_Fly: _do_fly(po.param); break;
		case OperationType_Roll: _do_roll(po.param); break;
		case OperationType_Yaw: _do_yaw(po.param); break;
		case OperationType_Pitch: _do_pitch(po.param); break;
		}
	}

	_transform(0,0) = _right.x;
	_transform(0,1) = _right.y;
	_transform(0,2) = _right.z;

	_transform(1,0) = _up.x;
	_transform(1,1) = _up.y;
	_transform(1,2) = _up.z;

	_transform(2,0) = _forward.x;
	_transform(2,1) = _forward.y;
	_transform(2,2) = _forward.z;

	_transform(0,3) = -_loc.x;
	_transform(1,3) = -_loc.y;
	_transform(2,3) = -_loc.z;
}

