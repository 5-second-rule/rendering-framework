#pragma once
class Buffer
{
private:
	size_t num;

public:
	Buffer(size_t);
	~Buffer();

	virtual void set() = 0;
	virtual size_t count();
};

