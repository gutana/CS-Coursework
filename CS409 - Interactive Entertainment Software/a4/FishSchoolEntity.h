#pragma once

#include <vector>

#include "ObjLibrary/ObjModel.h"

#include "Entity.h"
#include "FishEntity.h"

class FishSchoolEntity : public FixedEntity
{
public:
    FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration);
    void render();
    void Init();

    std::vector<FishEntity*> GetFishCollidingWithEntity(Entity* e) const;

    bool showDebugInformation;
    bool playerInsideSchool;

    std::vector<FishEntity*> children;
    int numFish;
    float maxDistanceFromStart;
    float fishSize;
    float fishSpeed;
    float fishAcceleration;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;

   	void InitNormalsDL() {};
	void DrawNormals() {};
};
