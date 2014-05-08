#include "Win32Input.h"

#include <iostream>

namespace Transmission {
	Input::Key Win32Input::keyMap[Win32Input::NUM_KEYS];

	bool Win32Input::keyMap_initialized = false;

	Win32Input::Win32Input()
	{
		if (!Win32Input::keyMap_initialized) {
			Win32Input::initializeKeyMap();
		}
	}


	Win32Input::~Win32Input()
	{
	}

	Input::KeyState Win32Input::getKeyState(Input::Key key) const {
		((Win32Input*)this)->queryController(key);
		return Input::getKeyState(key);
	}

	float Win32Input::getAnalogTriggerValue(Input::Key key) const {
		float depression;

		switch (key) {
		case Input::Key::GAMEPAD_LEFT_TRIGGER:
			depression = this->controllerState.Gamepad.bLeftTrigger;
			break;
		case Input::Key::GAMEPAD_RIGHT_TRIGGER:
			depression = this->controllerState.Gamepad.bRightTrigger;
			break;
		default:
			return 0.0;
		}

		depression -= TRIGGER_DEADZONE;

		if (depression <= 0) {
			return 0.0;
		}

		return depression / (255.0 - TRIGGER_DEADZONE);
	}

	std::pair<Common::Vector4, float> Win32Input::getAnalogStickPosition(Input::Key key) const {
		float x, y, deadzone;

		switch (key) {
		case Input::Key::GAMEPAD_LEFT_STICK:
			x = this->controllerState.Gamepad.sThumbLX;
			y = this->controllerState.Gamepad.sThumbLY;
			deadzone = LEFT_STICK_DEADZONE;
			break;
		case Input::Key::GAMEPAD_RIGHT_STICK:
			x = this->controllerState.Gamepad.sThumbRX;
			y = this->controllerState.Gamepad.sThumbRY;
			deadzone = RIGHT_STICK_DEADZONE;
			break;
		default:
			return std::pair<Common::Vector4, float>(Common::Vector4(), 0.0);
		}

		// Deadzone calculations given by Microsoft DirectX SDK, "Getting Started With XInput"
		float magnitude = sqrt(x*x + y*y);
		float normalizedX = x / magnitude;
		float normalizedY = y / magnitude;
		float normalizedMagnitude;

		if (magnitude <= deadzone) {
			magnitude = 0.0;
			normalizedMagnitude = 0.0;
			normalizedX = 0.0;
			normalizedY = 0.0;
		}

		magnitude = min(magnitude, STICK_MAX);
		magnitude -= deadzone;
		normalizedMagnitude = magnitude / (STICK_MAX - deadzone);

		return std::pair<Common::Vector4, float>(Common::Vector4(normalizedX, normalizedY, 0, 0), normalizedMagnitude);
	}

	Input::Key Win32Input::findKey(int winKey) {
		if (0 <= winKey && winKey < Win32Input::NUM_KEYS) {
			return Win32Input::keyMap[winKey];
		}

		return Input::Key::NOT_FOUND;
	}

	void Win32Input::mouseDown(WPARAM wParam) {
		int btn = GET_KEYSTATE_WPARAM(wParam); // bit mask for all mouse btns

		if (getKeyState(Input::Key::MOUSE_LMB) == Input::KeyState::STATE_UP && btn & MK_LBUTTON) {
			this->keyDown(Input::Key::MOUSE_LMB);
		}
		if (getKeyState(Input::Key::MOUSE_MMB) == Input::KeyState::STATE_UP && btn & MK_MBUTTON) {
			this->keyDown(Input::Key::MOUSE_MMB);
		}
		if (getKeyState(Input::Key::MOUSE_RMB) == Input::KeyState::STATE_UP && btn & MK_RBUTTON) {
			this->keyDown(Input::Key::MOUSE_RMB);
		}
		if (getKeyState(Input::Key::MOUSE_XB1) == Input::KeyState::STATE_UP && btn & MK_XBUTTON1) {
			this->keyDown(Input::Key::MOUSE_XB1);
		}
		if (getKeyState(Input::Key::MOUSE_XB2) == Input::KeyState::STATE_UP && btn & MK_XBUTTON2) {
			this->keyDown(Input::Key::MOUSE_XB2);
		}
	}

