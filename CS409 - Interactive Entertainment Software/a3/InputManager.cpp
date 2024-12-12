#include "InputManager.h"


bool InputManager::moveUpPressed = false;		// 'W'
bool InputManager::moveDownPressed = false;    // 'S'
bool InputManager::strafeLeftPressed = false;  // 'A'
bool InputManager::strafeRightPressed = false; // 'D'

bool InputManager::forwardPressed = false;    // ' '
bool InputManager::backwardPressed = false;    // '/'

bool InputManager::lookUpPressed = false;      // keyboard up
bool InputManager::lookDownPressed = false;    // keyboard down
bool InputManager::lookRightPressed = false;   // keyboard right
bool InputManager::lookLeftPressed = false;    // keyboard left

bool InputManager::rollRightPressed = false;   // e
bool InputManager::rollLeftPressed = false;    // q

bool InputManager::HPressed = false;
bool InputManager::UPressed = false;
bool InputManager::PPressed = false;
bool InputManager::YPressed = false;
bool InputManager::onePressed = false;
bool InputManager::twoPressed = false;


void (*InputManager::m_FpsOverlayToggleCallback)() = nullptr;
void (*InputManager::m_PauseToggleCallback)() = nullptr;
void (*InputManager::m_SchoolDebugToggleCallback)() = nullptr;


void InputManager::keyboardDown(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: // on [ESC]
			exit(0); // normal exit
			break;
		case '1':
			if (onePressed == false && m_FpsOverlayToggleCallback != nullptr)
				 m_FpsOverlayToggleCallback();
			onePressed = true;
			break;
		case 'p':
		case 'P':
			if (PPressed == false && m_PauseToggleCallback != nullptr)
				m_PauseToggleCallback();
			PPressed = true;
			break;
		case '2':
			if (twoPressed == false && m_SchoolDebugToggleCallback != nullptr)
				m_SchoolDebugToggleCallback();
			twoPressed = true;
			break;
		case 'y':
		case 'Y':
			YPressed = true;
			break;
		case ' ':
			forwardPressed = true;
			break;
		case 'u':
		case 'U':
			UPressed = true;
			break;
		case '/':
			backwardPressed = true;
			break;
		case 'w':
		case 'W':
			moveUpPressed = true; 
			break;
		case 'a':
		case 'A':
			strafeLeftPressed = true;
			break;
		case 's':
		case 'S':
			moveDownPressed = true;
			break;
		case 'd':
		case 'D':
			strafeRightPressed = true;
			break; 
		case 'q':
		case 'Q':
			rollLeftPressed = true;
			break;
		case 'e':
		case 'E':
			rollRightPressed = true;
			break;
		case 'h':
		case 'H':
			HPressed = true;
			break;
			
	}
}


void InputManager::keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
		{

		case 27: // on [ESC]
			exit(0); // normal exit
			break;
		case '1':
			onePressed = false;
			break;
		case '2':
			twoPressed = false;
			break;
		case 'p':
		case 'P':
			PPressed = false;
			break;
		case 'y':
		case 'Y':
			YPressed = false;
			break;
		case ' ':
			forwardPressed = false;
			break;
		case '/':
			backwardPressed = false;
			break;
		case 'u':
		case 'U':
			UPressed = false;
			break;
		case 'w':
		case 'W':
			moveUpPressed = false;
			break;
		case 'a':
		case 'A':
			strafeLeftPressed = false;
			break;
		case 's':
		case 'S':
			moveDownPressed = false;
			break;
		case 'd':
		case 'D':
			strafeRightPressed = false;
			break; 
		case 'q':
		case 'Q':
			rollLeftPressed = false;
			break;
		case 'e':
		case 'E':
			rollRightPressed = false;
			break;
		case 'h':
		case 'H':
			HPressed = false;
			break;

	}
}



void InputManager::specialDown(int special_key, int x, int y)
{
	switch (special_key)
	{
		case GLUT_KEY_RIGHT:
			lookRightPressed = true;
			break;
		case GLUT_KEY_LEFT:
			lookLeftPressed = true;
			break;
		case GLUT_KEY_UP:
			lookUpPressed = true;
			break;
		case GLUT_KEY_DOWN:
			lookDownPressed = true;
			break;
	}
}

void InputManager::specialUp(int special_key, int x, int y)
{		
	switch (special_key)
	{
		case GLUT_KEY_RIGHT:
			lookRightPressed = false;
			break;
		case GLUT_KEY_LEFT:
			lookLeftPressed = false;
			break;
		case GLUT_KEY_UP:
			lookUpPressed = false;
			break;
		case GLUT_KEY_DOWN:
			lookDownPressed = false;
			break;
	}
}

void InputManager::RegisterFpsOverlayToggleCallback(void(*callback)())
{
	m_FpsOverlayToggleCallback = callback;
}

void InputManager::RegisterPauseToggleCallback(void(*callback)())
{
	m_PauseToggleCallback = callback;
}

void InputManager::RegisterFishSchoolDebugDisplayToggleCallback(void(*callback)())
{
	m_SchoolDebugToggleCallback = callback;
}

bool InputManager::GetIsUPressed()
{
	return UPressed;
}

bool InputManager::GetIsYPressed()
{
	return YPressed;
}

float InputManager::GetCurrentClimbFloatNormalized()
{
	if (moveUpPressed && moveDownPressed)
		return 0;

	if (moveUpPressed)
		return 1;

	if (moveDownPressed)
		return -1;

	return 0;
}

Vector2 InputManager::GetCurrentLookVectorNormalized()
{
	// calculate vertical float
	float up = 0;
	if (lookUpPressed && !lookDownPressed)
		up = 1;
	else if (!lookUpPressed && lookDownPressed)
		up = -1;

	// calculate horizontal float
	float right = 0; 
	if (lookRightPressed && !lookLeftPressed)
		right = 1;
	else if (!lookRightPressed && lookLeftPressed)
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

	if (forwardPressed && !backwardPressed)
		forward = 1;
	else if (!forwardPressed && backwardPressed)
		forward = -1;

	// calculate the right float
	float right = 0;

	if (strafeRightPressed && !strafeLeftPressed)
		right = 1;
	else if (!strafeRightPressed && strafeLeftPressed)
		right = -1;

	Vector2 result(right, forward);

	if (!result.isZero())
		result.normalize();

	return result;

}


float InputManager::GetCurrentRollFloatNormalized()
{
	if (rollRightPressed && !rollLeftPressed)
		return 1;
	else if (!rollRightPressed && rollLeftPressed)
		return -1;
	return 0;
}

bool InputManager::GetIsHPressed()
{
	return HPressed;
}


