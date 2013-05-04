//
// GlutCamera.h
//

#ifndef __GLUTCAMERA_H__
#define __GLUTCAMERA_H__

#include "BaseType.h"
#include <stack>

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
	Vector _forward, _up, _right, _loc;
	Matrix _transform;

	std::stack<PendingOperation> _operation;

private:
	void _recompute();

	inline void _do_walk(float unit) {
		_loc += unit * _forward;
	}

	inline void _do_strafe(float unit) {
		_loc += unit * _right;
	}

	inline void _do_fly(float unit) {
		_loc += unit * _up;
	}

	inline void _do_pitch(float radius) {
		_forward *= Matrix::buildRotationMatrix(_right, radius);
		_up = _right * _forward;
		_up.normalize();
		_forward = _up * _right;
		_forward.normalize();
	}

	inline void _do_yaw(float radius) {
		_forward *= Matrix::buildRotationMatrix(_up, radius);
		_right = _forward * _up;
		_right.normalize();
		_forward = _up * _right;
		_forward.normalize();
	}

	inline void _do_roll(float radius) {
		_up *= Matrix::buildRotationMatrix(_forward, radius);
		_right = _forward * _up;
		_right.normalize();
		_up = _right * _forward;
		_up.normalize();
	}

public:
	GlutCamera();
	~GlutCamera() {};

	void lookAt(Vector eye, Vector at, Vector up);

	inline void walk(float unit) {
		_operation.push(PendingOperation(OperationType_Walk, unit));
	}

	inline void fly(float unit) {
		_operation.push(PendingOperation(OperationType_Fly, unit));
	}

	inline void strafe(float unit) {
		_operation.push(PendingOperation(OperationType_Strafe, unit));
	}

	inline void roll(float radius) {
		_operation.push(PendingOperation(OperationType_Roll, radius));
	}

	inline void yaw(float radius) {
		_operation.push(PendingOperation(OperationType_Yaw, radius));
	}

	inline void pitch(float radius) {
		_operation.push(PendingOperation(OperationType_Pitch, radius));
	}

	inline void update() {
		if(!_operation.empty())
			_recompute();

		glMatrixMode(GL_MODELVIEW);
		_transform.applyExclusively();
	}
};

#endif // __GLUTCAMERA_H__
