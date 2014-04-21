#pragma once
#include "render-framework.h"

class FRAMEWORKDLL Input
{
public:
	enum Key { 
		KEY_W, KEY_A, KEY_S, KEY_D, KEY_Q, KEY_E, 
		KEY_Z, KEY_X, KEY_C, KEY_V,
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
		KEY_SP, KEY_CTRL, KEY_SHFT,
	    KEY_LMB, KEY_RMB,
	};
	enum KeyState { STATE_UP, STATE_DOWN };

	KeyState keys[20];

	void keyDown(Key k);
	void keyUp(Key k);

	Input();
	~Input();
};
