#include "SphereEntity.h"
#include "GetGlut.h"

SphereEntity::SphereEntity(Vector3 pos, float rad, std::string file)
    : objModel(), displayList()
{
    transform.position = pos;
    transform.scale = { rad, rad, rad };
    filename = file;
}

void SphereEntity::render()
{
    glPushMatrix();
    glTranslatef(transform.position.x, transform.position.y, transform.position.z);
    glScalef(transform.scale.x, transform.scale.y, transform.scale.z);
    displayList->draw();
    glPopMatrix();
}

