#include "Shapes.h"
#include "GetGlut.h"

#define PI 3.14159265358979
#define RADTODEG(x) x * 180 / PI

// Draws normal at (0, 0, 0) in direction of normal
void Shapes::DrawNormal(const Vector3& pos, const Vector3& normal, float length)
{
	glPushMatrix();
		glColor3f(0.2, 0.8, 0.2);
		glTranslated(pos.x, pos.y, pos.z);
		glutSolidSphere(0.025, 6, 6);

		Vector3 shortenedNormal = normal * length;

		glBegin(GL_LINES);
			glVertex3d(0, 0, 0);
			glVertex3d(shortenedNormal.x, shortenedNormal.y, shortenedNormal.z);
		glEnd();


		glTranslated(shortenedNormal.x, shortenedNormal.y, shortenedNormal.z);

		double angle = Vector3::UNIT_Z_PLUS.getAngleSafe(normal);
		Vector3 axis = Vector3::UNIT_Z_PLUS.crossProduct(normal).getNormalizedSafe();

		glRotatef(RADTODEG(angle), axis.x, axis.y, axis.z);
		glutSolidCone(0.025, 0.05, 5, 5);

	glPopMatrix();
}

void Shapes::DrawAxes()
{
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
}
