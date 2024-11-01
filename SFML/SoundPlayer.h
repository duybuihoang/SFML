#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H
#include<SFML/System.hpp>
#include<list>
#include"ResourceIdentifiers.h"
#include"ResourceHolder.h"
#include<SFML/Audio/SoundBuffer.hpp>
#include<SFML/Audio/Sound.hpp>



class SoundPlayer : private sf::NonCopyable
{
public:
	SoundPlayer();
	void play(SoundEffect::ID effect);
	void play(SoundEffect::ID effect, sf::Vector2f position);

	void removeStoppedSounds();
	void setListenerPosition(sf::Vector2f position);
	sf::Vector2f getListenerPosition()const;
private:
	SoundBufferHolder mSoundBuffers;
	std::list<sf::Sound> mSounds;

};
#endif // !SOUNDPLAYER_H



