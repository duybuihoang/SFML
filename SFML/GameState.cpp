#include "GameState.h"
#include"MusicPlayer.h"
GameState::GameState(StateStack& stack, Context context)
	:State(stack, context)
	,  mPlayer(*context.player)
	, mWorld(*context.window,*context.fonts, *context.player,*context.sound)
	
{
	mPlayer.setMissionStatus(Player::MissionRunning);
	//context.music->
	context.music->play(Musics::MissionTheme);
}
void::GameState::draw()
{	
	mWorld.draw();
}
bool::GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setMissionStatus(Player::MissionFailure);
		requestStackPush(States::GameOver);
	}
	else if (mWorld.hasPlayerReachedEnd())
	{
		mPlayer.setMissionStatus(Player::MissionSuccess);
		requestStackPush(States::GameOver);
	}
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);
	return true;
}
bool::GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return true;
}
