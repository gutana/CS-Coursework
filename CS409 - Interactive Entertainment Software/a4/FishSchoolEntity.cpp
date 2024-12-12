#include "FishSchoolEntity.h"
#include "GetGlut.h"
#include "ObjLibrary/Vector3.h"
#include "Physics.h"

FishSchoolEntity::FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration)
    : numFish(n), maxDistanceFromStart(maxDist), fishSize(size), fishSpeed(speed), fishAcceleration(acceleration),
      FixedEntity(FixedEntityType::NotAFixedEntity)
{
    transform.position = c;
    transform.scale = { r, r, r };
    displayList = nullptr;
    objModel = nullptr;
    showDebugInformation = false;
    playerInsideSchool = false;
}

void FishSchoolEntity::render()
{
    for (auto fish : children)
    {
        fish->render();
    }

    if (showDebugInformation)
    {
        glPushMatrix();
            glTranslated(transform.position.x, transform.position.y, transform.position.z);
            glScaled(transform.scale.x, transform.scale.y, transform.scale.z);
            if (playerInsideSchool)
                glColor3f(1.0, 0.0, 0.0);
            else
                glColor3f(0.0, 1.0, 0.0);
            glutWireSphere(1, 16, 12);
        glPopMatrix();
    }
}

void FishSchoolEntity::Init()
{
    children.reserve(numFish);
    for (int i = 0; i < numFish; i++)
    {
        Vector3 pos = transform.position + Vector3::getRandomSphereVector() * transform.scale.x;
        Vector3 forward = Vector3::getRandomUnitVector();
        children.push_back(new FishEntity(fishSize, fishSpeed, fishAcceleration, pos, forward, displayList, this));
    }
}

std::vector<FishEntity*> FishSchoolEntity::GetFishCollidingWithEntity(Entity* e) const
{
    std::vector<FishEntity*> out;
    for (auto fish : children)
    {
        if (Physics::CheckSphereSphereCollision(e->transform.position, e->transform.scale.x,
            fish->transform.position, fish->transform.scale.x))
        {
            out.emplace_back(fish);
        }
    }
    return out;
}
