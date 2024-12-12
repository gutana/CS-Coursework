#include "Transform.h"
#include "GetGlut.h"

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

Transform::Transform(const Vector3& pos)
{
	position = pos;
	forward = { 1, 0, 0 };
	up = { 0, 1, 0 };
	right = { 1, 0, 0 };
}

Transform::Transform(const Vector3& pos, const Vector3& _forward)
	:  position(pos), forward(_forward)
{
	up = calculateUpVector(forward);
	right = forward.crossProduct(up);
}

void Transform::applyDrawTransformations() const
{
	glTranslated(position.x, position.y, position.z);
	
	double matrix[16] =
	{
		forward.x, forward.y, forward.z, 0.0,
		     up.x,      up.y,      up.z, 0.0,
		  right.x,   right.y,   right.z, 0.0,
		      0.0,       0.0,       0.0, 1.0
	};

	glMultMatrixd(matrix);
	glScaled(scale.x, scale.y, scale.z);

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

void Transform::setOrientation(const Vector3& _forward)
{
	forward = _forward;
	up = calculateUpVector(_forward);
	right = forward.crossProduct(up);
	right.normalize();
}

void Transform::rotateToUpright(double maxRadians)
{
	Vector3 targetUp = calculateUpVector(forward);
	Vector3 axis = up.crossProduct(targetUp);

	if (axis.isZero())
		axis = forward;
	else
		axis.normalize();

	double radians = up.getAngleSafe(targetUp);
	if (radians > maxRadians)
		radians = maxRadians;

	up.rotateArbitrary(axis, radians);
	right.rotateArbitrary(axis, radians);

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
	throw;
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

void Transform::randomizeUp()
{
	double random = rand() / (RAND_MAX + 1.0);
	up.rotateArbitrary(forward, random * 2 * 3.1415926535);
	right.rotateArbitrary(forward, random * 2 * 3.1415926535);
}

void Transform::randomizeOrientation()
{
	forward = Vector3::getRandomUnitVector();
	up = calculateUpVector(forward);
	right = forward.crossProduct(up);
}

Vector3 Transform::calculateUpVector(const Vector3& localForward) const
{
	static const Vector3 IDEAL_UP_VECTOR(0.0, 1.0, 0.0);
	static const double HALF_PI = 1.5707963267948966;

	if (localForward.isZero())
	{
		std::cout << "Warning: Zero vector given to Transform::calculateUpVector\n";
		return IDEAL_UP_VECTOR;
	}

	Vector3 axis = localForward.crossProduct(IDEAL_UP_VECTOR);

	if (axis.isZero())
		return Vector3(0.0, 1.0, 0.0);

	axis.normalize();

	Vector3 localUp = localForward.getRotatedArbitrary(axis, HALF_PI);
	return localUp;
}
