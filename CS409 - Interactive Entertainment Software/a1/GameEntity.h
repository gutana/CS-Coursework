#pragma once

#include <string>

#include "GetGlut.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"

#include "Drawable.h"

using ObjLibrary::Vector3;
using ObjLibrary::DisplayList;

#define RADTODEG(x) x * 180 / 3.14159265358979

class SphereEntity : public Drawable
{
public:

	Vector3 position;
	float radius;
	std::string filename;
	DisplayList displayList;
	ObjLibrary::ObjModel objModel;

	SphereEntity(Vector3 pos, float rad, std::string file)
		:objModel(), displayList()
	{
		position = pos;
		radius = rad;
		filename = file;
	}

	void render()
	{
		glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(radius, radius, radius);
			displayList.draw();
		glPopMatrix();
	}
};

class CylinderEntity : public Drawable
{
public:
	Vector3 start;
	Vector3 end; 
	Vector3 center;
	float radius;
	float length;
	std::string filename; 
	DisplayList displayList;
	ObjLibrary::ObjModel objModel;


	CylinderEntity(Vector3 _start, Vector3 _end, float rad, std::string file)
		: displayList(), objModel()
	{
		start = _start;
		end = _end;
		filename = file;
		radius = rad;
		length = start.getDistance(end);
		center = (start + end) / 2;
	}

	void render()
	{
		Vector3 currentDir = Vector3::UNIT_X_PLUS;
		Vector3 targetDir = getDirectionAlongCylinder();
		Vector3 axis = currentDir.crossProduct(targetDir);
		axis.normalizeSafe();

		double radians = currentDir.getAngleSafe(targetDir);
		double degrees = RADTODEG(radians);


		glPushMatrix();
			glTranslatef(center.x, center.y, center.z);
			glRotated(degrees, axis.x, axis.y, axis.z);
			glScalef(length / 2, radius, radius);
			displayList.draw();
		glPopMatrix();
	}

private:

	float getLength()
	{
		return start.getDistance(end);
	}

	Vector3 getDirectionAlongCylinder()
	{
		Vector3 a = end - start;
		a.normalizeSafe();
		return a;
	}
};


