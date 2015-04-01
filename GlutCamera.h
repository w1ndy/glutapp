//
// GlutCamera.h
//

#ifndef __GLUTCAMERA_H__
#define __GLUTCAMERA_H__

#include "BaseType.h"
#include <queue>

class GlutCamera
{
private:
	enum OperationType {
		OperationType_Walk,
		OperationType_Strafe,
		OperationType_Fly,
		OperationType_Roll,
		OperationType_Yaw,
		OperationType_Pitch,

		OperationType_Null = 0xFF
	};

	struct PendingOperation
	{
		OperationType 	type;
		float			param;

		PendingOperation()
			: type(OperationType_Null), param(0.0f) {};
		PendingOperation(OperationType _type, float _param)
			: type(_type), param(_param) {};
	};

private:
	Vectorf _forward, _up, _right, _loc;
	Matrixf _transform;
	bool _renew;

private:
	void _recompute();

public:
	GlutCamera();
	~GlutCamera() {};

	void lookAt(Vectorf eye, Vectorf at, Vectorf up);

	inline void reset()
	{
		_transform = Matrixf::buildIdentityMatrix();
		_transform(2,2) = -1;
		_loc.x = 0.0f, _loc.y = 0.0f, _loc.z = 0.0f;
		_forward.x = 0.0f, _forward.y = 0.0f, _forward.z = -1.0f;
		_right.x = 1.0f, _right.y = 0.0f, _right.z = 0.0f;
		_up.x = 0.0f, _up.y = 1.0f, _up.z = 0.0f;
		_renew = false;

	}
	inline void walk(float unit) {
		_loc += unit * _forward;
		_renew = true;
	}

	inline void fly(float unit) {
		_loc += unit * _up;
		_renew = true;
	}

	inline void strafe(float unit) {
		_loc += unit * _right;
		_renew = true;
	}

	inline void roll(float radius) {
		_up *= Matrixf::buildRotationMatrix(_forward, radius);
		_right = _forward * _up;
		_right.normalize();
		_up = _right * _forward;
		_up.normalize();
		_renew = true;
	}

	inline void yaw(float radius) {
		_forward *= Matrixf::buildRotationMatrix(_up, radius);
		_right = _forward * _up;
		_right.normalize();
		_forward = _up * _right;
		_forward.normalize();
		_renew = true;
	}

	inline void pitch(float radius) {
		_forward *= Matrixf::buildRotationMatrix(_right, radius);
		_up = _right * _forward;
		_up.normalize();
		_forward = _up * _right;
		_forward.normalize();
		_renew = true;
	}

	inline void update() {
		if(_renew)
			_recompute();

		glMatrixMode(GL_MODELVIEW);
		_transform.applyExclusively();
	}
};

#endif // __GLUTCAMERA_H__
