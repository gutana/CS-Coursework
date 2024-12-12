#include "Physics.h"

bool Physics::CheckSphereSphereCollision(const Vector3& c1, double r1, const Vector3& c2, double r2)
{
	double distance = c2.getDistance(c1);

	if (distance < r1 + r2)
		return true;

	return false; 
}

bool Physics::CheckEntityCylinderCollision(Entity* entity, CylinderEntity* cylinder, Vector3& out_rejection)
{
	Vector3 rejection = (entity->transform.position - cylinder->end).getRejection(cylinder->start - cylinder->end);

	float rejectionMagnitude = rejection.getDistance({ 0, 0, 0 });

	Vector3 projection = (entity->transform.position - cylinder->end).getProjection(cylinder->start - cylinder->end);
	float projectionMagnitude = projection.getDistance({ 0, 0, 0 });

	if ((cylinder->end + projection).isDistanceGreaterThan(cylinder->start, cylinder->length) ||
		(cylinder->end + projection).isDistanceGreaterThan(cylinder->end, cylinder->length))
		return false;

	if (rejectionMagnitude < cylinder->radius + entity->transform.scale.x)
	{
		out_rejection = rejection;
		return true;
	}
	return false;
}

void Physics::BounceEntityOffNormal(Entity* entity, const Vector3& normal, float velocityModifier)
{
	entity->bounce(normal);
	entity->velocity *= velocityModifier;
}

void Physics::EnforceMinimumDistance(Entity* entity, FixedEntity* fixedEntity, const Vector3& normal)
{
	if (entity->transform.position.isDistanceLessThan(fixedEntity->transform.position,	
													   entity->transform.scale.x + fixedEntity->transform.scale.x))
	{
		entity->transform.position = fixedEntity->transform.position + 
									 normal * 
									 (entity->transform.scale.x + fixedEntity->transform.scale.x);
	}
}

void Physics::EnforceMinimumDistance(Entity* entity, const Vector3& other, const Vector3& normal, double distance)
{
	if (entity->transform.position.isDistanceLessThan(other, distance))
	{
		entity->transform.position = other + normal * distance;
	}
}

