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
		void initializeKeyMap();
		static Key keyMap[NUM_KEYS];

		Key findKey(int winKey);
		void keyDown(int winKey);
		void keyUp(int winKey);
		void keyDown(Key k);
		void keyUp(Key k);
	};
}