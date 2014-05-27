#include "Input.h"

namespace Transmission {
	Input::Input() {
		for (int i = 0; i < COUNT; ++i) {
			this->keys[i] = Input::KeyState::STATE_UP;
		}

		this->keys[Input::Key::GAMEPAD_LEFT_TRIGGER]  = Input::KeyState::ANALOG;
		this->keys[Input::Key::GAMEPAD_RIGHT_TRIGGER] = Input::KeyState::ANALOG;
		this->keys[Input::Key::GAMEPAD_LEFT_STICK]    = Input::KeyState::ANALOG;
		this->keys[Input::Key::GAMEPAD_RIGHT_STICK]   = Input::KeyState::ANALOG;

		this->inputQueue = KeyStateQueue();
	}

	Input::~Input() {;
	}

	Input::KeyState Input::getKeyState(Input::Key k) const {
		//this->keys[W] = STATE_DOWN;
		if (0 <= k && k < COUNT) {
			return this->keys[k];
		}

		return KeyState::STATE_UP;
	}

	float Input::getAnalogTriggerValue(Input::Key k) const {
		return 0.0;
	}

	std::pair<Common::Vector4, float> Input::getAnalogStickPosition(Input::Key k) const {
		return std::pair<Common::Vector4, float>(Common::Vector4(), 0.0f);
	}

	bool Input::isControllerConnected() const {
		return controllerConnected;
	}

	Input::KeyStateQueue Input::getInputQueue() {
		// Save the list, prepare to pass to engine
		Input::KeyStateQueue q = this->inputQueue;
		
		// Purge current queue
		this->inputQueue = KeyStateQueue();

		return q;
	}

	int Input::getCursorXPosition() {
		return this->cursor_xPos;
	}

	int Input::getCursorYPosition() {
		return this->cursor_yPos;
	}
}