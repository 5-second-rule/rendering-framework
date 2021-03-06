#pragma once
#include "render-framework.h"

#include <Windows.h>

#include <queue>
#include "common/Vector4.h"

namespace Transmission {
	class FRAMEWORKDLL Input
	{
	public:
		enum Key {
			MOUSE_LMB, MOUSE_RMB, CANCEL, MOUSE_MMB, MOUSE_XB1, MOUSE_XB2,
			BACKSPACE, TAB, CLEAR, ENTER, SHIFT, CONTROL, ALT,
			PAUSE, CAPSLOCK, ESC, SPACE, PAGE_UP, PAGE_DOWN, END, HOME,
			LEFT_ARROW, UP_ARROW, RIGHT_ARROW, DOWN_ARROW, PRINTSCREEN, INSERT, DEL,
			KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			SUPER, MENU, NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
			NUM_MULT, NUM_ADD, NUM_SUB, NUM_DEC, NUM_DIV,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			NUM_LOCK, SCROLL_LOCK,
			GAMEPAD_A, GAMEPAD_B, GAMEPAD_X, GAMEPAD_Y,
			GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_RIGHT, GAMEPAD_DPAD_UP, GAMEPAD_DPAD_DOWN,
			GAMEPAD_LEFT_SHOULDER, GAMEPAD_RIGHT_SHOULDER,
			GAMEPAD_LEFT_THUMB, GAMEPAD_RIGHT_THUMB,
			GAMEPAD_BACK, GAMEPAD_START,
			GAMEPAD_LEFT_TRIGGER, GAMEPAD_RIGHT_TRIGGER,  // analog
			GAMEPAD_LEFT_STICK, GAMEPAD_RIGHT_STICK,      // analog
			COUNT, NOT_FOUND = -1
		};
		enum KeyState { STATE_UP, STATE_DOWN, ANALOG };
		typedef std::queue<std::pair<Input::Key, Input::KeyState>> KeyStateQueue;

		Input();
		~Input();

		virtual KeyState getKeyState(Key k) const;
		virtual float getAnalogTriggerValue(Key k) const;
		virtual std::pair<Common::Vector4,float> getAnalogStickPosition(Key k) const;
		bool isControllerConnected() const;
		KeyStateQueue getInputQueue();
		int getCursorXPosition(); // Signed x client coordinates
		int getCursorYPosition(); // Signed y client coordinates
		

	protected:
		KeyStateQueue inputQueue;
		KeyState keys[(unsigned int)COUNT];
		int cursor_xPos;
		int cursor_yPos;
		bool controllerConnected;
	};
}
