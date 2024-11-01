#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include<SFML/System.hpp>
#include<SFML/Audio.hpp>

#include<map>
#include<string>

#include"ResourceHolder.h"
#include"ResourceIdentifiers.h"
class MusicPlayer: private sf::NonCopyable
{
public:
	MusicPlayer();
	void play(Musics::ID theme);
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);
	float getVolume();
private:
	sf::Music mMusic;
	std::map<Musics::ID, std::string> mFilename;
	float mVolume;



};
#endif // !MUSICPLAYER_H



