#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include "ObjLibrary/ObjModel.h"

struct FishMovementStats
{
	float radius;
	float speed;
	float acceleration;
};

std::map<std::string, FishMovementStats> stringToFishStatsMap = {
	{"moonfish.obj",         {0.7f, 4.0f, 1.0f}},
	{"sturgeon.obj",         {0.4f, 0.3f, 0.5f}},
	{"dolphinfish.obj",      {0.25f, 18.0f, 15.0f}},
	{"salmon.obj",           {0.18f, 3.0f, 1.5f}},
	{"freshwater-drum.obj",  {0.12f, 0.75f, 0.8f}},
	{"rainbow_trout.obj",    {0.1f, 1.0f, 1.2f}},
	{"yellow-tang.obj",      {0.08f, 0.07f, 0.1f}},
	{"anchovy.obj",          {0.04f, 0.6f, 0.4f}},
	{"clownfish.obj",        {0.03f, 1.7f, 2.5f}}
};


using ObjLibrary::Vector3;

class SphereConfig
{
public:
	Vector3 center;
	float radius;
	std::string filename;
};

class CylinderConfig
{
public:
	Vector3 start;
	Vector3 end; 
	float radius;
	std::string filename; 
};

class TerrainConfig
{
public:
	Vector3 minimumCoordinates;
	Vector3 dimensions; 
	std::string filename;
};

class FishSchoolConfig
{
public:
	Vector3 center;
	float containerRadius;
	int numberOfFish;
	float maxDistanceFromStart;
	std::string filename; 

	float fishSize;
	float fishSpeed;
	float fishAcceleration;
};

class LoadInstruction
{
public:
	Vector3 playerStartingPosition;
	Vector3 playerStartingForward;
	Vector3 playerStartingUp;

	TerrainConfig terrainConfig;

	uint32_t numberOfGameObjects = 0; // spheres, cylinders, schools 

	std::vector<SphereConfig> sphereConfiguations;
	std::vector<CylinderConfig> cylinderConfigurations;
	std::vector<FishSchoolConfig> fishSchoolConfigurations;

};

class FileParser
{
public:
	static void GetLoadInstructions(LoadInstruction& out_instructions, std::string filepath)
	{
		std::ifstream mapFile(filepath);
		
		std::string line;
		while (std::getline(mapFile, line))
		{
			if (line.length() == 0 || line[0] == '#')
				continue;
			
			std::istringstream iss(line);
			std::string word;

			char entityType;

			iss >> entityType;

			switch (entityType)
			{
			case 't': // assignment 2
			{
				iss >> out_instructions.terrainConfig.minimumCoordinates
					>> out_instructions.terrainConfig.dimensions
					>> out_instructions.terrainConfig.filename;
				break;
			}
			case 'f': // assignment 2
			{
				FishSchoolConfig school;
				out_instructions.numberOfGameObjects++;

				iss >> school.center
					>> school.containerRadius
					>> school.numberOfFish
					>> school.maxDistanceFromStart
					>> school.filename;
				
				school.fishSize = stringToFishStatsMap[school.filename].radius;
				school.fishSpeed = stringToFishStatsMap[school.filename].speed;
				school.fishAcceleration = stringToFishStatsMap[school.filename].acceleration;

				out_instructions.fishSchoolConfigurations.push_back(school);
				break;
			}
			case 'p':
			{
				iss >> out_instructions.playerStartingPosition.x
					>> out_instructions.playerStartingPosition.y
					>> out_instructions.playerStartingPosition.z

					>> out_instructions.playerStartingForward.x
					>> out_instructions.playerStartingForward.y
					>> out_instructions.playerStartingForward.z

					>> out_instructions.playerStartingUp.x
					>> out_instructions.playerStartingUp.y
					>> out_instructions.playerStartingUp.z;

				break;
			}
			case 's':
			{
				out_instructions.numberOfGameObjects++;
				SphereConfig sphere;

				iss >> sphere.center.x >> sphere.center.y >> sphere.center.z 
					>> sphere.radius 
					>> sphere.filename;

				out_instructions.sphereConfiguations.push_back(sphere);
				break;
			}
			case 'c':
			{
				out_instructions.numberOfGameObjects++;
				CylinderConfig cylinder;

				iss >> cylinder.start.x >> cylinder.start.y >> cylinder.start.z
					>> cylinder.end.x >> cylinder.end.y >> cylinder.end.z
					>> cylinder.radius
					>> cylinder.filename;
				
				out_instructions.cylinderConfigurations.push_back(cylinder);
				break;
			}
			}
		}
	}
};
