#ifndef PLAYER_H
#define PLAYER_H
#include"Command.h"
#include<SFML/Window.hpp>
#include<map>
#include"Entity.h"
#include"AirCraft.h"
class CommandQueue;
class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		ActionCount
	};
	enum MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};
	enum Level
	{
		Easy,
		Normal,
		Challenged,
		Hard,
		Incredible,
		LevelCount
		
	};
public:
	Player();
	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);
	sf::Keyboard::Key getAssignedKey(Action action) const;
	void assignKey(Action action, sf::Keyboard::Key key);
	void setMissionStatus(MissionStatus status);
	MissionStatus getMissionStatus()const;
	Level GetMaxLevel()const;
	void setMaxLevel(Level level);
	Level GetCurrentLevel()const;
	void setCurrentLevel(Level level);
private:
	static bool isRealtimeAction(Action action);
	void initializeAction();
private:
	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;
	MissionStatus mCurrentMissionStatus;
	Level mMaxLevel;
	Level mCurrentLevel;
};
#endif // !PLAYER_H
