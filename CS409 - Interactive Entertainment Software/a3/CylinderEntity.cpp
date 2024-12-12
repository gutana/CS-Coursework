#include "CylinderEntity.h"
#include "GetGlut.h"

#define RADTODEG(x) x * 180 / 3.14159265358979

CylinderEntity::CylinderEntity(Vector3 _start, Vector3 _end, float rad, std::string file)
    : displayList(), objModel()
{
    start = _start;
    end = _end;
    filename = file;
    radius = rad;
    length = start.getDistance(end);
    center = (start + end) / 2;
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
    displayList->draw();
    glPopMatrix();
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

