#include "Entity.h"
#include <cmath>

void Entity::updatePosition(float dt, float drag)
{
    transform.position = transform.position + velocity * dt;
    double fraction = pow(drag, dt);
    velocity *= fraction;
}

void Entity::bounce(const Vector3& normal)
{
    // check if velocity and normal are within 90 degrees
    // if so, already moving away so don't bounce
    if (normal.dotProduct(velocity) > 0)
        return; 

    // split velocity into projection and rejection, new velocity is velocity - 2 * projection
    Vector3 rejection = velocity.getRejection(normal);
    Vector3 projection = velocity.getProjection(normal);
    velocity = velocity - 2 * projection;
    transform.setOrientation(velocity.getNormalizedSafe());
}
