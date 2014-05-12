#pragma once

#include "Input.h"

#include <cmath>

#include "XInput.h"
#pragma comment(lib, "Xinput9_1_0.lib")

// Use defaults for deazones for now
#define TRIGGER_DEADZONE XINPUT_GAMEPAD_TRIGGER_THRESHOLD    //30
#define LEFT_STICK_DEADZONE XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE     // 7849
#define RIGHT_STICK_DEADZONE XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE   // 8689
#define STICK_MAX 32767

namespace Transmission {
	class Win32Input :
		public Input
	{
		friend class Win32Window;
	public:
		static const int NUM_KEYS = 0x100;
		static const int NUM_CONTROLLER_KEYS = 0x10;

		Win32Input();
		~Win32Input();
		virtual Input::KeyState getKeyState(Input::Key k) const;
		virtual float getAnalogTriggerValue(Input::Key k) const;
		virtual std::pair<Common::Vector4, float> getAnalogStickPosition(Input::Key k) const;

	protected:
		static Input::Key keyMap[NUM_KEYS];
		static bool keyMap_initialized;
		XINPUT_STATE controllerState;

		void initializeKeyMap();
		Input::Key findKey(int winKey);
		void mouseDown(WPARAM wParam);
		void mouseUp(WPARAM wParam);
		void setMousePosition(int x, int y);
		void keyDown(int winKey);
		void keyUp(int winKey);
		void keyDown(Input::Key k);
		void keyUp(Input::Key k);
		void getControllerState();
		int getControllerButton(Input::Key key);
		void queryController(Input::Key key);
	};
}