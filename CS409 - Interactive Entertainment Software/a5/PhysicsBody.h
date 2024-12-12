#pragma once

#include "ObjLibrary/Vector3.h"

class PhysicsBody
{
public:
	ObjLibrary::Vector3 acceleration;

	PhysicsBody() : acceleration(ObjLibrary::Vector3::ZERO) 
	{
	}

	virtual void Update(float dt) = 0;
};

