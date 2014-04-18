#pragma once

#define Point(x, y, z) Vector4((x), (y), (z), 1.0f)
#define Vector(x, y, z) Vector4((x), (y), (z), 0.0f)

struct Vertex {
	float point[3];
	float color[4];
};

typedef unsigned int Index;

template<typename T>
struct Array{
	T buf[];
	size_t len;
};