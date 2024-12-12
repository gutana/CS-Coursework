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
#include "SphereEntity.h"
#include "Terrain.h"
#include "TimeManager.h"
#include "Transform.h"

using namespace ObjLibrary; 

unsigned int g_WindowWidth = 768;
unsigned int g_WindowHeight = 1024;

const int UPDATES_PER_SECOND = 60;
const int MIN_SMOOTH_DURATION = 5;
const unsigned int MAX_UPDATES_PER_FRAME = 10;

float g_MovementSpeed = 5;
float g_LookSpeed = 3;
float g_RollSpeed = 3;

bool g_ShowFpsOverlay = false;
bool g_GamePaused = false;
bool g_RunPhysicsSlowly = false;
bool g_RunDisplaySlowly = false;
bool g_ShowSchoolDebugInformation = false;

void init();
void initDisplay();
void update();
void reshape(int w, int h);
void display();
void drawUI();

TimeManager* g_TimeManager = nullptr;

uint32_t g_GreyTextureId;

std::vector<Drawable*> g_DrawableObjects;
std::vector<FishSchoolEntity*> g_FishShools;
std::vector<PhysicsBody*>  g_PhysicsBodies;
std::map<std::string, DisplayList> g_DisplayListMap;

SpriteFont g_Font;

ObjModel g_WaterSurface;
DisplayList g_WaterSurfaceDL;

ObjModel g_Skybox;
DisplayList g_SkyboxDL;

ObjModel g_Algae;
DisplayList g_AlgaeDL;

Terrain g_Terrain;

Camera g_Camera;

FishSchoolEntity* lastSchoolLookedAt = nullptr;

int main(int argc, char* argv[])
{
	glutInitWindowSize(g_WindowHeight, g_WindowWidth);
	glutInitWindowPosition(10, 10);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Assignment 3 - CS 409");

	glutKeyboardFunc(InputManager::keyboardDown);
	glutKeyboardUpFunc(InputManager::keyboardUp);

	glutSpecialFunc(InputManager::specialDown);
	glutSpecialUpFunc(InputManager::specialUp);

	InputManager::RegisterFpsOverlayToggleCallback([]() {
		g_ShowFpsOverlay = !g_ShowFpsOverlay;
	});

	InputManager::RegisterPauseToggleCallback([]() {
		if (g_GamePaused)
		{
			g_TimeManager->adjustNextUpdateTimeToNow();
		}
		g_GamePaused = !g_GamePaused;
	});

	InputManager::RegisterFishSchoolDebugDisplayToggleCallback([]() {
		g_ShowSchoolDebugInformation = !g_ShowSchoolDebugInformation;

		if (lastSchoolLookedAt == nullptr) return;

		if (g_ShowSchoolDebugInformation)
		{
			lastSchoolLookedAt->showDebugInformation = true;
		} 
		else
		{
			lastSchoolLookedAt->showDebugInformation = false;
		}
	});

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
		g_FishShools.push_back(school);
		for (int i = 0; i < school->children.size(); i++)
		{
			g_PhysicsBodies.push_back(school->children[i]);
		}
#ifdef ANCHOVY_SCHOOL_MOVEMENT
		g_PhysicsBodies.push_back(school);
#endif // ANCHOVY_SCHOOL_MOVEMENT

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
	g_Terrain.Init("Resources/dirt2.bmp", "Resources/grass1.bmp", 0, 10);

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

void updateNearestSchool()
{

	FishSchoolEntity* nearestSchool = findNearestSchoolToPosition(g_Camera.transform.position, g_FishShools);

	if (nearestSchool == nullptr) throw "Unable to find nearest school.";

	if (lastSchoolLookedAt == nullptr)
	{
		lastSchoolLookedAt = nearestSchool;
		nearestSchool->showDebugInformation = true;
	}

	float distanceToNearestSchool = nearestSchool->transform.position.getDistance(g_Camera.transform.position) - nearestSchool->transform.scale.x;
	float distanceToLastNearestSchool = lastSchoolLookedAt->transform.position.getDistance(g_Camera.transform.position) - lastSchoolLookedAt->transform.scale.x;

	if (distanceToNearestSchool < distanceToLastNearestSchool)
	{
		if (lastSchoolLookedAt != nullptr)
		{
			lastSchoolLookedAt->showDebugInformation = false;
		}
		lastSchoolLookedAt = nearestSchool;
		nearestSchool->showDebugInformation = true;
	}
}

void doUpdates()
{
	const double dt = g_TimeManager->getUpdateDeltaTime();
	doAllPhysicsUpdates(dt);

	float rollAmount = InputManager::GetCurrentRollFloatNormalized();
	Vector2 cameraLookVector = InputManager::GetCurrentLookVectorNormalized();
	bool HPressed = InputManager::GetIsHPressed();

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
	if (!g_ShowSchoolDebugInformation && lastSchoolLookedAt != nullptr)
	{
		lastSchoolLookedAt->showDebugInformation = false;
		lastSchoolLookedAt = nullptr;
		return;
	}

	if (g_ShowSchoolDebugInformation)
		updateNearestSchool();

}

void update ()
{
	if (g_GamePaused)
	{
		glutPostRedisplay();
		return; 
	}

	if (InputManager::GetIsUPressed())
	{
		sleep(0.08);
	}

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
	if (InputManager::GetIsYPressed())
		sleep(0.08);

	g_TimeManager->markNextFrame();
	bool isUnderwater = g_Camera.transform.position.y < 0;

	if (isUnderwater)
	{
		// gradually fade to complete darkness as we go down
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

	g_Terrain.render(tType);

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
void drawUI()
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