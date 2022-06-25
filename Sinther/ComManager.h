#pragma once
#include"WindowsException.h"
class ComManager
{
private:
	static unsigned int object_count;
public:
	ComManager();
	~ComManager();
};
