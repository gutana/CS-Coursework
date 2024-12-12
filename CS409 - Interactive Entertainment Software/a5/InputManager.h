#pragma once

#include "GetGlut.h"
#include "ObjLibrary/ObjModel.h"

using ObjLibrary::Vector2;

const unsigned int NUM_NORMAL_KEYS = 256;
const unsigned int NUM_SPECIAL_KEYS = 4;
const unsigned int NUM_KEYS = NUM_NORMAL_KEYS + NUM_SPECIAL_KEYS;

const unsigned int ARROW_UP = 256;
const unsigned int ARROW_RIGHT = 257;
const unsigned int ARROW_DOWN = 258;
const unsigned int ARROW_LEFT = 259;
const unsigned int ESC = 27; // not a special key btw, ascii char

class InputManager
{
public:
	// call these to get the current movement values
	static Vector2 GetCurrentLookVectorNormalized();
	static Vector2 GetCurrentMoveVectorNormalized();
	static float GetCurrentClimbFloatNormalized();
	static float GetCurrentRollFloatNormalized();

	// if you want to get a callback when a char is pressed
	// not case sensitive
	static void RegisterKeyDownCallback(unsigned int c, void (*callback)());
	static void RegisterMoveOrRotateKeyDownCallback(void (*callback)());

	// Register these functions with glut
	static void keyboardDown(unsigned char key, int x, int y);
	static void keyboardUp(unsigned char key, int x, int y);
	static void specialDown(int special_key, int x, int y);
	static void specialUp(int special_key, int x, int y);

	static bool GetIsPressed(unsigned int c);

private:
	static bool keysPressed[NUM_KEYS];
	static void (*functionCallbacks[NUM_KEYS])();
	static void (*anyKeyPressedCallback)();
};
