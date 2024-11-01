#include"Player.h"
#include"CommandQueue.h"
#include"AirCraft.h"
#include"Foreach.h"
#include<iostream>
#include<map>
#include<string>
#include<algorithm>
#include<fstream>
using namespace std::placeholders;

struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity * aircraft.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player()
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::M] = LaunchMissile;


	// Set initial action bindings
	initializeAction();

	// Assign all categories to player's aircraft
	FOREACH(auto& pair, mActionBinding)
		pair.second.Category = Category::PlayerAircraft;

	std::ifstream Reader;
	Reader.open("data/Level.txt", std::ios_base::in);
	unsigned int MaxLevel;
	Reader >> MaxLevel;
	Reader.close();
	if (MaxLevel >= Level::LevelCount)
	{
		std::ofstream WriteLevel;
		WriteLevel.open("data/Level.txt", std::ios_base::out);
		WriteLevel << Level::Incredible;
		WriteLevel.close();
	}
	setMaxLevel(static_cast<Level>(MaxLevel));
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}
	return sf::Keyboard::Unknown;
}
void::Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}
Player::MissionStatus Player::getMissionStatus()const
{
	return mCurrentMissionStatus;
}
void::Player::setMaxLevel(Level level)
{
	mMaxLevel = level;
}
Player::Level Player::GetMaxLevel()const
{
	return mMaxLevel;
}
void::Player::setCurrentLevel(Level level)
{
	mCurrentLevel = level;
}
Player::Level Player::GetCurrentLevel()const
{
	return mCurrentLevel;
}
void Player::initializeAction()
{
	mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-1, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+1, 0.f));
	mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -1));
	mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +1));
	mActionBinding[Fire].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time) {a.fire(); });
	mActionBinding[LaunchMissile].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time) {a.launchMissile(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
		case Fire:
			return true;
		default:
			return false;
	}
}