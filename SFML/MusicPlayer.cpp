#include "MusicPlayer.h"
#include<SFML/Audio.hpp>
MusicPlayer::MusicPlayer()
	:mMusic()
	, mVolume(100.f)
	, mFilename()
{
	mFilename[Musics::MenuTheme] = "data/ES_Glitching Through the Sky - William Benckert.wav";
	mFilename[Musics::MissionTheme] = "data/tunetank.com_5611_vacation_by_rednote.wav";
}
void::MusicPlayer::play(Musics::ID theme)
{
	std::string filename = mFilename[theme];
	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");
	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void::MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}
void::MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}
float::MusicPlayer::getVolume()
{
	return mVolume;
}
void::MusicPlayer::stop()
{
	mMusic.stop();
}