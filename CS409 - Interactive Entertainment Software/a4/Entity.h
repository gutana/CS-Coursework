#pragma once

#include "Drawable.h"
#include "Transform.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/DisplayList.h"

using ObjLibrary::Vector3;

enum FixedEntityType
{
    Sphere,
    Cylinder,
    NotAFixedEntity // ??????
};

class FixedEntity : public Drawable
{
public:
    FixedEntity(FixedEntityType fixedEntityType): entityType(fixedEntityType) {}

    Transform transform;
    FixedEntityType entityType;
    ObjLibrary::DisplayList* m_NormalsDL;
    virtual void InitNormalsDL() = 0;
    virtual void DrawNormals() = 0;

    bool ShowNormals = false;
};


class Entity : public FixedEntity
{
public:
    Entity() : FixedEntity(FixedEntityType::NotAFixedEntity) {}
    Vector3 velocity;

    // drag of 1 means no loss of velocity, 0 means full loss. 
    void updatePosition(float dt, float drag = 1);
    void bounce(const Vector3& normal);
};