	void Win32Input::mouseUp(WPARAM wParam) {
		int btn = GET_KEYSTATE_WPARAM(wParam); // bit mask for all mouse btns

		// Note: flags are set if buttons are DOWN
		if (getKeyState(Input::Key::MOUSE_LMB) == Input::KeyState::STATE_DOWN && !(btn & MK_LBUTTON)) {
			this->keyUp(Input::Key::MOUSE_LMB);
		}
		if (getKeyState(Input::Key::MOUSE_MMB) == Input::KeyState::STATE_DOWN &&!(btn & MK_MBUTTON)) {
			this->keyUp(Input::Key::MOUSE_MMB);
		}
		if (getKeyState(Input::Key::MOUSE_RMB) == Input::KeyState::STATE_DOWN &&!(btn & MK_RBUTTON)) {
			this->keyUp(Input::Key::MOUSE_RMB);
		}
		if (getKeyState(Input::Key::MOUSE_XB1) == Input::KeyState::STATE_DOWN &&!(btn & MK_XBUTTON1)) {
			this->keyUp(Input::Key::MOUSE_XB1);
		}
		if (getKeyState(Input::Key::MOUSE_XB2) == Input::KeyState::STATE_DOWN &&!(btn & MK_XBUTTON2)) {
			this->keyUp(Input::Key::MOUSE_XB2);
		}
	}

	void Win32Input::setMousePosition(int x, int y) {
		this->cursor_xPos = x;
		this->cursor_yPos = y;
	}

	void Win32Input::keyDown(int winKey) {
		Input::Key k = this->findKey(winKey);
		if (k != Input::Key::NOT_FOUND && k < Input::Key::COUNT) {
			this->keys[k] = Input::KeyState::STATE_DOWN;
			this->inputQueue.emplace(k, Input::KeyState::STATE_DOWN);
		}
	}

	void Win32Input::keyUp(int winKey) {
		Input::Key k = this->findKey(winKey);
		if (k != Input::Key::NOT_FOUND && k < Input::Key::COUNT) {
			this->keys[k] = Input::KeyState::STATE_UP;
			this->inputQueue.emplace(k, Input::KeyState::STATE_UP);
		}
	}

	void Win32Input::keyDown(Input::Key key) {
		this->keys[key] = STATE_DOWN;
		this->inputQueue.emplace(key, Input::KeyState::STATE_DOWN);
	}

	void Win32Input::keyUp(Input::Key key) {
		this->keys[key] = STATE_UP;
		this->inputQueue.emplace(key, Input::KeyState::STATE_UP);
	}

