#pragma once
#include<Windows.h>
#include<comdef.h>
#include<source_location>
#include<exception>
#include<sstream>
class WindowsException : public std::exception
{
private:
	std::string what_buffer;
public:
#if defined(_DEBUG)
	WindowsException(HRESULT errorcode , std::source_location s = std::source_location::current())
	{
		std::stringstream s_stream;
		s_stream << _com_error(errorcode).ErrorMessage()
			<< "THROWN FROM : " << s.file_name()
			<< "ON	: " << s.line();
		what_buffer = s_stream.str();
	}
#else
	WindowsException(HRESULT errorcode)
	{
		what_buffer = _com_error(errorcode).ErrorMessage();
	}
#endif
	const char* what() const override
	{
		return what_buffer.c_str();
	}
};

#define THROW_IF_FAILED(func) if(auto hr = func ; FAILED(hr)) throw WindowsException(hr)