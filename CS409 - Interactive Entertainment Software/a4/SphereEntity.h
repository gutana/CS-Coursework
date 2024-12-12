#pragma once

#include <string>

#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"

#include "Entity.h"
#include "CylinderEntity.h"

#define PI 3.14159265358979

class SphereEntity : public FixedEntity
{
public:
    SphereEntity(Vector3 pos, float rad, std::string file);
    void render();
    std::string filename;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;

   	void InitNormalsDL();
	void DrawNormals();

    Box GetBoundingBox(float padding);

};
