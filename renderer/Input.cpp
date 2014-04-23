#include "Input.h"

namespace Transmission {

	const int Input::keyMap [] = {
		0x57, 0x41, 0x53, 0x44, 0x51, 0x45,
		0x5A, 0x58, 0x43, 0x56,
		0x31, 0x32, 0x33, 0x34, 0x35,
		VK_SPACE, VK_CONTROL, VK_SHIFT,
		VK_LBUTTON, VK_RBUTTON
	};

	Input::Input() {
	}

	Input::~Input() {
	}

	Input::Key Input::findKey(int winKey) {
		for (int i = 0; i < NUM_KEYS; i++) {
			if (keyMap[i] == winKey)
				return (Key) i;
		}

		return NOT_FOUND;
	}

	void Input::keyDown(Key k) {
		keys[k] = STATE_DOWN;
	}

	void Input::keyUp(Key k) {
		keys[k] = STATE_UP;
	}

}