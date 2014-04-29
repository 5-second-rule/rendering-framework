#include "Win32Input.h"

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
}