#pragma once

#include "ObjLibrary/Vector3.h"

#include "Transform.h"

using ObjLibrary::Vector3;

class Camera
{
public:
	Camera();
	Camera(const Vector3& pos, const Vector3& forward, const Vector3& up);

	Transform transform; 

};

