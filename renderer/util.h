#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#define POINT(x, y, z) XMVectorSet((x), (y), (z), 1.0f)
#define VECTOR(x, y, z) XMVectorSet((x), (y), (z), 0.0f)

struct Vertex {
	XMFLOAT3 point;
	XMFLOAT4 color;
};

typedef unsigned int Index;

template<typename T>
struct Array{
	T buf[];
	size_t len;
};