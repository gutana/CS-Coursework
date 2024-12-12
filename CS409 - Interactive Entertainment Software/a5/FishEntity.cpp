#include "FishEntity.h"
#include "Shapes.h"
#include "FishSchoolEntity.h"
#include "GetGlut.h"
#include "math.h"

ObjLibrary::DisplayList* FishEntity::m_DebugDL = nullptr;


FishEntity::FishEntity(float _size, float _speed, float _acceleration, const Vector3& _position, const Vector3& _forward, ObjLibrary::DisplayList* dl, FishSchoolEntity* parent)
    : m_DisplayList(dl)
{
    transform.position = _position;
    transform.setOrientation(_forward);
    transform.scale = { _size, _size, _size };
    maxVelocity = _speed;
	velocity = transform.forward * maxVelocity;
    maxAcceleration = _acceleration;
    parentSchool = parent;
	schoolCenter = parentSchool->transform.position;
	maxDistanceToCenter = parent->transform.scale.x;
	nearestNeighbours.reserve(NUM_NEIGHBOURS);
}

void FishEntity::Update(float dt)
{
	if (transform.position.y > 0)
	{
		velocity += {0, -0.1, 0};
	}

	if (isFlockLeader)
	{
		// check if near target, if so, choose new one
		if (transform.position.getDistance(parentSchool->currentExploreTarget) < transform.scale.x)
		{
			parentSchool->chooseNewExploreTarget();
		}

		// Seek
		Vector3 target = parentSchool->currentExploreTarget;
		//Vector3 R = target - transform.position;
		//Vector3 D = R.getNormalized() * maxVelocity;
		//Vector3 S = D - velocity;
		//acceleration = S; 
		//velocity += S;

		velocity.rotateTowards(target - transform.position, maxVelocity * dt * 2);

		// max currentSpeed: 85% of fish max currentSpeed
		if (velocity.getDistance({ 0, 0, 0 }) > maxVelocity)
		{
			velocity.truncate(maxVelocity * 0.85);
		}
		
	}
	else
	{
		Vector3 steeringVector = calculateDesiredSteeringVector();
		acceleration = steeringVector;
		velocity += acceleration;
	}

    transform.setOrientation(velocity.getNormalizedSafe());
	updatePosition(dt);
}

FishEntity::~FishEntity()
{
	parentSchool->OnChildDeleted(this);
}

Vector3 FishEntity::calculateDesiredSteeringVector()
{
	Vector3 vec = calculateDesiredVelocity() - velocity; 
	vec.truncate(maxAcceleration);
	return vec;
}

Vector3 FishEntity::calculateDesiredVelocity()
{
	Vector3 desiredVelocity = calculateDesiredLeaderFollowVelocity() + calculateSumOfSeparationForces() * SEPARATION_CONSTANT;

	if (desiredVelocity.isFinite())
		desiredVelocity.truncate(maxVelocity);
	return desiredVelocity;
}
Vector3 FishEntity::calculateSumOfSeparationForces()
{

	//assert(nearestNeighbours.size() == NUM_NEIGHBOURS || nearestNeighbours.size() == parentSchool->children.size() - 1);
	Vector3 sum(0, 0, 0);

	for (int i = 0; i < nearestNeighbours.size(); i++)
	{
		if (nearestNeighbours[i].first == nullptr)
		{
			std::cout << "Nearest neighbour was null. ignoring separation force...\n";
			continue;
		}
		sum += calculateDesiredSeparationVelocity(nearestNeighbours[i].first);
	}
	return sum; 
}

void TryInsertIntoDistanceArray(std::vector<std::pair<FishEntity*, double>>& array, std::pair<FishEntity*, double> pair)
{
	// if (array.size() == NUM_NEIGHBOURS && pair.second > array[NUM_NEIGHBOURS - 1].second) return; 

	std::pair<FishEntity*, double> temp = pair;
	int index = 0;

	while (index < array.size() && temp.second < array[index].second)
	{
		std::pair<FishEntity*, double> tmp = temp;
		temp = array[index];
		array[index] = tmp; 
		index++;
	}

	if (array.size() < NUM_NEIGHBOURS)
		array.push_back(temp);

}

