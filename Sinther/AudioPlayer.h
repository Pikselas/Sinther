#pragma once
#include<xaudio2.h>
#include<fstream>
#include"WindowsException.h"
class AudioPlayer
{
public:
	class Audio
	{
	public:
		class AudioException : public Exception
		{
		public:
			AudioException(const std::string& cause);
		};
	private:
		WAVEFORMATEX format;
		XAUDIO2_BUFFER audio_buffer;
		std::unique_ptr<BYTE[]> audio_data;
	public:
		Audio(const std::string& file);
		void Load(const std::string& file);
	};
};