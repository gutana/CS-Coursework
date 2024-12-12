#pragma once

#include "Entity.h"
#include "CylinderEntity.h"

#include "ObjLibrary/Vector3.h"

using ObjLibrary::Vector3;

class Physics
{
public:
	static bool CheckSphereSphereCollision(const Vector3& c1, double r1, const Vector3& c2, double r2);
	// assumes entity is a sphere,
	// If entity is inside of the cylinder, it will be placed at the minimum distance in the direction of the normal
	static bool CheckEntityCylinderCollision(Entity* entity, CylinderEntity* cylinder,
											 Vector3& out_rejection);
	

	static void BounceEntityOffNormal(Entity* entity, const Vector3& normal, float velocityModifier = 1);

	// Assumes entity is a sphere
	// If entity is inside of the fixedEntity, it will be placed at the minimum distance in the direction of the normal
	static void EnforceMinimumDistance(Entity* entity, FixedEntity* fixedEntity, const Vector3& normal);
	static void EnforceMinimumDistance(Entity* entity, const Vector3& fixedPoint, const Vector3& normal, double distance);

};

