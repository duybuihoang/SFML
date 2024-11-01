#ifndef LEVELSTATE
#define LEVELSTATE

#include<array>
#include<SFML/Graphics.hpp>
#include"State.h"
#include"Player.h"
#include"Container.h"
#include"Button.h"
class LevelState : public State
{
public:
	LevelState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	void addButton(Player::Level mlevel, float y,const std::string& text, Context context);
private:
	GUI::Container mGUIcontainer;
	sf::Sprite mBackgroundSprite;
	std::array<GUI::Button::Ptr,Player::LevelCount> mLevelButton;
	std::array<int, Player::LevelCount> mSelectedLevel;
};

#endif // !LEVELSTATE


