#include "Input.h"

namespace Transmission {
	Input::KeyState Input::keys[(unsigned int)Input::Key::COUNT];
	bool Input::keyMap_initialized = false;

	Input::Input() {
	}

	Input::KeyState Input::getKeyState(Input::Key k) {
		if (0 <= k && k < COUNT) {
			return Input::keys[k];
		}

		return KeyState::STATE_UP;
	}
}