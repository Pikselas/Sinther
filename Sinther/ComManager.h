#pragma once
#include"WindowsException.h"
class ComManager
{
private:
	static unsigned int object_count;
public:
	ComManager()
	{
		if (++object_count == 1)
		{
			THROW_IF_FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
		}
	}
	~ComManager()
	{
		if (--object_count == 0)
		{
			CoUninitialize();
		}
	}
};

unsigned int ComManager::object_count = 0;