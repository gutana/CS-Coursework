//
//  main.cpp
//

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <string>

#include "GetGlut.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/SpriteFont.h"
#include "ObjLibrary/TextureManager.h"
#include "Sleep.h"

#include "Camera.h"
#include "CylinderEntity.h"
#include "Entity.h"
#include "FileParser.h"
#include "FishEntity.h"
#include "FishSchoolEntity.h"
#include "InputManager.h"
#include "Physics.h"
#include "SphereEntity.h"
#include "SurfaceNormal.h"
#include "Terrain.h"
#include "TimeManager.h"
#include "Transform.h"

using namespace ObjLibrary; 

#define SHOW_HMAP_TRIANGLES

unsigned int g_WindowWidth = 768;
unsigned int g_WindowHeight = 1024;

const int UPDATES_PER_SECOND = 60;
const int MIN_SMOOTH_DURATION = 5;
const unsigned int MAX_UPDATES_PER_FRAME = 10;

float g_MovementSpeed = 5;
float g_LookSpeed = 3;
float g_RollSpeed = 3;

unsigned int g_FishCaught = 0;

bool g_ShowFpsOverlay = false;
bool g_ShowControls = false;
bool g_GamePaused = false;

bool g_RunPhysicsSlowly = false;
bool g_RunDisplaySlowly = false;

bool g_ShowSchoolDebugInformation = false;
bool g_ShowFixedEntityNormals = false;
bool g_ShowTerrainNormals = false;

void init();
void initDisplay();
void update();
void reshape(int w, int h);
void display();
void drawUI();

TimeManager* g_TimeManager = nullptr;

std::vector<Drawable*> g_DrawableObjects;
std::vector<FishSchoolEntity*> g_FishSchools;
std::vector<PhysicsBody*>  g_PhysicsBodies;
std::vector<FixedEntity*> g_FixedEntities;
std::map<std::string, DisplayList> g_DisplayListMap;

Camera g_Camera;
Terrain g_Terrain;

ObjModel g_WaterSurface;
DisplayList g_WaterSurfaceDL;

ObjModel g_Skybox;
DisplayList g_SkyboxDL;

ObjModel g_Algae;
DisplayList g_AlgaeDL;

SpriteFont g_Font;
uint32_t g_GreyTextureId; // grey bg for the fps overlay

FishSchoolEntity* lastNearestSchoolLookedAt = nullptr;
FixedEntity* lastFixedEntityLookedAt = nullptr;

int main(int argc, char* argv[])
{
	glutInitWindowSize(g_WindowHeight, g_WindowWidth);
	glutInitWindowPosition(10, 10);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Assignment 4 - CS 409");

	// get the glut callbacks going to the input manager
	glutKeyboardFunc(InputManager::keyboardDown);
	glutKeyboardUpFunc(InputManager::keyboardUp);

	glutSpecialFunc(InputManager::specialDown);
	glutSpecialUpFunc(InputManager::specialUp);

	// passing in these lambdas is highly suspect,
	// but this scope will live forever so it's probably fine
	InputManager::RegisterKeyDownCallback(ESC, []() { exit(0); });

	InputManager::RegisterKeyDownCallback('1', []() { g_ShowFpsOverlay = !g_ShowFpsOverlay; });

	InputManager::RegisterKeyDownCallback('2', []() 
	{
		g_ShowSchoolDebugInformation = !g_ShowSchoolDebugInformation;

		if (lastNearestSchoolLookedAt == nullptr) return;

		if (g_ShowSchoolDebugInformation)
			lastNearestSchoolLookedAt->showDebugInformation = true;
		else
			lastNearestSchoolLookedAt->showDebugInformation = false;

	});

	InputManager::RegisterKeyDownCallback('4', []() { g_ShowTerrainNormals = !g_ShowTerrainNormals; });

	InputManager::RegisterKeyDownCallback('3', []() { g_ShowFixedEntityNormals = !g_ShowFixedEntityNormals;	});

	InputManager::RegisterKeyDownCallback('p', []() 
	{
		if (g_GamePaused)
		{
			g_TimeManager->adjustNextUpdateTimeToNow();
		}
		g_GamePaused = !g_GamePaused;
	});

	// TODO maybe have little window you can toggle that shows all controls
	// InputManager::RegisterKeyDownCallback('c', []() { g_ShowControls = !g_ShowControls; });

	glutIdleFunc(update);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	init();
	glutMainLoop();  // contains an infinite loop, so it never returns
	return 1;
}

