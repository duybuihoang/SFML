#include"GameOverState.h"
#include"Utility.h"
#include"Player.h"
#include"ResourceHolder.h"
#include<SFML/Graphics.hpp>
#include"MusicPlayer.h"
GameOverState::GameOverState(StateStack& stack, Context context)
	:State(stack,context)
	, mGameOverText()
	, mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	
	mGameOverText.setFont(font);
	if (context.player->getMissionStatus() == Player::MissionFailure)
		mGameOverText.setString("Mission Failed!!!");
	else
	{
		mGameOverText.setString("Mission Succecssful!!!");

		std::ifstream ReadLevel;
		ReadLevel.open("data/Level.txt", std::ios_base::in);
		
		unsigned int CurrentLevel;
		ReadLevel>>CurrentLevel;
		if ( CurrentLevel >= Player::Incredible)
		{
			std::ofstream WriteLevel;
			WriteLevel.open("data/Level.txt", std::ios_base::out);
			WriteLevel << Player::Incredible  ;
			getContext().player->setMaxLevel(Player::Incredible);
			WriteLevel.close();
		}
		else
		{
			if (getContext().player->GetCurrentLevel() == CurrentLevel)
			{
				std::ofstream WriteLevel;
				WriteLevel.open("data/Level.txt", std::ios_base::out);
				WriteLevel << CurrentLevel + 1;
				getContext().player->setMaxLevel(static_cast<Player::Level>(CurrentLevel + 1));
				WriteLevel.close();
			}
		}
		ReadLevel.close();
		


	}
	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition
	(0.5f * context.window->getView().getSize().x, 0.5f * context.window->getView().getSize().y);
	
}
void::GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}
bool::GameOverState::update(sf::Time dt)
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += dt;
	//getContext().music->setVolume(getContext().music->getVolume() - 5);
	if (mElapsedTime > sf::seconds(3))
	{
		getContext().music->stop();// khong lam no bi loi j do mac du k anh huong may
		getContext().music->play(Musics::MenuTheme);
		requestStateClear();
		requestStackPush(States::Menu);
	}
	return false;
}
bool::GameOverState::handleEvent(const sf::Event& )
{
	return false;
}