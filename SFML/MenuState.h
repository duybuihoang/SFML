#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP
#include<SFML/Graphics.hpp>
#include<vector>
#include"State.h"
#include"Container.h"
class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	//void updateOptionText();

	virtual bool handleEvent(const sf::Event& event);


private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	
	
};
#endif // !MENUSTATE_HPP
