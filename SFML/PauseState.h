#ifndef PAUSESTATE_H
#define PAUSESTATE_H
#include<SFML/Graphics.hpp>
#include"State.h"
#include"Container.h"
class PauseState: public State
{
public: 
	PauseState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::Sprite mBackgroundSprite;
	sf::Text mPausedText;
	GUI::Container mGUIContainer;
};
#endif // !PAUSESTATE_H



