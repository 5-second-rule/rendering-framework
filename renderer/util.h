#pragma once

#include <DirectXMath.h>
using namespace DirectX;

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