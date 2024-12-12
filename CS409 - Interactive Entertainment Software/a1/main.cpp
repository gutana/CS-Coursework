//
//  main.cpp
//

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <map>

#include "GetGlut.h"
#include "Sleep.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/TextureManager.h"

#include "InputManager.h"
#include "FileParser.h"
#include "GameEntity.h"

using namespace std;
using namespace ObjLibrary; 

float g_MovementSpeed = 0.1;
float g_LookSpeed = 0.05;
float g_RollSpeed = 0.05;

void init ();
void initDisplay ();
void update ();
void reshape (int w, int h);
void display ();
void rotateCameraToOrigin();
void rotateCameraToVector(const Vector3& desiredForward);

std::vector<Drawable*> g_DrawableObjects;
std::map<std::string, DisplayList> g_DisplayListMap;

ObjModel g_WaterSurface;
DisplayList g_WaterSurfaceDL;

ObjModel g_Skybox;
DisplayList g_SkyboxDL;

Vector3 g_CameraForward(1, 0, 0);
Vector3 g_CameraUp(0, 1, 0);
Vector3 g_CameraRight(0, 0, 1);

Vector3 g_CameraPosition(0, 0, 0);


int main (int argc, char* argv[])
{
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Assignment 1 - CS 409");

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

	g_CameraPosition = loadInstructions.playerStartingPosition;
	g_CameraForward = loadInstructions.playerStartingForward;
	g_CameraUp = loadInstructions.playerStartingUp;
	g_CameraRight = g_CameraForward.crossProduct(g_CameraUp);

	for (auto sphereConfig : loadInstructions.sphereConfiguations)
	{
		SphereEntity* sphere = new SphereEntity(sphereConfig.center, sphereConfig.radius, sphereConfig.filename);

		auto it = g_DisplayListMap.find(sphereConfig.filename);
		// if already in our displaylist map
		if (it != g_DisplayListMap.end())
		{
			sphere->displayList = it->second;
		}
		// if not in display list map
		else
		{
			sphere->objModel.load("Resources/" + sphere->filename);
			sphere->displayList = sphere->objModel.getDisplayList();
			g_DisplayListMap[sphere->filename] = sphere->displayList;
		}
		g_DrawableObjects.push_back(sphere);
	}
	for (auto cylinderConfig : loadInstructions.cylinderConfigurations)
	{
		CylinderEntity* cylinder = new CylinderEntity(cylinderConfig.start, cylinderConfig.end, cylinderConfig.radius, cylinderConfig.filename);

		auto it = g_DisplayListMap.find(cylinderConfig.filename);
		if (it != g_DisplayListMap.end())
		{
			cylinder->displayList = it->second;
		}
		// if not in display list map
		else
		{
			cylinder->objModel.load("Resources/" + cylinder->filename);
			cylinder->displayList = cylinder->objModel.getDisplayList();
			g_DisplayListMap[cylinder->filename] = cylinder->displayList;
		}
		g_DrawableObjects.push_back(cylinder);
	}

	g_WaterSurface.load("Resources/surface.obj");
	g_WaterSurfaceDL = g_WaterSurface.getDisplayList();

	g_Skybox.load("Resources/Skybox.obj");
	g_SkyboxDL = g_Skybox.getDisplayList();
}

void initDisplay ()
{
	glColor3f(0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutPostRedisplay();
}

void update ()
{
	sleep(1.0 / 60.0);

	float cameraUpMovement = InputManager::GetCurrentClimbFloatNormalized();
	float rollAmount = InputManager::GetCurrentRollFloatNormalized();
	Vector2 cameraHorizontalMovement = InputManager::GetCurrentMoveVectorNormalized();
	Vector2 cameraLookVector = InputManager::GetCurrentLookVectorNormalized();
	bool HPressed = InputManager::GetIsHPressed();
	

	// up/down movement
	g_CameraPosition += g_CameraUp * g_MovementSpeed * cameraUpMovement;

	// forward/right movement
	g_CameraPosition += g_CameraForward * cameraHorizontalMovement.x * g_MovementSpeed;
	g_CameraPosition += g_CameraRight * cameraHorizontalMovement.y * g_MovementSpeed;

	// looking up/down
	g_CameraForward.rotateArbitrary(g_CameraRight, g_LookSpeed * cameraLookVector.y);
	g_CameraUp.rotateArbitrary(g_CameraRight, g_LookSpeed * cameraLookVector.y);

	// looking left/right
	g_CameraForward.rotateArbitrary(g_CameraUp, g_LookSpeed * -cameraLookVector.x);
	g_CameraRight.rotateArbitrary(g_CameraUp, g_LookSpeed * -cameraLookVector.x);

	// roll
	g_CameraUp.rotateArbitrary(g_CameraForward, g_RollSpeed * rollAmount);
	g_CameraRight.rotateArbitrary(g_CameraForward, g_RollSpeed * rollAmount);

	// rotate to origin
	if (HPressed)
		rotateCameraToOrigin();

	glutPostRedisplay();
}

void rotateCameraToOrigin()
{
	Vector3 origin = Vector3::ZERO;
	Vector3 directionToOrigin = origin - g_CameraPosition;

	directionToOrigin.normalize();

	rotateCameraToVector(directionToOrigin);
}

void rotateCameraToVector(const Vector3& desiredForward)
{
	if (desiredForward.isZero())
		return; 

	Vector3 axis = g_CameraForward.crossProduct(desiredForward);

	if (axis.isZero())
		axis = g_CameraUp;
	else
		axis.normalize();

	float radians = g_CameraForward.getAngleSafe(desiredForward);

	if (radians > g_LookSpeed)
		radians = g_LookSpeed;

	g_CameraForward.rotateArbitrary(axis, std::min(g_LookSpeed, radians));
	g_CameraUp.rotateArbitrary(axis, std::min(g_LookSpeed, radians));
	g_CameraRight.rotateArbitrary(axis, std::min(g_LookSpeed, radians));

}

void reshape (int w, int h)
{
	glViewport (0, 0, w, h);

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
	bool isUnderwater = g_CameraPosition.y < 0;

	if (isUnderwater)
	{
		glEnable(GL_FOG);

		// gradually fade to complete darkness as we go down
		float depth = g_CameraPosition.y;
		float depthScalar = g_CameraPosition.y / 30;

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
		glDisable(GL_FOG);
		glClearColor(1, 1, 1, 0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display at all

	glLoadIdentity();

	Vector3 lookAt = g_CameraPosition + g_CameraForward;

	gluLookAt(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z,  // (x, y, z) camera eye position
	          lookAt.x,			  lookAt.y,			  lookAt.z,  // (x, y, z) camera look at position
	          g_CameraUp.x,		  g_CameraUp.y,       g_CameraUp.z); // (x, y, z) camera up direction

	if (!isUnderwater) // Draw skybox
	{
		glPushMatrix();
			glDepthMask(GL_FALSE);
			glTranslatef(g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z);
			g_SkyboxDL.draw();
			glDepthMask(GL_TRUE);
		glPopMatrix();
	}

	// All of the objects in map.txt
	for (auto drawable : g_DrawableObjects)
	{
		drawable->render();
	}

	g_WaterSurfaceDL.draw();

	glutSwapBuffers();
}