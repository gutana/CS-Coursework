//
//  main.cpp
//

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

#include "GetGlut.h"
#include "Sleep.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/TextureManager.h"
#include "ObjLibrary/SpriteFont.h"

#include "InputManager.h"
#include "FileParser.h"
#include "GameEntity.h"
#include "Transform.h"
#include "Camera.h"

using namespace std;
using namespace ObjLibrary; 

unsigned int g_WindowWidth = 768;
unsigned int g_WindowHeight = 1024;

float g_MovementSpeed = 0.1;
float g_LookSpeed = 0.05;
float g_RollSpeed = 0.05;

void init();
void initDisplay();
void update();
void reshape(int w, int h);
void display();
void drawUI();

std::vector<Drawable*> g_DrawableObjects;
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

int main (int argc, char* argv[])
{
	glutInitWindowSize(g_WindowHeight, g_WindowWidth);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Assignment 2 - CS 409");

	glutKeyboardFunc(InputManager::keyboardDown);
	glutKeyboardUpFunc(InputManager::keyboardUp);
	
	glutSpecialFunc(InputManager::specialDown);
	glutSpecialUpFunc(InputManager::specialUp);

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

	ObjLibrary::TextureBmp* terrainHMap = new ObjLibrary::TextureBmp("Resources/heightmap-highpoly.bmp");

	g_Terrain = Terrain(terrainHMap,
		{
			loadInstructions.terrainConfig.dimensions.x / 2 + loadInstructions.terrainConfig.minimumCoordinates.x,
			0.f,
			loadInstructions.terrainConfig.dimensions.z / 2 + loadInstructions.terrainConfig.minimumCoordinates.z,
		}, loadInstructions.terrainConfig.minimumCoordinates.y,
		loadInstructions.terrainConfig.minimumCoordinates.y + loadInstructions.terrainConfig.dimensions.y,
		&g_AlgaeDL);

	g_Terrain.Init("Resources/dirt2.bmp", "Resources/grass1.bmp", 0, 10);
}

void initDisplay ()
{
	glColor3f(0.0, 0.0, 0.0);
			0.f,

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutPostRedisplay();
}

void update ()
{

	float cameraUpMovement = InputManager::GetCurrentClimbFloatNormalized();
	float rollAmount = InputManager::GetCurrentRollFloatNormalized();
	Vector2 cameraHorizontalMovement = InputManager::GetCurrentMoveVectorNormalized();
	Vector2 cameraLookVector = InputManager::GetCurrentLookVectorNormalized();
	bool HPressed = InputManager::GetIsHPressed();
	

	// up/down
	g_Camera.transform.moveUp(cameraUpMovement * g_MovementSpeed);
	// right/left
	g_Camera.transform.moveRight(cameraHorizontalMovement.x * g_MovementSpeed);
	// forward/backward
	g_Camera.transform.moveForward(cameraHorizontalMovement.y * g_MovementSpeed);


	// looking up/down
	g_Camera.transform.rotateAroundRight(g_LookSpeed * cameraLookVector.y);
	// looking left/right
	g_Camera.transform.rotateAroundUp(g_LookSpeed * -cameraLookVector.x);
	// roll
	g_Camera.transform.rotateAroundForward(g_RollSpeed * rollAmount);

	// rotate to origin
	if (HPressed)
		g_Camera.transform.rotateTowardPosition({0,  0, 0}, g_LookSpeed);

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
	bool isUnderwater = g_Camera.transform.position.y < 0;

	if (isUnderwater)
	{
		// gradually fade to complete darkness as we go down
		float depth = g_Camera.transform.position.y;
		float depthScalar = g_Camera.transform.position.y / 30;

		float color[4] = { max(lightBlue.r + depthScalar, 0.0f),
						   max(lightBlue.g + depthScalar, 0.0f), 
						   max(lightBlue.b + depthScalar, 0.5f), 
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
	// only show depth indicator when underwater
	if (g_Camera.transform.position.y > 0) return;

	float depth = -g_Camera.transform.position.y;

	std::stringstream ss;
	ss << "Depth: " << std::setprecision(0) << std::fixed << std::setw(2) << depth << 'm';

	SpriteFont::setUp2dView(g_WindowWidth, g_WindowHeight);
	g_Font.draw(ss.str(), depthMeterMargin, g_WindowHeight - (g_Font.getHeight() + depthMeterMargin), SpriteFont::BOLD);
	SpriteFont::unsetUp2dView();
}