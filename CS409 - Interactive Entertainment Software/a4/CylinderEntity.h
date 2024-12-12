#pragma once

#include "Entity.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"
#include "math.h"
#include <string>

struct Box
{
    double minX;
    double minY;
    double minZ;
    double maxX;
    double maxY;
    double maxZ;

    double getWidth() const
    {
        return fabs(maxX - minX);
    }
	double getHeight() const
    {
        return fabs(maxY - minY); 
    }

	double getDepth() const
    {
        return fabs(maxZ - minZ);
    }
};

class CylinderEntity : public FixedEntity
{
public:
    CylinderEntity(Vector3 _start, Vector3 _end, float rad, std::string file);
    void render();
    
    // TODO: use our inherited transform for center, radius, length
    Vector3 start;
    Vector3 end;
    Vector3 center;
    float radius;
    float length;
    std::string filename;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;

    void InitNormalsDL();
    void DrawNormals();

private:
    float getLength();
    Vector3 getDirectionAlongCylinder();
    Box GetCylinderBoundingBox(float padding);

};
