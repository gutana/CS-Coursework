#include "InputManager.h"

#include "stdio.h"

bool InputManager::keysPressed[NUM_KEYS] = {0};
void (*InputManager::functionCallbacks[NUM_KEYS])() = {0};

void InputManager::keyboardDown(unsigned char key, int x, int y)
{
	char lower = tolower(key);
	if (lower < 0)
	{
		std::cout << "Unexpected input: " << key << '\n';
		return;
	}

	keysPressed[lower] = true;

	if (functionCallbacks[lower] != nullptr)
		(functionCallbacks[lower])();
}

void InputManager::keyboardUp(unsigned char key, int x, int y)
{
	char lower = tolower(key);
	if (lower < 0)
	{
		std::cout << "Unexpected input: " << key << '\n';
		return;
	}

	keysPressed[lower] = false;
}

void InputManager::specialDown(int special_key, int x, int y)
{
	switch (special_key)
	{
		case GLUT_KEY_RIGHT:
			keysPressed[ARROW_RIGHT] = true;
			break;
		case GLUT_KEY_LEFT:
			keysPressed[ARROW_LEFT] = true;
			break;
		case GLUT_KEY_UP:
			keysPressed[ARROW_UP] = true;
			break;
		case GLUT_KEY_DOWN:
			keysPressed[ARROW_DOWN] = true;
			break;
		case 27:
			keysPressed[ESC] = true;
			break;
	}
}

void InputManager::specialUp(int special_key, int x, int y)
{		
	switch (special_key)
	{
		case GLUT_KEY_RIGHT:
			keysPressed[ARROW_RIGHT] = false;
			break;
		case GLUT_KEY_LEFT:
			keysPressed[ARROW_LEFT] = false;
			break;
		case GLUT_KEY_UP:
			keysPressed[ARROW_UP] = false;
			break;
		case GLUT_KEY_DOWN:
			keysPressed[ARROW_DOWN] = false;
			break;
	}
}

void InputManager::RegisterKeyDownCallback(unsigned int c, void(*callback)())
{
	char lower = tolower(c);

	if (functionCallbacks[lower] != nullptr)
		throw "Key down callback was already registered";

	functionCallbacks[lower] = callback;
}

bool InputManager::GetIsPressed(unsigned int c)
{
	char lower = tolower(c);
	return keysPressed[lower];
}

float InputManager::GetCurrentClimbFloatNormalized()
{
	if (keysPressed['w'] && keysPressed['s'])
		return 0;

	if (keysPressed['w'])
		return 1;

	if (keysPressed['s'])
		return -1;

	return 0;
}

Vector2 InputManager::GetCurrentLookVectorNormalized()
{
	// calculate vertical float
	float up = 0;
	if (keysPressed[ARROW_UP] && !keysPressed[ARROW_DOWN])
		up = 1;
	else if (!keysPressed[ARROW_UP] && keysPressed[ARROW_DOWN])
		up = -1;

	// calculate horizontal float
	float right = 0; 
	if (keysPressed[ARROW_RIGHT] && !keysPressed[ARROW_LEFT])
		right = 1;
	else if (!keysPressed[ARROW_RIGHT] && keysPressed[ARROW_LEFT])
		right = -1;

	Vector2 result(right, up);

	if (!result.isZero())
		result.normalize();

	return result;
}
Vector2 InputManager::GetCurrentMoveVectorNormalized()
{
	// calculate the forward float
	float forward = 0;

	if (keysPressed[' '] && !keysPressed['/'])
		forward = 1;
	else if (!keysPressed[' '] && keysPressed['/'])
		forward = -1;

	// calculate the right float
	float right = 0;

	if (keysPressed['d'] && !keysPressed['a'])
		right = 1;
	else if (!keysPressed['d'] && keysPressed['a'])
		right = -1;

	Vector2 result(right, forward);

	if (!result.isZero())
		result.normalize();

	return result;
}

float InputManager::GetCurrentRollFloatNormalized()
{
	if (keysPressed['e'] && !keysPressed['q'])
		return 1;
	else if (!keysPressed['e'] && keysPressed['q'])
		return -1;
	return 0;
}

