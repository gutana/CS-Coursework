#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const Vector3& pos, const Vector3& forward, const Vector3& up)
{
	transform.setPosition(pos);
	transform.setRotation(forward, up);
}
