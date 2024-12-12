#pragma once

#include "ObjLibrary/Vector3.h"

using ObjLibrary::Vector3;

const float NORMAL_RENDER_LENGTH = 0.25;

class Shapes
{
public:
	static void DrawNormal(const Vector3& pos, const Vector3& normal, float length);
	static void DrawAxes();
};

