#pragma once

#include "Entity.h"
#include "PhysicsBody.h"

#include <vector>

#include "ObjLibrary/DisplayList.h"

class FishSchoolEntity;

const float SEPARATION_CONSTANT = 2.0f;
const int NUM_NEIGHBOURS = 4;
const float D_SLOW = 0.01;

class FishEntity : public Entity, public PhysicsBody
{
public:
    FishEntity(float _size, float _speed, float _acceleration, const Vector3& _position, const Vector3& _forward, ObjLibrary::DisplayList* dl, FishSchoolEntity* parent);
    void render();
    void Update(float dt);
    ~FishEntity();
    


    void handleSiblingDeath(FishEntity* deadSibling)
    {
        auto it = std::find_if(nearestNeighbours.begin(), nearestNeighbours.end(),
            [deadSibling](const std::pair<FishEntity*, double>& p) {
            return p.first == deadSibling;
        });

        if (it != nearestNeighbours.end())
            nearestNeighbours.erase(it);

    }

    bool isFlockLeader = false;
    bool showLinesToNeighbours = false;

    std::vector<std::pair<FishEntity*, double>> nearestNeighbours;

    Vector3 calculateDesiredSteeringVector();
    Vector3 calculateDesiredVelocity(); // With respect to the flock

    Vector3 calculateSumOfSeparationForces();
    void calculateNearestNeighbours();
    Vector3 calculateDesiredSeparationVelocity(FishEntity* other) const; // Calculation of separation force
    Vector3 calculateDesiredLeaderFollowVelocity() const; // Calculation of leader following force

private:
    ObjLibrary::DisplayList* m_DisplayList;
    static ObjLibrary::DisplayList* m_DebugDL;
 
    FishSchoolEntity* parentSchool;
    Vector3 schoolCenter;
    float maxDistanceToCenter;

    float maxVelocity;
    float maxAcceleration;

   	void InitNormalsDL() {};
	void DrawNormals() {};
};
