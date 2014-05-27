#pragma once

#include "render-framework.h"

namespace Transmission {
	class FRAMEWORKDLL Sound {
	public:
		Sound();
		~Sound();

		virtual bool play() = 0;
		virtual bool playLooped() = 0;
		virtual bool stop() = 0;
	};
}

