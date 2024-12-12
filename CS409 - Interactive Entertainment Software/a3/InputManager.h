#pragma once


#include "GetGlut.h"
#include "ObjLibrary/ObjModel.h"

using ObjLibrary::Vector2;

class InputManager
{
public:
	// call these to get the current movement values
	static Vector2 GetCurrentLookVectorNormalized();
	static Vector2 GetCurrentMoveVectorNormalized();
	static float GetCurrentClimbFloatNormalized();
	static float GetCurrentRollFloatNormalized();
	static bool GetIsHPressed();

	// Register these functions with glut
	static void keyboardDown(unsigned char key, int x, int y);
	static void keyboardUp(unsigned char key, int x, int y);
	static void specialDown(int special_key, int x, int y);
	static void specialUp(int special_key, int x, int y);

	// Get callback for '1' pressed
	static void RegisterFpsOverlayToggleCallback(void (*callback)());
	static void RegisterPauseToggleCallback(void (*callback)());
	static void RegisterFishSchoolDebugDisplayToggleCallback(void (*callback)());

	static bool GetIsUPressed();
	static bool GetIsYPressed();

private: // Having now looked at tutorial 5, I realize I should've just used an array of bools. 
	static bool moveUpPressed;		// 'W'
	static bool moveDownPressed;    // 'S'
	static bool strafeLeftPressed;  // 'A'
	static bool strafeRightPressed; // 'D'

	static bool forwardPressed;    // ' '
	static bool backwardPressed;    // '/'


	static bool lookUpPressed;      // keyboard up
	static bool lookDownPressed;    // keyboard down
	static bool lookRightPressed;   // keyboard right
	static bool lookLeftPressed;    // keyboard left

	static bool rollRightPressed;
	static bool rollLeftPressed;

	static bool HPressed;
	static bool UPressed;
	static bool YPressed;
	static bool PPressed;
	static bool onePressed;
	static bool twoPressed;

	static void (* m_FpsOverlayToggleCallback)();
	static void (* m_PauseToggleCallback)();
	static void (* m_SchoolDebugToggleCallback)();

};
