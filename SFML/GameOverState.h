#ifndef GAMEOVERSTATE
#define GAMEOVERSTATE
#include"State.h"
#include"Container.h"
#include<SFML/Graphics.hpp>
#include<fstream>
class GameOverState: public State
{
public:
	GameOverState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::Text mGameOverText;
	sf::Time mElapsedTime;
};
#endif // !GAMEOVERSTATE



