#include<thread>
#include<iostream>
#include"AudioPlayer.h"

int main()
{
	try
	{
		AudioPlayer::Audio audio1("./musics/Bamboo.wav");
		AudioPlayer::Audio audio2("./musics/forest_bird.wav");
		AudioPlayer::Audio audio3("./musics/little_bird.wav");
		
		AudioPlayer player;

		player.Play(audio2);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		player.Play(audio1);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		player.Play(audio3);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	catch (const Exception& ex)
	{
		std::cout << ex.what();
	}
	return 0;
}