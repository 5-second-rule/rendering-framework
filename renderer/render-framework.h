#pragma once
#ifdef WIN32
// disable warning about zero-len
// exporting STL classes, and str
#pragma warning( disable : 4200 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4996 )
#ifdef _IS_RENDER_FRAMEWORK
#define FRAMEWORKDLL __declspec(dllexport)
#else
#define FRAMEWORKDLL __declspec(dllimport)
#endif
#endif

#include "renderers.h"

#ifndef RENDERER
	#define RENDERER RENDERER_NONE
#endif