#pragma once
#include "render-framework.h"

#include <Windows.h>

class FRAMEWORKDLL Input
{
public:
	enum Key { 
		KEY_W, KEY_A, KEY_S, KEY_D, KEY_Q, KEY_E, 
		KEY_Z, KEY_X, KEY_C, KEY_V,
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
		KEY_SP, KEY_CTRL, KEY_SHFT,
	    KEY_LMB, KEY_RMB, NOT_FOUND = -1
	};
	enum KeyState { STATE_UP, STATE_DOWN };

	static const int NUM_KEYS = 20;
	static const int keyMap[];
	KeyState keys[NUM_KEYS];

	static Key findKey(int winKey);
	void keyDown(Key k);
	void keyUp(Key k);

	Input();
	~Input();
};
