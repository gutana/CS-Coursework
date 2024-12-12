#pragma once

#include "Entity.h"
#include "PhysicsBody.h"
#include "ObjLibrary/DisplayList.h"

class FishSchoolEntity;

//#define ANCHOVY_SCHOOL_MOVEMENT
//#define FISH_SMOOTH_TURNING

class FishEntity : public Entity, public PhysicsBody
{
public:
    FishEntity(float _size, float _speed, float _acceleration, const Vector3& _position, const Vector3& _forward, ObjLibrary::DisplayList* dl, FishSchoolEntity* parent);

    void render();
    void Update(float dt);

private:
    ObjLibrary::DisplayList* m_DisplayList;
    static ObjLibrary::DisplayList* m_DebugDL;
 
    FishSchoolEntity* parentSchool;
    Vector3 schoolCenter;
    float maxDistanceToCenter;

    float speed;
    float acceleration;
};
