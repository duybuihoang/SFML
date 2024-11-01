#ifndef STATE_H
#define STATE_H
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include"ResourceIdentifiers.h"
#include"StateIdentifiers.h"
#include<memory>
namespace sf
{
	class RenderWindow;
}
class StateStack;
class Player;
class MusicPlayer;
class SoundPlayer;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;
	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
			Player& player, MusicPlayer& music, SoundPlayer& sound);
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder*	 fonts;
		Player* player;
		MusicPlayer* music;
		SoundPlayer* sound;
	};
public:
	State(StateStack& stack, Context context);
	virtual ~State();
	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;
protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();
	Context getContext() const;
private:
	StateStack* mStack;
	Context mContext;

};


#endif // !STATE_H
