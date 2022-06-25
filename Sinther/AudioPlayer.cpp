#include "AudioPlayer.h"

AudioPlayer::Audio::Audio(const std::string& file)
{
	Load(file);
}

void AudioPlayer::Audio::Load(const std::string& file)
{
	std::ifstream audio_file(file, std::ios_base::binary);
	if (audio_file.is_open())
	{
		unsigned int audio_size = 0;
		//First chunk is RIFF
		int fourcc = 0;
		audio_file.read(reinterpret_cast<char*>(&fourcc), 4);
		if (fourcc == 'FFIR')
		{
			//file size at 2nd field;
			unsigned int file_size = 0;
			audio_file.read(reinterpret_cast<char*>(&file_size), 4);
			//does not includes forcc and itself;
			file_size += 8;
			audio_file.seekg(4, std::ios_base::cur);
			bool GotFormat = false;
			bool GotData = false;
			for (auto i = 12u; i < file_size; )
			{
				int chunkType = 0;
				int chunk_size = 0;
				audio_file.read(reinterpret_cast<char*>(&chunkType), 4);
				audio_file.read(reinterpret_cast<char*>(&chunk_size), 4);
				if (chunkType == ' tmf')
				{
					GotFormat = true;
					audio_file.read(reinterpret_cast<char*>(&format), sizeof(format));
				}
				else if (chunkType == 'atad')
				{
					GotData = true;
					audio_data = std::make_unique<BYTE[]>(chunk_size);
					audio_file.read(reinterpret_cast<char*>(audio_data.get()), chunk_size);
					audio_size = chunk_size;
				}
				if (GotFormat && GotData)
				{
					break;
				}
				audio_file.seekg(chunk_size - audio_file.gcount(), std::ios_base::cur);
				//chunk size + chunkID size + chunkSize size
				i += chunk_size + 8;
			}
			if (!GotFormat)
			{
				throw AudioException("UNKNOWN AUDIO FORMAT");
			}
			if (!GotData)
			{
				throw AudioException("NO PLAYABLE DATA IN AUDIO FILE");
			}
		}
		else
		{
			throw AudioException("FILE IS NOT A RIFF FILE");
		}
		audio_buffer = {};
		audio_buffer.AudioBytes = audio_size;
		audio_buffer.pAudioData = audio_data.get();
		audio_buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	else
	{
		throw AudioException("FILE CAN'T BE OPEN");
	}
}

WAVEFORMATEX AudioPlayer::Audio::GetFormat() const
{
	return format;
}

XAUDIO2_BUFFER AudioPlayer::Audio::GetAudioBuffer() const
{
	return audio_buffer;
}

AudioPlayer::Audio::AudioException::AudioException(const std::string& cause) : Exception(cause)
{

}

AudioPlayer::AudioPlayer()
{
	THROW_IF_FAILED(XAudio2Create(&audio_engine));
	THROW_IF_FAILED(audio_engine->CreateMasteringVoice(&mastering_voice));
}

void AudioPlayer::Play(const Audio& sound)
{
	IXAudio2SourceVoice* source_voice = nullptr;
	auto format = sound.GetFormat();
	auto audio_buffer = sound.GetAudioBuffer();
	THROW_IF_FAILED(audio_engine->CreateSourceVoice(&source_voice, &format));
	THROW_IF_FAILED(source_voice->SubmitSourceBuffer(&audio_buffer));
	THROW_IF_FAILED(source_voice->Start());
}