	void Win32Input::initializeKeyMap() {
		for (int i = 0; i < Win32Input::NUM_KEYS; i++) {
			Win32Input::keyMap[i] = NOT_FOUND;
		}

		Win32Input::keyMap[VK_LBUTTON] = MOUSE_LMB;
		Win32Input::keyMap[VK_RBUTTON] = MOUSE_RMB;
		Win32Input::keyMap[VK_CANCEL] = CANCEL;
		Win32Input::keyMap[VK_MBUTTON] = MOUSE_MMB;
		Win32Input::keyMap[VK_XBUTTON1] = MOUSE_XB1;
		Win32Input::keyMap[VK_XBUTTON2] = MOUSE_XB2;
		Win32Input::keyMap[VK_BACK] = BACKSPACE;
		Win32Input::keyMap[VK_TAB] = TAB;
		Win32Input::keyMap[VK_CLEAR] = CLEAR;
		Win32Input::keyMap[VK_RETURN] = ENTER;
		Win32Input::keyMap[VK_SHIFT] = SHIFT;
		Win32Input::keyMap[VK_CONTROL] = CONTROL;
		Win32Input::keyMap[VK_MENU] = ALT;
		Win32Input::keyMap[VK_PAUSE] = PAUSE;
		Win32Input::keyMap[VK_CAPITAL] = CAPSLOCK;
		Win32Input::keyMap[VK_ESCAPE] = ESC;
		Win32Input::keyMap[VK_SPACE] = SPACE;
		Win32Input::keyMap[VK_PRIOR] = PAGE_UP;
		Win32Input::keyMap[VK_NEXT] = PAGE_DOWN;
		Win32Input::keyMap[VK_END] = END;
		Win32Input::keyMap[VK_HOME] = HOME;
		Win32Input::keyMap[VK_LEFT] = LEFT_ARROW;
		Win32Input::keyMap[VK_UP] = UP_ARROW;
		Win32Input::keyMap[VK_RIGHT] = RIGHT_ARROW;
		Win32Input::keyMap[VK_DOWN] = DOWN_ARROW;
		Win32Input::keyMap[VK_SNAPSHOT] = PRINTSCREEN;
		Win32Input::keyMap[VK_INSERT] = INSERT;
		Win32Input::keyMap[VK_DELETE] = DEL;
		Win32Input::keyMap[0x30] = KEY_0;
		Win32Input::keyMap[0x31] = KEY_1;
		Win32Input::keyMap[0x32] = KEY_2;
		Win32Input::keyMap[0x33] = KEY_3;
		Win32Input::keyMap[0x34] = KEY_4;
		Win32Input::keyMap[0x35] = KEY_5;
		Win32Input::keyMap[0x36] = KEY_6;
		Win32Input::keyMap[0x37] = KEY_7;
		Win32Input::keyMap[0x38] = KEY_8;
		Win32Input::keyMap[0x39] = KEY_9;
		Win32Input::keyMap[0x41] = A;
		Win32Input::keyMap[0x42] = B;
		Win32Input::keyMap[0x43] = C;
		Win32Input::keyMap[0x44] = D;
		Win32Input::keyMap[0x45] = E;
		Win32Input::keyMap[0x46] = F;
		Win32Input::keyMap[0x47] = G;
		Win32Input::keyMap[0x48] = H;
		Win32Input::keyMap[0x49] = I;
		Win32Input::keyMap[0x4A] = J;
		Win32Input::keyMap[0x4B] = K;
		Win32Input::keyMap[0x4C] = L;
		Win32Input::keyMap[0x4D] = M;
		Win32Input::keyMap[0x4E] = N;
		Win32Input::keyMap[0x4F] = O;
		Win32Input::keyMap[0x50] = P;
		Win32Input::keyMap[0x51] = Q;
		Win32Input::keyMap[0x52] = R;
		Win32Input::keyMap[0x53] = S;
		Win32Input::keyMap[0x54] = T;
		Win32Input::keyMap[0x55] = U;
		Win32Input::keyMap[0x56] = V;
		Win32Input::keyMap[0x57] = W;
		Win32Input::keyMap[0x58] = X;
		Win32Input::keyMap[0x59] = Y;
		Win32Input::keyMap[0x5A] = Z;
		Win32Input::keyMap[VK_LWIN] = SUPER;
		Win32Input::keyMap[VK_RWIN] = SUPER;
		Win32Input::keyMap[VK_APPS] = MENU;
		Win32Input::keyMap[VK_NUMPAD0] = NUM_0;
		Win32Input::keyMap[VK_NUMPAD1] = NUM_1;
		Win32Input::keyMap[VK_NUMPAD2] = NUM_2;
		Win32Input::keyMap[VK_NUMPAD3] = NUM_3;
		Win32Input::keyMap[VK_NUMPAD4] = NUM_4;
		Win32Input::keyMap[VK_NUMPAD5] = NUM_5;
		Win32Input::keyMap[VK_NUMPAD6] = NUM_6;
		Win32Input::keyMap[VK_NUMPAD7] = NUM_7;
		Win32Input::keyMap[VK_NUMPAD8] = NUM_8;
		Win32Input::keyMap[VK_NUMPAD9] = NUM_9;
		Win32Input::keyMap[VK_MULTIPLY] = NUM_MULT;
		Win32Input::keyMap[VK_ADD] = NUM_ADD;
		Win32Input::keyMap[VK_SEPARATOR] = ENTER;
		Win32Input::keyMap[VK_SUBTRACT] = NUM_SUB;
		Win32Input::keyMap[VK_DECIMAL] = NUM_DEC;
		Win32Input::keyMap[VK_DIVIDE] = NUM_DIV;
		Win32Input::keyMap[VK_F1] = F1;
		Win32Input::keyMap[VK_F2] = F2;
		Win32Input::keyMap[VK_F3] = F3;
		Win32Input::keyMap[VK_F4] = F4;
		Win32Input::keyMap[VK_F5] = F5;
		Win32Input::keyMap[VK_F6] = F6;
		Win32Input::keyMap[VK_F7] = F7;
		Win32Input::keyMap[VK_F8] = F8;
		Win32Input::keyMap[VK_F9] = F9;
		Win32Input::keyMap[VK_F10] = F10;
		Win32Input::keyMap[VK_F11] = F11;
		Win32Input::keyMap[VK_F12] = F12;
		Win32Input::keyMap[VK_NUMLOCK] = NUM_LOCK;
		Win32Input::keyMap[VK_SCROLL] = SCROLL_LOCK;
		Win32Input::keyMap[VK_LSHIFT] = SHIFT;
		Win32Input::keyMap[VK_RSHIFT] = SHIFT;
		Win32Input::keyMap[VK_LCONTROL] = CONTROL;
		Win32Input::keyMap[VK_RCONTROL] = CONTROL;

		Win32Input::keyMap_initialized = true;
	}

