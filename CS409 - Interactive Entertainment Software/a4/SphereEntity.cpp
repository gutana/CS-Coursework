#include "SphereEntity.h"
#include "GetGlut.h"
#include "SurfaceNormal.h"
#include "Shapes.h"
#include "tgmath.h"

SphereEntity::SphereEntity(Vector3 pos, float rad, std::string file)
    : objModel(), displayList(), FixedEntity(FixedEntityType::Sphere)
{
    transform.position = pos;
    transform.scale = { rad, rad, rad };
    filename = file;
}

void SphereEntity::render()
{
    glPushMatrix();
    glTranslatef(transform.position.x, transform.position.y, transform.position.z);
    glScalef(transform.scale.x, transform.scale.y, transform.scale.z);
    displayList->draw();
    glPopMatrix();

    if (!ShowNormals) return; 

    if (m_NormalsDL == nullptr)
        InitNormalsDL(); // only init with the DL if it ends up being needed
    else
        m_NormalsDL->draw();
}

void SphereEntity::InitNormalsDL()
{
    m_NormalsDL = new ObjLibrary::DisplayList();
    m_NormalsDL->begin();

    int gridSize = 10;

    for (int i = 0; i < 10; i++)
    {
        double latitude = PI * (double)i / (gridSize);
        for (int j = 0; j < 10; j++)
        {
            double longitude = 2.0 * PI * (double)j / (gridSize);
            
            Vector3 pos(transform.position.x + transform.scale.x * sin(latitude) * cos(longitude),
                        transform.position.y + transform.scale.y * sin(latitude) * sin(longitude),
                        transform.position.z + transform.scale.z * cos(latitude));

            Vector3 normal = SurfaceNormal::getSphere(pos, transform.position);
            Shapes::DrawNormal(pos, normal, 0.25);
        }
    }
    

    m_NormalsDL->end();
}

void SphereEntity::DrawNormals()
{
    if (m_NormalsDL == nullptr) throw "Uninitialized normals DL on SphereEntity";

    m_NormalsDL->draw();
}

Box SphereEntity::GetBoundingBox(float padding)
{
    Box box;
    box.minX = transform.position.x - (transform.scale.x + padding);
    box.minY = transform.position.y - (transform.scale.y + padding);
    box.minZ = transform.position.z - (transform.scale.z + padding);

    box.maxX = transform.position.x + transform.scale.x + padding;
    box.maxY = transform.position.y + transform.scale.y + padding;
    box.maxZ = transform.position.z + transform.scale.z + padding;

    return box;
}