void init ()
{
	initDisplay();

	g_Font.load("Resources/Font.bmp");

	TextureBmp texBmp("grey.bmp");
	g_GreyTextureId = texBmp.addToOpenGL();

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1f);
    float fog_color[4] = {     0.0f, 0.5f, 1.0f, 0.0f     };
    glFogfv(GL_FOG_COLOR, fog_color);
	// transparency on anemone
	TextureManager::load("Resources/anemone.bmp",
		GL_CLAMP, GL_CLAMP,
		GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST,
		Vector3(0, 0, 0));

	// load scene details
	LoadInstruction loadInstructions;
	FileParser::GetLoadInstructions(loadInstructions, "Resources/map.txt");

	g_DrawableObjects.reserve(loadInstructions.numberOfGameObjects);
	
	g_Camera = Camera(loadInstructions.playerStartingPosition,
		loadInstructions.playerStartingForward,
		loadInstructions.playerStartingUp);

	for (int i = 0; i < loadInstructions.sphereConfiguations.size(); i++)

	{
		SphereEntity* sphere = new SphereEntity(loadInstructions.sphereConfiguations[i].center, 
			loadInstructions.sphereConfiguations[i].radius, 
			loadInstructions.sphereConfiguations[i].filename);

		auto it = g_DisplayListMap.find(loadInstructions.sphereConfiguations[i].filename);
		// if already in our displaylist map
		if (it != g_DisplayListMap.end())
		{
			sphere->displayList = &it->second;
		}
		// if not in display list map
		else
		{
			sphere->objModel = new ObjModel();
			sphere->objModel->load("Resources/" + sphere->filename);
			g_DisplayListMap[sphere->filename] = sphere->objModel->getDisplayList();
			sphere->displayList = &g_DisplayListMap[sphere->filename];
		}
		g_DrawableObjects.push_back(sphere);
		g_FixedEntities.push_back(sphere);
	}
	for (int i = 0; i < loadInstructions.cylinderConfigurations.size(); i++)
	{
		CylinderEntity* cylinder = new CylinderEntity(loadInstructions.cylinderConfigurations[i].start, 
			loadInstructions.cylinderConfigurations[i].end, 
			loadInstructions.cylinderConfigurations[i].radius,
			loadInstructions.cylinderConfigurations[i].filename);

		auto it = g_DisplayListMap.find(loadInstructions.cylinderConfigurations[i].filename);
		if (it != g_DisplayListMap.end())
		{
			cylinder->displayList = &it->second;
		}
		// if not in display list map
		else
		{
			cylinder->objModel = new ObjModel();
			cylinder->objModel->load("Resources/" + cylinder->filename);
			g_DisplayListMap[cylinder->filename] = cylinder->objModel->getDisplayList();
			cylinder->displayList = &g_DisplayListMap[cylinder->filename];
		}
		g_DrawableObjects.push_back(cylinder);
		cylinder->InitNormalsDL();
		g_FixedEntities.push_back(cylinder);
	}
	for (int i = 0; i < loadInstructions.fishSchoolConfigurations.size(); i++)
	{
		FishSchoolEntity* school = new FishSchoolEntity(loadInstructions.fishSchoolConfigurations[i].center,
			loadInstructions.fishSchoolConfigurations[i].containerRadius,
			loadInstructions.fishSchoolConfigurations[i].numberOfFish,
			loadInstructions.fishSchoolConfigurations[i].maxDistanceFromStart,
			loadInstructions.fishSchoolConfigurations[i].fishSize,
			loadInstructions.fishSchoolConfigurations[i].fishSpeed,
			loadInstructions.fishSchoolConfigurations[i].fishAcceleration);

		auto it = g_DisplayListMap.find(loadInstructions.fishSchoolConfigurations[i].filename);
		if (it != g_DisplayListMap.end())
		{
			school->displayList = &it->second;
		}
		else
		{
			school->objModel = new ObjModel();
			school->objModel->load("Resources/" + loadInstructions.fishSchoolConfigurations[i].filename);
			g_DisplayListMap[loadInstructions.fishSchoolConfigurations[i].filename] = school->objModel->getDisplayList();
			school->displayList = &g_DisplayListMap[loadInstructions.fishSchoolConfigurations[i].filename];
		}
		school->Init();
		g_FishSchools.push_back(school);
		for (int i = 0; i < school->children.size(); i++)
		{
			g_PhysicsBodies.push_back(school->children[i]);
		}
		g_DrawableObjects.push_back(school);
	}

	g_WaterSurface.load("Resources/surface.obj");
	g_WaterSurfaceDL = g_WaterSurface.getDisplayList();

	g_Skybox.load("Resources/Skybox.obj");
	g_SkyboxDL = g_Skybox.getDisplayList();

	TextureManager::load("Resources/green_algae2.bmp",
                     GL_CLAMP, GL_CLAMP,
                     GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST,
                     Vector3(0.0, 0.0, 0.0));

	g_Algae.load("Resources/algae.obj");
	g_AlgaeDL = g_Algae.getDisplayList();

	ObjLibrary::TextureBmp* terrainHMap = new ObjLibrary::TextureBmp("Resources/heightmap.bmp");

	g_Terrain = Terrain(terrainHMap, loadInstructions.terrainConfig.minimumCoordinates, loadInstructions.terrainConfig.dimensions, &g_AlgaeDL);

