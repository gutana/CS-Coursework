#pragma once

#include <vector>

#include "ObjLibrary/ObjModel.h"

#include "Entity.h"
#include "FishEntity.h"

class FishSchoolEntity : public Entity, public PhysicsBody
{
public:
    FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration);
    void render();
    void Init();
    void Update(float dt);

    bool showDebugInformation;

    std::vector<FishEntity*> children;
    int numFish;
    float maxDistanceFromStart;
    float fishSize;
    float fishSpeed;
    float fishAcceleration;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;
};
