#pragma once

#include "ObjLibrary/Vector3.h"

using ObjLibrary::Vector3;

class SurfaceNormal
{
public:
	static Vector3 getGroundPlain(const Vector3& pos);
	static Vector3 getSphere(const Vector3& pos, const Vector3& center);
	static Vector3 getVerticalCylinder(const Vector3& pos, const Vector3& center);
	static Vector3 getOrientedCylinder(const Vector3& pos, const Vector3& endPos1, const Vector3& endPos2);
	static Vector3 getCuboid(const Vector3& pos, const Vector3& center, const Vector3& cuboidHalfSize);
};