#ifdef SHOW_HMAP_TRIANGLES
	g_Terrain.Init("Resources/triangles.bmp", "Resources/grass1.bmp", 0, 64);
#else
	g_Terrain.Init("Resources/dirt2.bmp", "Resources/grass1.bmp", 0, 10);
#endif

	g_TimeManager = new TimeManager(UPDATES_PER_SECOND, MIN_SMOOTH_DURATION, MAX_UPDATES_PER_FRAME);
}

void initDisplay ()
{
	glColor3f(0.0, 0.0, 0.0);
			0.f,

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutPostRedisplay();
}

void updateNearestSchool();

const float PLAYER_ACCELERATION = 0.5f;
const Vector3 GRAVITY = { 0, -0.1, 0 };

void doAllPhysicsUpdates(double dt)
{
	if (g_Camera.transform.position.y < 0)
	{
		Vector2 moveVec = InputManager::GetCurrentMoveVectorNormalized();
		float cameraUpMovement = InputManager::GetCurrentClimbFloatNormalized();
		float forwardMovement = moveVec.y;
		float strafeMovement = moveVec.x;
		
		Vector3 acceleration;
		acceleration += g_Camera.transform.forward * forwardMovement * PLAYER_ACCELERATION;
		acceleration += g_Camera.transform.right * strafeMovement * PLAYER_ACCELERATION;
		acceleration += g_Camera.transform.up * cameraUpMovement * PLAYER_ACCELERATION;

		g_Camera.velocity += acceleration;


		if (std::abs(g_Camera.velocity.getDistance({0, 0, 0})) > g_MovementSpeed)
		{
			g_Camera.velocity.normalize();
			g_Camera.velocity *= g_MovementSpeed;
		}
	}
	else
	{
		g_Camera.velocity += GRAVITY;
	}
	g_Camera.updatePosition(dt, 0.2);


	for (auto body : g_PhysicsBodies)
	{
		body->Update(dt);
	}

	// COLLISIONS 

	// collide player, fish with fixed entities
	for (auto fixedEntity : g_FixedEntities)
	{
		switch (fixedEntity->entityType)
		{
		case Sphere:
			// PLAYER - SPHERE COLLISION 
		{
			if (Physics::CheckSphereSphereCollision(g_Camera.transform.position, g_Camera.transform.scale.x,
				fixedEntity->transform.position, fixedEntity->transform.scale.x))
			{
				Vector3 normal = SurfaceNormal::getSphere(g_Camera.transform.position, fixedEntity->transform.position);
				Physics::BounceEntityOffNormal(&g_Camera, normal, 0.7f);
				// Keep a minimum distance at all times, so player can't go inside by forcing their way through
				Physics::EnforceMinimumDistance(&g_Camera, fixedEntity, normal);
			}
		}
		// FISH - SPHERE COLLISION 
		{
			for (auto fishSchool : g_FishSchools)
			{
				// Check if the entity overlaps with the school
				// If so, we do the physics checks for every fish in that school.
				if (Physics::CheckSphereSphereCollision(fishSchool->transform.position, fishSchool->transform.scale.x,
					fixedEntity->transform.position, fixedEntity->transform.scale.x))
				{
					for (auto fish : fishSchool->children)
					{
						if (Physics::CheckSphereSphereCollision(fish->transform.position, fish->transform.scale.x,
							fixedEntity->transform.position, fixedEntity->transform.scale.x))
						{
							Vector3 normal = SurfaceNormal::getSphere(fish->transform.position, fixedEntity->transform.position);
							Physics::BounceEntityOffNormal(fish, normal);
							Physics::EnforceMinimumDistance(fish, fixedEntity, normal);
						}
					}
				}
			}
		}
		break;
		case Cylinder:
			// PLAYER - CYLINDER COLLISION
		{
			CylinderEntity* cylinder = (CylinderEntity*)fixedEntity;
			Vector3 rejection;
			if (Physics::CheckEntityCylinderCollision(&g_Camera, cylinder, rejection))
			{
				Physics::BounceEntityOffNormal(&g_Camera, rejection.getNormalized(), 0.7f);
				// not sure why we need this 0.2f extra padding, but it looks like we do
				Physics::EnforceMinimumDistance(&g_Camera, (g_Camera.transform.position - rejection), rejection.getNormalized(), cylinder->radius + 0.2f);
			}
		}
		// FISH - CYLINDER COLLISION
		{
			for (auto fishSchool : g_FishSchools)
			{
				// Check if the entity overlaps with the school
				// If so, we do the physics checks for every fish in that school.
				Vector3 rejection;
				if (Physics::CheckEntityCylinderCollision((Entity*)fishSchool, (CylinderEntity*)fixedEntity, rejection))
				{
					for (auto fish : fishSchool->children)
					{
						if (Physics::CheckEntityCylinderCollision((Entity*)fish, (CylinderEntity*)fixedEntity, rejection))
						{
							Physics::BounceEntityOffNormal(fish, rejection.getNormalized());
						}
					}
				}
			}

		}
		break;
		}

	}

	// Collide fish with Terrain
	for (auto school : g_FishSchools)
	{
		if ((school->transform.position.y - school->transform.scale.y) < g_Terrain.hMapUnderWater.getHeight(school->transform.position))
		{
			for (auto fish : school->children)
			{
				if ((fish->transform.position.y - fish->transform.scale.y) < g_Terrain.hMapUnderWater.getHeight(fish->transform.position))
				{
					Vector3 normal = g_Terrain.hMapUnderWater.getSurfaceNormal(fish->transform.position.x, fish->transform.position.z, g_Terrain.m_Scale);
					Physics::BounceEntityOffNormal(fish, normal);
				}
			}
		}
	}

	// Collide player with Terrain
	double currentHmapHeight = g_Terrain.hMapUnderWater.getHeight(g_Camera.transform.position);
	if ((g_Camera.transform.position.y - g_Camera.transform.scale.y) < currentHmapHeight)
	{
		Vector3 normal = g_Terrain.hMapUnderWater.getSurfaceNormal(g_Camera.transform.position.x, g_Camera.transform.position.z, g_Terrain.m_Scale);
		Physics::BounceEntityOffNormal(&g_Camera, normal, 0.7f);
		Physics::EnforceMinimumDistance(&g_Camera, 
			// height at cameras position
			{ g_Camera.transform.position.x, currentHmapHeight, g_Camera.transform.position.z },
			normal, g_Camera.transform.scale.x);
	}

	// Fish school stuff: collision, finding nearest, updating count.
	updateNearestSchool(); // this is a physics update since we use this to check for player-fish collisions

	if (lastNearestSchoolLookedAt != nullptr)
	{
		if (Physics::CheckSphereSphereCollision(g_Camera.transform.position, g_Camera.transform.scale.x,
			lastNearestSchoolLookedAt->transform.position,
			lastNearestSchoolLookedAt->transform.scale.x))
		{

			lastNearestSchoolLookedAt->playerInsideSchool = true;
			std::vector<FishEntity*> fishCaught = lastNearestSchoolLookedAt->GetFishCollidingWithEntity(&g_Camera);

			for (auto fish : fishCaught)
			{
				std::cout << "Fish caught: " << ++g_FishCaught << '\n';

				// Remove the caught fish from physicsbodies and from the school's children
				auto it = std::find(g_PhysicsBodies.begin(), g_PhysicsBodies.end(), fish);
				if (it != g_PhysicsBodies.end())
				{
					g_PhysicsBodies.erase(it);
				}

				auto it2 = std::find(lastNearestSchoolLookedAt->children.begin(), lastNearestSchoolLookedAt->children.end(), fish);
				if (it2 != lastNearestSchoolLookedAt->children.end())
				{
					lastNearestSchoolLookedAt->children.erase(it2);
				}
	
				delete(fish);
			}

			if (lastNearestSchoolLookedAt->children.size() == 0)
			{
				auto it = std::find(g_FishSchools.begin(), g_FishSchools.end(), lastNearestSchoolLookedAt);
				if (it != g_FishSchools.end())
				{
					g_FishSchools.erase(it);
				}
				
				auto it2 = std::find(g_DrawableObjects.begin(), g_DrawableObjects.end(), lastNearestSchoolLookedAt);
				if (it2 != g_DrawableObjects.end())
				{
					g_DrawableObjects.erase(it2);
				}
				delete(lastNearestSchoolLookedAt);
				lastNearestSchoolLookedAt = nullptr;
				// todo: clean up the display list, since i think all fish schools are unique species?
			}

		} 
		else
		{
			lastNearestSchoolLookedAt->playerInsideSchool = false;
		}
	}
}

