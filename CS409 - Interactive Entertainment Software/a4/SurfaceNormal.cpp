#include "SurfaceNormal.h"

Vector3 SurfaceNormal::getGroundPlain(const Vector3& pos)
{
	return { 0.0, 1.0, 0.0 };
}

Vector3 SurfaceNormal::getSphere(const Vector3& pos, const Vector3& center)
{
	return (pos - center).getNormalizedSafe();
}

Vector3 SurfaceNormal::getVerticalCylinder(const Vector3& pos, const Vector3& center)
{
	return Vector3(pos.x - center.x, 0, pos.z - center.z).getNormalizedSafe();
}

Vector3 SurfaceNormal::getOrientedCylinder(const Vector3& pos, const Vector3& endPos1, const Vector3& endPos2)
{
	Vector3 dir = endPos2 - endPos1;
	Vector3 toQuery = pos - endPos1;
	Vector3 surfaceNormal = toQuery.getRejectionSafe(dir);
	surfaceNormal.normalizeSafe();
	return surfaceNormal;
}

Vector3 SurfaceNormal::getCuboid(const Vector3& pos, const Vector3& center, const Vector3& cuboidHalfSize)
{
	Vector3 signedDist = pos - center;
	Vector3 dist = fabs(signedDist) - cuboidHalfSize;
	Vector3 surfaceNormal;
	// choose axis with largest distance
	if (dist.x > dist.y && dist.x > dist.z)
	{
		// along x
		if (signedDist.x > 0.0)
			surfaceNormal = { 1, 0, 0 };
		else
			surfaceNormal = { -1, 0, 0 };
	}
	else if (dist.y > dist.z)
	{
		// along y		
		if (signedDist.y > 0.0)
			surfaceNormal = { 0, 1, 0 };
		else
			surfaceNormal = { 0, -1, 0 };

	}
	else // along z	
	{
		if (signedDist.z > 0.0)
			surfaceNormal = { 0, 0, 1 };
		else
			surfaceNormal = { 0, 0, -1 };
	}
	return surfaceNormal;
}
