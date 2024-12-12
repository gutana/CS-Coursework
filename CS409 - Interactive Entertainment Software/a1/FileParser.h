#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "ObjLibrary/ObjModel.h"

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


class LoadInstruction
{
public:
	Vector3 playerStartingPosition;
	Vector3 playerStartingForward;
	Vector3 playerStartingUp;

	std::vector<SphereConfig> sphereConfiguations;
	std::vector<CylinderConfig> cylinderConfigurations;

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
				break;
			}
			case 'f': // assignment 2
			{
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
				SphereConfig sphere;

				iss >> sphere.center.x >> sphere.center.y >> sphere.center.z 
					>> sphere.radius 
					>> sphere.filename;

				out_instructions.sphereConfiguations.push_back(sphere);
				break;
			}
			case 'c':
			{
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