FishSchoolEntity* findNearestSchoolToPosition(const Vector3& pos, const std::vector<FishSchoolEntity*>& objects)
{
	if (objects.size() == 0) throw "No schools passed to findNearestSchoolToPosition";

	// set element 0 as nearest no matter what, since it's the only one we've seen 
	FishSchoolEntity* nearest = objects[0];
	float nearestDistance = pos.getDistance(nearest->transform.position) - nearest->transform.scale.x;

	// start at element 1 since 0 is our nearest 
	for (int i = 1; i < objects.size(); i++)
	{
		FishSchoolEntity* current = objects[i];
		// Subtract the radius of the schools sphere from the distance calculation so that
		// we get the distance to the edge of the closest sphere, instead of the center
		float distanceToCurrent = pos.getDistance(current->transform.position) - current->transform.scale.x;

		if (distanceToCurrent < nearestDistance || nearest == nullptr)
		{
			nearest = current;
			nearestDistance = distanceToCurrent;
		}
	}

	return nearest;

}

FixedEntity* findNearestFixedEntityToPosition(const Vector3& pos, const std::vector<FixedEntity*>& objects)
{
	if (objects.size() == 0) throw "No Fixed entities passed to findNearestFixedEntityToPos";

	// set element 0 as nearest no matter what, since it's the only one we've seen 
	FixedEntity* nearest = objects[0];
	float nearestDistance = pos.getDistance(nearest->transform.position) - nearest->transform.scale.x;

	// start at element 1 since 0 is our nearest 
	for (int i = 1; i < objects.size(); i++)
	{
		FixedEntity* current = objects[i];
		// Subtract the radius of the schools sphere from the distance calculation so that
		// we get the distance to the edge of the closest sphere, instead of the center
		float distanceToCurrent = pos.getDistance(current->transform.position) - current->transform.scale.x;

		if (distanceToCurrent < nearestDistance || nearest == nullptr)
		{
			nearest = current;
			nearestDistance = distanceToCurrent;
		}
	}
	return nearest;
}

