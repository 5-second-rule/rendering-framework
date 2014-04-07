#pragma once

#ifdef _IS_RENDER_FRAMEWORK
#define FRAMEWORKDLL __declspec(dllexport)
#else
#define FRAMEWORKDLL __declspec(dllimport)
#endif

#include "renderers.h"

#ifndef RENDERER
	#define RENDERER RENDERER_NONE
#endif