	void Win32Input::getControllerState() {
		// Code from Microsoft DirectX SDK, "Getting Started With XInput"
		ZeroMemory(&this->controllerState, sizeof(XINPUT_STATE));

		if (XInputGetState(0, &this->controllerState) == ERROR_SUCCESS) {
			this->controllerConnected = true;
		} else {
			this->controllerConnected = false;
		}
	}

	int Win32Input::getControllerButton(Input::Key key) {
		switch (key) {
		case GAMEPAD_A:
			return XINPUT_GAMEPAD_A;
		case GAMEPAD_B:
			return XINPUT_GAMEPAD_B;
		case GAMEPAD_X:
			return XINPUT_GAMEPAD_X;
		case GAMEPAD_Y:
			return XINPUT_GAMEPAD_Y;
		case GAMEPAD_DPAD_LEFT:
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case GAMEPAD_DPAD_RIGHT:
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		case GAMEPAD_DPAD_UP:
			return XINPUT_GAMEPAD_DPAD_UP;
		case GAMEPAD_DPAD_DOWN:
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case GAMEPAD_LEFT_SHOULDER:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case GAMEPAD_RIGHT_SHOULDER:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case GAMEPAD_LEFT_THUMB:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case GAMEPAD_RIGHT_THUMB:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case GAMEPAD_BACK:
			return XINPUT_GAMEPAD_BACK;
		case GAMEPAD_START:
			return XINPUT_GAMEPAD_START;
		default:
			return 0;
		}
	}
	
	void Win32Input::queryController(Input::Key key) {
		this->getControllerState();
		int buttonCode = this->getControllerButton(key);
		Input::KeyState currentState = Input::getKeyState(key);

		if (this->controllerConnected && key >= Input::Key::GAMEPAD_A && key <= Input::Key::GAMEPAD_START) {
			if (currentState == Input::KeyState::STATE_UP && ((this->controllerState.Gamepad.wButtons & buttonCode) != 0)) {
				this->keyDown(key);
			} else if (currentState == Input::KeyState::STATE_DOWN && ((this->controllerState.Gamepad.wButtons & buttonCode) == 0)) {
				this->keyUp(key);
			}
		}
	}
}