void updateNearestSchool()
{

	FishSchoolEntity* nearestSchool = findNearestSchoolToPosition(g_Camera.transform.position, g_FishSchools);

	if (nearestSchool == nullptr) throw "Unable to find nearest school.";

	if (lastNearestSchoolLookedAt == nullptr)
	{
		lastNearestSchoolLookedAt = nearestSchool;
		if (g_ShowSchoolDebugInformation)
			nearestSchool->showDebugInformation = true;
	}

	float distanceToNearestSchool = nearestSchool->transform.position.getDistance(g_Camera.transform.position) - nearestSchool->transform.scale.x;
	float distanceToLastNearestSchool = lastNearestSchoolLookedAt->transform.position.getDistance(g_Camera.transform.position) - lastNearestSchoolLookedAt->transform.scale.x;

	if (distanceToNearestSchool < distanceToLastNearestSchool)
	{
		if (lastNearestSchoolLookedAt != nullptr)
		{
			lastNearestSchoolLookedAt->showDebugInformation = false;
		}
		lastNearestSchoolLookedAt = nearestSchool;
		nearestSchool->showDebugInformation = true;
	}
}

void updateNearestFixedEntity()
{

	FixedEntity* nearestFEntity = findNearestFixedEntityToPosition(g_Camera.transform.position, g_FixedEntities);

	if (nearestFEntity == nullptr) throw "Unable to find nearest school.";

	if (lastFixedEntityLookedAt == nullptr)
	{
		lastFixedEntityLookedAt = nearestFEntity;
		nearestFEntity->ShowNormals = true;
	}

	float distanceToNearestFE = nearestFEntity->transform.position.getDistance(g_Camera.transform.position);
	float distanceToLastNearestFE = lastFixedEntityLookedAt->transform.position.getDistance(g_Camera.transform.position);

	if (distanceToNearestFE < distanceToLastNearestFE)
	{
		if (lastFixedEntityLookedAt != nullptr)
		{
			lastFixedEntityLookedAt->ShowNormals = false;
		}
		lastFixedEntityLookedAt = nearestFEntity;
		nearestFEntity->ShowNormals = true;
	}
}


