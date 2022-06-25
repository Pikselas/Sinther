#pragma once
#include<xaudio2.h>
#include<wrl.h>
#include<fstream>
#include"ComManager.h"
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
		WAVEFORMATEX GetFormat() const;
		XAUDIO2_BUFFER GetAudioBuffer() const;
	};
private:
	ComManager com_manager;
	Microsoft::WRL::ComPtr<IXAudio2> audio_engine;
	IXAudio2MasteringVoice * mastering_voice;
public:
	AudioPlayer();
	void Play(const Audio& sound);
};