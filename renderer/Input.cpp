#include "Input.h"

namespace Transmission {
	Input::Input() {
		for (int i = 0; i < COUNT; ++i) {
			this->keys[i] = Input::KeyState::STATE_UP;
		}
		this->inputQueue = new KeyStateQueue();
	}

	Input::~Input() {
		delete this->inputQueue;
	}

	Input::KeyState Input::getKeyState(Input::Key k) {
		if (0 <= k && k < COUNT) {
			return this->keys[k];
		}

		return KeyState::STATE_UP;
	}

	Input::KeyStateQueue* Input::getInputQueue() {
		// Save the list, prepare to pass to engine
		Input::KeyStateQueue* q = this->inputQueue;
		
		// Purge current queue
		delete this->inputQueue;
		this->inputQueue = new KeyStateQueue();

		return q;
	}

	int Input::getCursorXPosition() {
		return this->cursor_xPos;
	}

	int Input::getCursorYPosition() {
		return this->cursor_yPos;
	}
}