void doUpdates()
{
	const double dt = g_TimeManager->getUpdateDeltaTime();
	doAllPhysicsUpdates(dt);

	float rollAmount = InputManager::GetCurrentRollFloatNormalized();
	Vector2 cameraLookVector = InputManager::GetCurrentLookVectorNormalized();
	bool HPressed = InputManager::GetIsPressed('h');

	// looking up/down
	g_Camera.transform.rotateAroundRight(g_LookSpeed * cameraLookVector.y * dt);
	// looking left/right
	g_Camera.transform.rotateAroundUp(g_LookSpeed * -cameraLookVector.x * dt);
	// roll
	g_Camera.transform.rotateAroundForward(g_RollSpeed * rollAmount * dt);

	// rotate to origin
	if (HPressed)
		g_Camera.transform.rotateTowardPosition({0,  0, 0}, g_LookSpeed * dt);

	// If we had debug on and turned it off, disable wireframe and reset things
	if (!g_ShowSchoolDebugInformation && lastNearestSchoolLookedAt != nullptr)
	{
		lastNearestSchoolLookedAt->showDebugInformation = false;
		// lastNearestSchoolLookedAt = nullptr;
	}
	

	if (!g_ShowFixedEntityNormals && lastFixedEntityLookedAt != nullptr)
	{
		lastFixedEntityLookedAt->ShowNormals = false;
		lastFixedEntityLookedAt = nullptr;
	}
	if (g_ShowFixedEntityNormals)
		updateNearestFixedEntity();

}

