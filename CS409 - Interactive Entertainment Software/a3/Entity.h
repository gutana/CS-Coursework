#pragma once

#include "Drawable.h"
#include "Transform.h"
#include "ObjLibrary/Vector3.h"

using ObjLibrary::Vector3;

class Entity : public Drawable
{
public:
    Entity() {}
    Transform transform;
    Vector3 velocity;

    // drag of 1 means no loss of velocity, 0 means full loss. 
    void updatePosition(float dt, float drag = 1);
    void bounce(const Vector3& normal);
};


