#pragma once

#include "ObjLibrary/Vector3.h"

#include "Transform.h"
#include "Entity.h"

using ObjLibrary::Vector3;

class Camera : public Entity
{
public:
	Camera();
	Camera(const Vector3& pos, const Vector3& forward, const Vector3& up);

	void render(){}
};

