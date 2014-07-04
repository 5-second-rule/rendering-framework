#pragma once

// disable warning about zero-len
// exporting STL classes, and str
#pragma warning( disable : 4200 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4067 )

// DLL export macro
#ifdef _IS_RENDER_FRAMEWORK
#define FRAMEWORKDLL __declspec(dllexport)
#else
#define FRAMEWORKDLL __declspec(dllimport)
#endif

// list available renderers
#define RENDERER_NONE 0
#define RENDERER_DX 1
#define RENDERER_GL 2

#ifndef RENDERER
	#define RENDERER RENDERER_NONE
#endif