#include "FishEntity.h"
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
    speed = _speed;
	velocity = transform.forward * speed;
    acceleration = _acceleration;
    parentSchool = parent;
	schoolCenter = parentSchool->transform.position;
	maxDistanceToCenter = parent->transform.scale.x;
}

void FishEntity::Update(float dt)
{
	if (transform.position.y > 0)
	{
		velocity += {0, -0.1, 0};
	}

	if (transform.position.isDistanceGreaterThan(schoolCenter, maxDistanceToCenter))
	{
#ifdef FISH_SMOOTH_TURNING
		// Rotating towards the center instead of bouncing to look more natural, 
		velocity.rotateTowards(parentSchool->transform.position - transform.position, speed * dt * 6);
#else
		Vector3 normal = (schoolCenter - transform.position) / (schoolCenter - transform.position).getDistance({0, 0, 0});
		bounce(normal.getNormalized());
#endif // FISH_SMOOTH_TURNING
	}
	updatePosition(dt);
    transform.setOrientation(velocity.getNormalizedSafe());
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
					glBegin(GL_LINES);

					glColor3d(1.0, 0.0, 0.0);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(2.0f, 0, 0);

					glColor3d(0.0, 1.0, 0.0);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(0, 2.0, 0);

					glColor3d(0.0, 0.0, 1.0);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(0.0, 0.0, 2.0);
					glEnd();

					glColor3d(1.0, 0.0, 0.0);
					glPushMatrix();
						glTranslated(2.0, 0.0, 0.0);
						glRotatef(90, 0, 1, 0);
						glutSolidCone(0.1, 0.25, 5, 5);
					glPopMatrix();

					glColor3d(0.0, 1.0, 0.0);
					glPushMatrix();
						glTranslated(0.0, 2.0, 0.0);
						glRotatef(-90, 1, 0, 0);
						glutSolidCone(0.1, 0.25, 5, 5);
					glPopMatrix();

					glColor3d(0.0, 0.0, 1.0);
					glPushMatrix();
						glTranslated(0.0, 0.0, 2.0);
						glRotatef(0, 0, 1, 0);
						glutSolidCone(0.1, 0.25, 5, 5);
					glPopMatrix();
				m_DebugDL->end();
			}
			else
			{
				m_DebugDL->draw();
			}

		}
		m_DisplayList->draw();
    glPopMatrix();
}

