#include"ComManager.h"

unsigned int ComManager::object_count = 0;

ComManager::ComManager()
{
	if (++object_count == 1)
	{
		THROW_IF_FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	}
}

ComManager::~ComManager()
{
	if (--object_count == 0)
	{
		CoUninitialize();
	}
}
