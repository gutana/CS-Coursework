#include "Transform.h"

Transform::Transform()
{
	position = Vector3::ZERO;
	forward = { 1, 0, 0 };
	up = { 0, 1, 0 };
	right = { 1, 0, 0 };
}

Transform::Transform(const Vector3& _pos, const Vector3& _forward, const Vector3& _up)
{
	position = _pos;
	forward = _forward;
	up = _up;
}

void Transform::setPosition(const Vector3& _pos)
{
	position = _pos;
}

void Transform::setRotation(const Vector3& _forward, const Vector3& _up)
{
	up = _up; 
	forward = _forward;
	right = forward.crossProduct(up);
}

const Vector3& Transform::getPosition() const
{
	return position;
}

const Vector3& Transform::getForward() const
{
	return forward;
}

const Vector3& Transform::getRight() const
{
	return right;
}

const Vector3& Transform::getUp() const
{
	return up;
}

void Transform::moveForward(double distance)
{
	position += forward * distance;
}

void Transform::moveUp(double distance)
{
	position += up * distance;
}

void Transform::moveRight(double distance)
{
	position += right * distance;
}

void Transform::rotateAroundForward(double radians)
{
	up.rotateArbitrary(forward, radians);
	right.rotateArbitrary(forward, radians);
}

void Transform::rotateAroundUp(double radians)
{
	forward.rotateArbitrary(up, radians);
	right.rotateArbitrary(up, radians);
}

void Transform::rotateAroundRight(double radians)
{
	forward.rotateArbitrary(right, radians);
	up.rotateArbitrary(right, radians);
}

void Transform::rotateAroundArbitrary(const Vector3& axis, double radians)
{
}

void Transform::rotateToVector(const Vector3& targetForward, double maxRadians)
{
	if (targetForward.isZero())
		return; 

	Vector3 axis = forward.crossProduct(targetForward);

	if (axis.isZero())
		axis = up;
	else
		axis.normalize();

	double radians = forward.getAngleSafe(targetForward);

	if (radians > maxRadians)
		radians = maxRadians;

	forward.rotateArbitrary(axis, std::min(maxRadians, radians));
	up.rotateArbitrary(axis, std::min(maxRadians, radians));
	right.rotateArbitrary(axis, std::min(maxRadians, radians));

}

void Transform::rotateTowardPosition(const Vector3& pos, double maxRadians)
{
	Vector3 dir = pos - position; 
	dir.normalizeSafe();
	rotateToVector(dir, maxRadians);
}
