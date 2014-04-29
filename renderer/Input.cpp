#include "Input.h"

namespace Transmission {
	Input::KeyState Input::keys[(unsigned int)Input::Key::COUNT];
	bool Input::keyMap_initialized = false;
	int Input::cursor_xPos;
	int Input::cursor_yPos;

	Input::Input() {
	}

	Input::KeyState Input::getKeyState(Input::Key k) {
		if (0 <= k && k < COUNT) {
			return Input::keys[k];
		}

		return KeyState::STATE_UP;
	}

	int Input::getCursorXPosition() {
		return Input::cursor_xPos;
	}

	int Input::getCursorYPosition() {
		return Input::cursor_yPos;
	}
}