#pragma once
#include "Input.h"

namespace Transmission {
	class Win32Input :
		public Input
	{
		friend class Win32Window;
	public:
		static const int NUM_KEYS = 0x100;

		Win32Input();
		~Win32Input();

	protected:
		static Input::Key keyMap[NUM_KEYS];
		static bool keyMap_initialized;

		void initializeKeyMap();
		Input::Key findKey(int winKey);
		void mouseDown(WPARAM wParam);
		void mouseUp(WPARAM wParam);
		void setMousePosition(int x, int y);
		void keyDown(int winKey);
		void keyUp(int winKey);
		void keyDown(Input::Key k);
		void keyUp(Input::Key k);
	};
}