
#include<Windows.h>
#include<comdef.h>
#include<xaudio2.h>
#include<wrl.h>
#include<memory>

#include<iostream>
#include<fstream>
#include<thread>

void LoadMusic(std::string filename)
{
	std::ifstream audio_file(filename, std::ios_base::binary);
	if (audio_file.is_open())
	{
		WAVEFORMATEX format = {};
		std::unique_ptr<BYTE[]> audio_data;
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
			for (auto i = 12u; i < file_size ; )
			{
				int chunkType = 0;
				int chunk_size = 0;
				audio_file.read(reinterpret_cast<char*>(&chunkType), 4);
				audio_file.read(reinterpret_cast<char*>(&chunk_size), 4);
				std::cout << audio_file.tellg() << '\n';
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
				std::cout << audio_file.tellg() << '\n';
				if (GotFormat && GotData)
				{
					break;
				}
				audio_file.seekg(chunk_size - audio_file.gcount() , std::ios_base::cur);
				std::cout << audio_file.tellg();
				//chunk size + chunkID size + chunkSize size
				i += chunk_size + 8;
			}
			audio_file.close();
			Microsoft::WRL::ComPtr<IXAudio2> audio_pipeline;
			if (!FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
			{
				(XAudio2Create(&audio_pipeline, 0, XAUDIO2_DEFAULT_PROCESSOR));
				IXAudio2MasteringVoice* master;
				(audio_pipeline->CreateMasteringVoice(&master));

				XAUDIO2_BUFFER audio_buffer = {};
				audio_buffer.AudioBytes = audio_size;
				audio_buffer.pAudioData = audio_data.get();
				audio_buffer.Flags = XAUDIO2_END_OF_STREAM;

				IXAudio2SourceVoice* source;
				(audio_pipeline->CreateSourceVoice(&source, &format));

				if (auto hr = source->SubmitSourceBuffer(&audio_buffer); FAILED(hr))
				{
					_com_error error(hr);
					std::cout << error.ErrorMessage();
				}
				(source->Start());
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				CoUninitialize();
			}
		}
	}
}

void Load(std::string filename)
{
	std::ifstream fl(filename , std::ios_base::binary);
	if (fl.is_open())
	{
		WAVEFORMATEX format = {};
		int fourcc;
		fl.read(reinterpret_cast<char*>(&fourcc), 4);
		unsigned int size;
		fl.read(reinterpret_cast<char*>(&size), 4);
		fl.seekg(4, std::ios_base::cur);
		fl.read(reinterpret_cast<char*>(&size), 4);
		if (size == ' tmf')
		{
			fl.seekg(4, std::ios_base::cur);
			fl.read(reinterpret_cast<char*>(&format), sizeof(format));
		}
		//std::cout << fl.tellg();
		fl.seekg(-2, std::ios_base::cur);
		fl.read(reinterpret_cast<char*>(&size), 4);
		if (size == 'atad')
		{
			fl.read(reinterpret_cast<char*>(&size), 4);
			auto audio_data = new BYTE[size];
			fl.read(reinterpret_cast<char*>(audio_data), size);
			std::cout << fl.tellg() << " " << fl.gcount();
			Microsoft::WRL::ComPtr<IXAudio2> audio_pipeline;
			if (!FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
			{
				(XAudio2Create(&audio_pipeline, 0, XAUDIO2_DEFAULT_PROCESSOR));
				IXAudio2MasteringVoice* master;
				(audio_pipeline->CreateMasteringVoice(&master));
				
				XAUDIO2_BUFFER audio_buffer = {};
				audio_buffer.AudioBytes = size;
				audio_buffer.pAudioData = audio_data;
				audio_buffer.Flags = XAUDIO2_END_OF_STREAM;

				IXAudio2SourceVoice* source;
				(audio_pipeline->CreateSourceVoice(&source , &format));

				if (auto hr = source->SubmitSourceBuffer(&audio_buffer) ; FAILED(hr))
				{
					_com_error error(hr);
					std::wcout << error.ErrorMessage();
				}
				(source->Start(0));
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				CoUninitialize();
			}
		}
	}
}


int main()
{
	//std::cout << sizeof(unsigned long);
	LoadMusic("./musics/Bamboo.wav");
	return 0;
}