void FishEntity::calculateNearestNeighbours()
{
	for (int i = 0; i < parentSchool->children.size(); i++)
	{
		if (parentSchool->children[i] == this) continue; 

		double distance = transform.position.getDistance(parentSchool->children[i]->transform.position);
		TryInsertIntoDistanceArray(nearestNeighbours, { parentSchool->children[i], distance });
	}
}

Vector3 FishEntity::calculateDesiredSeparationVelocity(FishEntity* other) const
{
	assert(other != nullptr);
	Vector3 R = other->transform.position - transform.position;
	double d = R.getDistance({ 0, 0, 0, });

	if (d == 0)
		return maxVelocity * -R;

	Vector3 dir = -R / d;
	return maxVelocity * (1 - (d / maxDistanceToCenter)) * dir;
}

Vector3 FishEntity::calculateDesiredLeaderFollowVelocity() const
{
	double d0 = (parentSchool->currentFlockLeader->transform.position - transform.position).getDistance({0, 0, 0});
	double t = d0 - maxVelocity;

	Vector3 futureLeaderPos = parentSchool->currentFlockLeader->transform.position + parentSchool->currentFlockLeader->velocity * t;

	Vector3 R = futureLeaderPos - transform.position;
	double d = R.getDistance({ 0, 0, 0 });

	double sClipped;

	if (d < D_SLOW)
		sClipped = maxVelocity * (d / D_SLOW);
	else
		sClipped = maxVelocity;

	Vector3 followVelocity = R / d * sClipped;
	return followVelocity;
}

void FishEntity::render()
{
    glPushMatrix();
		transform.applyDrawTransformations();

		if (parentSchool->showDebugInformation)
		{
			if (m_DebugDL == nullptr)
			{
				m_DebugDL = new ObjLibrary::DisplayList();
				m_DebugDL->begin();
				Shapes::DrawAxes();
				glutWireSphere(1.0, 10, 10);
				m_DebugDL->end();
			}
			else
			{
				m_DebugDL->draw();
			}

		}
		m_DisplayList->draw();
    glPopMatrix();

	if (isFlockLeader && parentSchool->showDebugInformation)
	{
		glPushMatrix();
			transform.applyDrawTransformations();
			if (parentSchool->showDebugInformation)
			{
				glColor3f(1.0, 0.0, 0.0);
				glutWireSphere(1.5, 10, 10);
			}
			m_DisplayList->draw();
		glPopMatrix();

		glBegin(GL_LINES);
			glVertex3d(transform.position.x, transform.position.y, transform.position.z);
			glVertex3d(parentSchool->currentExploreTarget.x, parentSchool->currentExploreTarget.y, parentSchool->currentExploreTarget.z);
		glEnd();

	}

	if (showLinesToNeighbours && parentSchool->showDebugInformation)
	{
		glColor3d(1, 0, 0.96);
		for (int i = 0; i < nearestNeighbours.size(); i++)
		{
			glBegin(GL_LINES);
				glVertex3d(transform.position.x, transform.position.y, transform.position.z);
				glVertex3d(nearestNeighbours[i].first->transform.position.x, 
					nearestNeighbours[i].first->transform.position.y, 
					nearestNeighbours[i].first->transform.position.z);
			glEnd();
		}

		glColor3b(0.1, 0.1, 1.0);
		glBegin(GL_LINES);
			glVertex3d(transform.position.x, transform.position.y, transform.position.z);
			glVertex3d(parentSchool->currentFlockLeader->transform.position.x, 
				parentSchool->currentFlockLeader->transform.position.y, 
				parentSchool->currentFlockLeader->transform.position.z);
		glEnd();
	}
}

