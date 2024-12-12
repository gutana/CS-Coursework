#include "FishSchoolEntity.h"
#include "GetGlut.h"
#include "ObjLibrary/Vector3.h"

FishSchoolEntity::FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration)
    : numFish(n), maxDistanceFromStart(maxDist), fishSize(size), fishSpeed(speed), fishAcceleration(acceleration)
{
    transform.position = c;
    transform.scale = { r, r, r };
    displayList = nullptr;
    objModel = nullptr;
    showDebugInformation = false;
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

float fishbobbleclock = 0;
Vector3 originalPos;
bool origTset = false;
void FishSchoolEntity::Update(float dt)
{
#ifdef ANCHOVY_SCHOOL_MOVEMENT
    if (fishSpeed != 0.6f) return;

    if (!origTset)
    {
        originalPos = transform.position;
    }
    // Move the fish school around because it looks cool :) 
    transform.position.y = originalPos.y + sin(fishbobbleclock + 0.8 * 2) / 64.0f;
    transform.position.x = originalPos.x + sin(fishbobbleclock + 0.3 * -.3) / 256.0f;
    transform.position.z = originalPos.z + sin(fishbobbleclock + 0.5 * 1.1) / 256.0f;
    fishbobbleclock += dt;
#endif // ANCHOVY_SCHOOL_MOVEMENT
}

