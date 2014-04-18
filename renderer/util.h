#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct Vertex {
	float point[3];
	float texCoord[2];
	float normal[3];
};

typedef unsigned int Index;

template<typename T>
struct Array{
	T buf[];
	size_t len;
};