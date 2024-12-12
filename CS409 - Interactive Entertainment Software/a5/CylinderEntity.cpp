#include "CylinderEntity.h"
#include "GetGlut.h"
#include "SurfaceNormal.h"
#include "Shapes.h"

#define PI 3.14159265358979
#define RADTODEG(x) x * 180 / PI

CylinderEntity::CylinderEntity(Vector3 _start, Vector3 _end, float rad, std::string file)
    : displayList(), objModel(), FixedEntity(FixedEntityType::Cylinder)
{
    start = _start;
    end = _end;
    filename = file;
    radius = rad;
    length = start.getDistance(end);
    center = (start + end) / 2;
    transform.position = center;
}

void CylinderEntity::render()
{
    Vector3 currentDir = Vector3::UNIT_X_PLUS;
    Vector3 targetDir = getDirectionAlongCylinder();
    Vector3 axis = currentDir.crossProduct(targetDir);
    axis.normalizeSafe();

    double radians = currentDir.getAngleSafe(targetDir);
    double degrees = RADTODEG(radians);

    glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glRotated(degrees, axis.x, axis.y, axis.z);
		glScalef(length / 2, radius, radius);
		displayList->draw(); // TODO: Display list can contain all our bullshit
    glPopMatrix();

    if (!ShowNormals) return;

    if (m_NormalsDL == nullptr)
        InitNormalsDL();
    else
        m_NormalsDL->draw();
}

void CylinderEntity::InitNormalsDL()
{
    m_NormalsDL = new ObjLibrary::DisplayList();
    m_NormalsDL->begin();

    for (int i = 0; i < 100; i++)
    {
		Vector3 unrotated = Vector3::getRandomUnitVectorYZ() * radius;
		unrotated.x = getLength() * Vector3::getRandomInRange(-0.5, 0.5).x;

		Vector3 cylDir = getDirectionAlongCylinder();
		assert(!cylDir.isZero());

		Vector3 axis = Vector3::UNIT_X_PLUS.crossProduct(cylDir);
		axis.normalizeSafe();
		double rad = Vector3::UNIT_X_PLUS.getAngleSafe(cylDir);
		double deg = RADTODEG(rad);

		Vector3 rotated = unrotated.getRotatedArbitrary(axis, rad);
		Vector3 pos = center + rotated;
	   
		Shapes::DrawNormal(pos, SurfaceNormal::getOrientedCylinder(pos, start, end), 0.25);
    }
	m_NormalsDL->end();

}

void CylinderEntity::DrawNormals()
{
    if (m_NormalsDL == nullptr) throw "Uninitialized Normals DL on CylinderEntity";

    m_NormalsDL->draw();

}

float CylinderEntity::getLength()
{
    return start.getDistance(end);
}

Vector3 CylinderEntity::getDirectionAlongCylinder()
{
    Vector3 a = end - start;
    a.normalizeSafe();
    return a;
}

Box CylinderEntity::GetCylinderBoundingBox(float padding = 0)
{
    Box box;
    box.minX = (start.x < end.x ? start.x : end.x) - (radius + padding);
    box.minY = (start.y < end.y ? start.y : end.y) - (radius + padding);
    box.minZ = (start.z < end.z ? start.z : end.z) - (radius + padding);

    box.maxX = (start.x < end.x ? end.x : start.x) + (radius + padding);
    box.maxY = (start.y < end.y ? end.y : start.y) + (radius + padding);
    box.maxZ = (start.z < end.z ? end.z : start.z) + (radius + padding);


    return box;
}

