#pragma once
#include<Windows.h>
#include<comdef.h>
#include"Exception.h"
class WindowsException : public Exception
{
public:
	WindowsException(HRESULT errorcode) : Exception(_com_error(errorcode).ErrorMessage()){}
};

#define THROW_IF_FAILED(func) if(auto hr = func ; FAILED(hr)) throw WindowsException(hr)