#include "FishSchoolEntity.h"
#include "GetGlut.h"
#include "ObjLibrary/Vector3.h"
#include "Physics.h"
#include <random>

FishSchoolEntity::FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration, HeightMap* hmap)
    : numFish(n), maxDistanceFromStart(maxDist), fishSize(size), maxFishSpeed(speed), maxFishAcceleration(acceleration),
      FixedEntity(FixedEntityType::NotAFixedEntity)
{

    originalPos = c;
    originalRadius = r;
    transform.position = c;

    transform.scale = { r, r, r };
    displayList = nullptr;
    objModel = nullptr;
    showDebugInformation = false;
    playerInsideSchool = false;

    exploreAreaCenter = c;
    maxExploreDistance = maxDist;
    currentExploreTarget = c;
    _hmap = hmap;
}

void FishSchoolEntity::render()
{
    for (auto fish : children)
    {
        fish->render();
    }

    if (showDebugInformation)
    {
        Vector3 avg(0, 0, 0);
        double maxDist = 0;
        for (auto fish : children)
        {
            avg += fish->transform.position;
        }
        avg /= children.size();
        transform.position = avg; 
        for (auto fish : children)
        {
            float dist = transform.position.getDistance(fish->transform.position);
            if (dist > maxDist) maxDist = dist;
        }
        transform.scale.x = maxDist + fishSize;
        transform.scale.y = maxDist + fishSize;
        transform.scale.z = maxDist + fishSize;

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
        children.push_back(new FishEntity(fishSize, maxFishSpeed, maxFishAcceleration, pos, forward, displayList, this));
    }
    for (int i = 0; i < children.size(); i++)
    {
        children[i]->calculateNearestNeighbours();
    }
    
    chooseNewFlockLeader();
    chooseNewNeighboursDisplayer();
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

void FishSchoolEntity::chooseNewExploreTarget()
{
    do
    {
        currentExploreTarget = originalPos + Vector3::getRandomSphereVector() * originalRadius;
    } while (currentExploreTarget.y < _hmap->getHeight(currentExploreTarget));

}

int randomInRange(int upper, int lower = 0) // returns [lower, upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(lower, upper - 1);
    return dist(gen);

}

void FishSchoolEntity::chooseNewFlockLeader()
{
    if (children.size() == 0) return; 
    int index = randomInRange(children.size());
    currentFlockLeader = children[index];
    currentFlockLeader->isFlockLeader = true;
}

void FishSchoolEntity::chooseNewNeighboursDisplayer()
{
    if (children.size() == 0) return; 
    int index = randomInRange(children.size());
    children[index]->showLinesToNeighbours = true;
}
