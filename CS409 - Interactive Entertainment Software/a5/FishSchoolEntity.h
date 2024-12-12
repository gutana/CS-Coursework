#pragma once

#include <vector>

#include "ObjLibrary/ObjModel.h"

#include "Entity.h"
#include "FishEntity.h"
#include "Heightmap.h"

class FishSchoolEntity : public FixedEntity
{
public:
    FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration, HeightMap* hmap);
    void render();
    void Init();

    void OnChildDeleted(FishEntity* deadChild)
    {
        for (auto child : children)
        {
            if (child == deadChild) continue; 
            child->handleSiblingDeath(deadChild);
        }

        if (deadChild->showLinesToNeighbours)
            chooseNewNeighboursDisplayer();
    }

    std::vector<FishEntity*> GetFishCollidingWithEntity(Entity* e) const;

    bool showDebugInformation;
    bool playerInsideSchool;

    std::vector<FishEntity*> children;
    int numFish;
    float maxDistanceFromStart;
    float fishSize;
    float maxFishSpeed;
    float maxFishAcceleration;
    ObjLibrary::DisplayList* displayList;
    ObjLibrary::ObjModel* objModel;

    Vector3 exploreAreaCenter;
    float maxExploreDistance;
    FishEntity* currentFlockLeader;
    Vector3 currentExploreTarget;
    Vector3 originalPos;
    float originalRadius;

    HeightMap* _hmap;

    void chooseNewExploreTarget();
    void chooseNewFlockLeader();
    void chooseNewNeighboursDisplayer();

   	void InitNormalsDL() {};
	void DrawNormals() {};


    unsigned int m_NearestNeighbourUpdateCounter = 0;
    const unsigned int UPDATE_RATE = 5; // updates split among 5 frames
    void UpdateNeighbours()
    {
        for (int i = 0; i < children.size(); i++)
        {
            if (i % UPDATE_RATE == 0)
            {
                children[i]->calculateNearestNeighbours();
            }
        }
        m_NearestNeighbourUpdateCounter++;
    }

};
