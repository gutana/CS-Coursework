#pragma once

#include "ObjLibrary/Vector3.h"

using ObjLibrary::Vector3;

class Transform
{
public:
	Transform();
	Transform(const Vector3& pos, const Vector3& forward, const Vector3& up);
	Transform(const Vector3& pos);
	Transform(const Vector3& pos, const Vector3& forward);

	void applyDrawTransformations() const;

	void setPosition(const Vector3& pos);
	void setRotation(const Vector3& forward, const Vector3& up);
	void setOrientation(const Vector3& forward);

	void rotateToUpright(double maxRadians);

	const Vector3& getPosition() const;
	const Vector3& getForward() const;
	const Vector3& getRight() const;
	const Vector3& getUp() const;

	void moveForward(double distance);
	void moveUp(double distance);
	void moveRight(double distance);

	void rotateAroundForward(double radians);
	void rotateAroundUp(double radians);
	void rotateAroundRight(double radians);
	void rotateAroundArbitrary(const Vector3& axis, double radians);
	void rotateToVector(const Vector3& targetForward, double maxRadians);
	void rotateTowardPosition(const Vector3& pos, double maxRadians);

	Vector3 position;
	Vector3 forward;
	Vector3 up;
	Vector3 right;
	Vector3 scale = { 1.0, 1.0, 1.0 };

	void randomizeUp();
	void randomizeOrientation();

private:
	Vector3 calculateUpVector(const Vector3& localForward) const;
};

