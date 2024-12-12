#pragma once

#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/Vector2.h"

#include "Transform.h"
#include "Entity.h"

using ObjLibrary::Vector3;
using ObjLibrary::Vector2;

class Camera : public Entity
{
public:
	Camera();
	Camera(const Vector3& pos, const Vector3& forward, const Vector3& up);

	bool autopilotEnabled = false;

	void render(){}

	void InitNormalsDL() {};
	void DrawNormals() {};
};

