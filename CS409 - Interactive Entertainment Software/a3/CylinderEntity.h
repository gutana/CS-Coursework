#pragma once

#include "Entity.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"
#include <string>

class CylinderEntity : public Entity
{
public:
    CylinderEntity(Vector3 _start, Vector3 _end, float rad, std::string file);
    void render();

    Vector3 start;
    Vector3 end;
    Vector3 center;
    float radius;
    float length;
    std::string filename;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;

private:
    float getLength();
    Vector3 getDirectionAlongCylinder();
};
