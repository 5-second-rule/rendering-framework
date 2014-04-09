#pragma once

struct Color {
	float r, g, b, a;
};
struct Point {
	float x, y, z;
};
struct Vertex {
	Point point;
	Color color;
};

typedef unsigned int Index;

template<typename T>
struct Array{
	T buf[];
	size_t len;
};