#pragma once

#include <string>

#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"

#include "Entity.h"

class SphereEntity : public Entity
{
public:
    SphereEntity(Vector3 pos, float rad, std::string file);
    void render();
    std::string filename;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;
};
