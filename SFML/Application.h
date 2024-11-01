#ifndef APPLICATION_H
#define APPLICATION_H
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<sstream>
#include"HealthBar.h"
#include"Utility.h"
#include"StateStack.h"
#include"Player.h"
#include"ResourceHolder.h"
#include"ResourceIdentifiers.h"
#include"StateIdentifiers.h"
#include"TitleState.h"
#include"MenuState.h"
#include"PauseState.h"
#include"GameState.h"
#include"LoadingState.h"
#include"SettingsState.h"
#include"GameOverState.h"
#include"LevelState.h"
#include"MusicPlayer.h"
#include"SoundPlayer.h"
#include"Utility.inl"
class Application
{
public: 
	Application();
	void run();
private:
	void processInput();
	void update(sf::Time dt);	
	void render();

	void updateStatistics(sf::Time dt);
	void registerStates();
private:
	static const sf::Time TimePerFrame;
	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	Player mPlayer;
	MusicPlayer mMusic;
	SoundPlayer mSound;

	StateStack mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;

	
};
#endif // !APPLICATION_H