void update ()
{
	if (g_GamePaused)
	{
		glutPostRedisplay();
		return; 
	}

	if (InputManager::GetIsPressed('u'))
		sleep(0.08);

	for (uint32_t i = 0; i < MAX_UPDATES_PER_FRAME && g_TimeManager->isUpdateWaiting(); i++)
	{
		g_TimeManager->markNextUpdate();
		doUpdates();
	}
	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

	g_WindowWidth = w;
	g_WindowHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

struct Color
{
	Color(float _r, float _g, float _b, float _a) { r = _r; g = _g, b = _b, a = _a; }
	float r;
	float g;
	float b;
	float a;
};

Color lightBlue(0, 0.75, 1, 1.0);

void display ()
{
	if (InputManager::GetIsPressed('y'))
		sleep(0.08);

	g_TimeManager->markNextFrame();
	bool isUnderwater = g_Camera.transform.position.y < 0;

	if (isUnderwater)
	{
		// gradually fade to  darkness as we go down
		float depth = g_Camera.transform.position.y;
		float depthScalar = g_Camera.transform.position.y / 30;

		float color[4] = { std::max(lightBlue.r + depthScalar, 0.0f),
						   std::max(lightBlue.g + depthScalar, 0.0f), 
						   std::max(lightBlue.b + depthScalar, 0.5f), 
						   lightBlue.a
		};

		glClearColor(color[0], color[1], color[2], color[3]);
		glFogfv(GL_FOG_COLOR, color);
	}
	else
	{
		glClearColor(1, 1, 1, 0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display at all

	glLoadIdentity();

	Vector3 lookAt = g_Camera.transform.position + g_Camera.transform.forward;

	gluLookAt(g_Camera.transform.position.x,
			  g_Camera.transform.position.y, 
		      g_Camera.transform.position.z,  // (x, y, z) camera eye position
	          lookAt.x,
			  lookAt.y,
		      lookAt.z,  // (x, y, z) camera look at position
	          g_Camera.transform.up.x,		  
			  g_Camera.transform.up.y,       
		      g_Camera.transform.up.z); // (x, y, z) camera up direction

	if (!isUnderwater) // Draw skybox
	{
		glPushMatrix();
			glDepthMask(GL_FALSE);
			glTranslatef(g_Camera.transform.position.x, g_Camera.transform.position.y, g_Camera.transform.position.z);
			g_SkyboxDL.draw();
			glDepthMask(GL_TRUE);
		glPopMatrix();
	}

	glEnable(GL_FOG);

	// All of the objects in map.txt
	for (auto drawable : g_DrawableObjects)
	{
		drawable->render();
	}

	TerrainType tType = isUnderwater ? TerrainType::UnderWater : TerrainType::AboveWater;

	if (!isUnderwater)
		glDisable(GL_FOG);
	else
		glEnable(GL_FOG);

	g_Terrain.render(tType, g_ShowTerrainNormals, g_Camera.transform.position, 10);

	if (g_Camera.transform.position.y < 0)
		glEnable(GL_FOG);

	g_WaterSurfaceDL.draw();

	drawUI();

	glutSwapBuffers();

	unsigned int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error after main draw loop: " << err << '\n';
	}
}

int depthMeterMargin = 10;
int fishCaughtCounterOffsetFromCenter = 200;

void drawUI() // todo: line things up better 
{
	SpriteFont::setUp2dView(g_WindowWidth, g_WindowHeight);

	// only show depth indicator when underwater
	if (g_Camera.transform.position.y < 0)
	{
		float depth = -g_Camera.transform.position.y;

		std::stringstream ss;
		ss << "Depth: " << std::setprecision(0) << std::fixed << std::setw(2) << depth << 'm';

		g_Font.draw(ss.str(), depthMeterMargin, g_WindowHeight - (g_Font.getHeight() + depthMeterMargin), SpriteFont::BOLD);
	}

	{
		std::stringstream ss;
		ss << "Fish caught: " << g_FishCaught;
		g_Font.draw(ss.str(), g_WindowWidth / 2 - g_Font.getWidth(ss.str()) / 2, (g_WindowHeight / (float)2) + fishCaughtCounterOffsetFromCenter, SpriteFont::BOLD);
	}

	if (g_GamePaused)
	{
		int screenMiddleWidth = g_WindowWidth / 2;
		int screenMiddleHeight = g_WindowHeight / 2;
		int fontCharWidth = g_Font.getWidth('P');

		int boxWidth = 150;
		int boxHeight = 80;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, g_GreyTextureId);
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex2i(screenMiddleWidth - boxWidth / 2, screenMiddleHeight - boxHeight / 2);
			glTexCoord2f(0, 1);
			glVertex2i(screenMiddleWidth - boxWidth / 2,  screenMiddleHeight + boxHeight / 2);
			glTexCoord2f(1, 1);
			glVertex2i(screenMiddleWidth + boxWidth / 2, screenMiddleHeight + boxHeight / 2);
			glTexCoord2f(1, 0);
			glVertex2i(screenMiddleWidth + boxWidth / 2, screenMiddleHeight - boxHeight / 2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		g_Font.draw("PAUSED", screenMiddleWidth - (fontCharWidth * 3 + 3), screenMiddleHeight - g_Font.getHeight() / 2);
		g_Font.draw("[P] Unpause", g_WindowWidth - 120, 16);
	}
	else
	{
		g_Font.draw("[P] Pause", g_WindowWidth - 120, 16);
	}

	if (g_ShowSchoolDebugInformation)
	{
		g_Font.draw("[2] Disable School Debug", 360, 16);
	}
	else
	{
		g_Font.draw("[2] Enable School Debug", 360, 16);
	}

	if (g_ShowFixedEntityNormals)
	{
		g_Font.draw("[3] Disable Fixed Entity Normals", 360, 40);
	}
	else
	{
		g_Font.draw("[3] Enable Fixed Entity Normals", 360, 40);
	}

	if (g_ShowTerrainNormals)
	{
		g_Font.draw("[4] Disable Terrain Normals", 360, 64);
	}
	else
	{
		g_Font.draw("[4] Enable Terrain Normals", 360, 64);
	}



	if (g_ShowFpsOverlay)
	{

		glColor3ub(255, 255, 255);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_GreyTextureId);
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex2i(5, 5);
			glTexCoord2f(0, 1);
			glVertex2i(5, 265);
			glTexCoord2f(1, 1);
			glVertex2i(350, 265);
			glTexCoord2f(1, 0);
			glVertex2i(350, 5);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		std::stringstream ss0;
		ss0 << "Frame count: " << std::setprecision(0) << std::fixed << std::setw(28)  << g_TimeManager->getFrameCount();
		g_Font.draw(ss0.str(), 16, 48);
	
		std::stringstream ss1;
		ss1 << "Game running time: " << std::setprecision(2) << std::fixed << std::setw(18)  << g_TimeManager->getGameDuration();
		g_Font.draw(ss1.str(), 16, 16);

		std::stringstream ss2;
		ss2 << "Average frame rate: " << std::setprecision(2) << std::fixed << std::setw(18)  << g_TimeManager->getFrameRateAverage();
		g_Font.draw(ss2.str(), 16, 72);

		std::stringstream ss3;
		ss3 << "Instantaneous frame rate: " << std::setprecision(2) << std::fixed << std::setw(10)  << g_TimeManager->getFrameRateInstantaneous();
		g_Font.draw(ss3.str(), 16, 96);

		std::stringstream ss4;
		ss4 << "Smoothed frame rate: " << std::setprecision(2) << std::fixed << std::setw(16)  << g_TimeManager->getFrameRateSmoothed();
		g_Font.draw(ss4.str(), 16, 120);

		std::stringstream ss5;
		ss5 << "Update count: " << std::setprecision(0) << std::fixed << std::setw(28)  << g_TimeManager->getUpdateCount();
		g_Font.draw(ss5.str(), 16, 152);

		std::stringstream ss6;
		ss6 << "Average Update rate: " << std::setprecision(2) << std::fixed << std::setw(16)  << g_TimeManager->getUpdateRateAverage();
		g_Font.draw(ss6.str(), 16, 176);

		std::stringstream ss7;
		ss7 << "Instantaneous update rate: " << std::setprecision(2) << std::fixed << std::setw(9)  << g_TimeManager->getUpdateRateInstantaneous();
		g_Font.draw(ss7.str(), 16, 200);

		// weird stuff happens with g-sync 
		std::stringstream ss8;
		ss8 << "Smoothed update rate: " << std::setprecision(2) << std::fixed << std::setw(14)  << g_TimeManager->getUpdateRateSmoothed();
		g_Font.draw(ss8.str(), 16, 224);	


		g_Font.draw("[1] Hide Performance Stats", 16, 268);
	}
	else
	{
		g_Font.draw("[1] Performance Stats", 16, 10);
	}

	SpriteFont::unsetUp2dView();

	unsigned int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error after UI draw: " << err << '\n';
	}

}