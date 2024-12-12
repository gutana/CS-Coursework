#pragma once

#include <string>

#include "GetGlut.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/TextureManager.h"
#include "ObjLibrary/TextureBmp.h"

#include "Drawable.h"
#include "Heightmap.h"

using ObjLibrary::Vector3;
using ObjLibrary::DisplayList;

#define RADTODEG(x) x * 180 / 3.14159265358979

class SphereEntity : public Drawable
{
public:

	Vector3 position;
	float radius;
	std::string filename;
	DisplayList* displayList;
	ObjLibrary::ObjModel* objModel;

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
			displayList->draw();
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
	DisplayList* displayList;
	ObjLibrary::ObjModel* objModel;


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
			displayList->draw();
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

class FishEntity : public Drawable
{
public:
	Vector3 position;
	Vector3 rotation;

	float size;
	float speed;
	float acceleration;

	FishEntity(float _size, float _speed, float _acceleration, Vector3 _position, DisplayList* dl)
		: m_DisplayList(dl)
	{
		position = _position;
		size = _size;
		speed = _speed;
		acceleration = _acceleration;
	}

	void render()
	{
		glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(size, size, size);
			m_DisplayList->draw();
		glPopMatrix();
	}

private:
	DisplayList* m_DisplayList;
};

class FishSchoolEntity : public Drawable
{
public:
	Vector3 center;
	float radius;
	int numFish;
	float maxDistanceFromStart;

	DisplayList* displayList;
	ObjLibrary::ObjModel* objModel;

	float fishSize;
	float fishSpeed;
	float fishAcceleration;

	FishSchoolEntity(Vector3 c, float r, int n, float maxDist, float size, float speed, float acceleration)
		: center(c), radius(r), numFish(n), 
		  maxDistanceFromStart(maxDist), fishSize(size), fishSpeed(speed), 
		  fishAcceleration(acceleration)
	{
		displayList = nullptr;
		objModel = nullptr;
	}

	void render()
	{
		for (auto fish : m_Fish)
		{
			fish->render();
		}
	}

	void Init()
	{
		m_Fish.reserve(numFish);
		for (int i = 0; i < numFish; i++)
		{
			Vector3 pos = center + Vector3::getRandomSphereVector() * radius; 
			m_Fish.push_back(new FishEntity(fishSize, fishSpeed,
				fishAcceleration, pos, displayList));
		}
	}

private:
	std::vector<FishEntity*> m_Fish;
};

enum TerrainType
{
	UnderWater,
	AboveWater
};

struct int2
{
	int x;
	int y;
};

class Terrain 
{
public:
	HeightMap hMapUnderWater;
	HeightMap hMapAboveWater;
	float heights[129][129];

	DisplayList* m_AlgaeDL;
	ObjLibrary::TextureBmp* m_HeightMapImage;
	std::vector<int2> plantPositions;

	Terrain( ObjLibrary::TextureBmp* hMapImg, Vector3 center, float _minHeight, float _maxHeight, DisplayList* algaeDl)
		: hMapUnderWater(hMapImg, center, _minHeight, _maxHeight),
		  hMapAboveWater(hMapImg, center, _minHeight, _maxHeight)
	{
		m_AlgaeDL = algaeDl;
		m_HeightMapImage = hMapImg; 
	}

	void Init(const std::string& underWaterTexFile, const std::string& aboveWaterTexFile, float texOff, float texRepeat)
	{
		hMapUnderWater.initDisplayList(underWaterTexFile, texOff, texRepeat);
		hMapAboveWater.initDisplayList(aboveWaterTexFile, texOff, texRepeat);

		int verts = hMapUnderWater.getSizeCells() + 1;
		for (int i = 0; i < verts; i++)
		{
			for (int j = 0; j < verts; j++)
			{
				heights[i][j] = hMapUnderWater.getHeight(i, j);
			}
		}

		for (int i = 0; i < verts; i++)
		{
			for (int j = 0; j < verts; j++)
			{
				unsigned char green = m_HeightMapImage->getGreen(i, j);
				if (green >= 64)
				{
					plantPositions.push_back({ i, j });
				}
			}
		}

	}

	void render(TerrainType type)
	{
		// draw underwater one no matter what
		glEnable(GL_FOG);
		hMapUnderWater.draw();
		if (type == TerrainType::AboveWater) // if above water, also draw the grass
		{
			glDisable(GL_FOG);
			hMapAboveWater.draw();
		}

		if (m_AllAlgaeDisplayList == nullptr)
		{
		
			m_AllAlgaeDisplayList = new DisplayList();
			m_AllAlgaeDisplayList->begin();

			// draw algae at vertices
			const int numVerts = hMapUnderWater.getSizeCells();
			for (int i = 0; i <= numVerts; i++)
			{
				for (int j = 0; j <= numVerts; j++)
				{
					unsigned char green = m_HeightMapImage->getGreen(i, j);
					if (green < 64) continue;
	
					glPushMatrix();
					float x = i - 64;
					float z = j - 64;
					
					float y = heights[i][j];
	
					glTranslatef(x, y, z);
					
					glEnable(GL_FOG);
					m_AlgaeDL->draw();
					glDisable(GL_FOG);
	
					glPopMatrix();
				}
			}
			m_AllAlgaeDisplayList->end();

		}
		else
		{
			m_AllAlgaeDisplayList->draw();
		}
	}
	
	Terrain(){}

private:
	DisplayList* m_AllAlgaeDisplayList = nullptr;
};
