#include"LevelState.h"
#include"Button.h"
#include"Utility.h"
#include"ResourceHolder.h"
#include"Container.h"
#include"label.h"
LevelState::LevelState(StateStack& stack,Context context)
	:State(stack, context)
	, mGUIcontainer()

{

	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	addButton(Player::Easy, 200, "EASY", context);
	addButton(Player::Normal, 300, "NORMAL", context);
	addButton(Player::Challenged, 400, "CHALLENGED", context);
	addButton(Player::Hard, 500, "HARD", context);
	addButton(Player::Incredible, 600, "IMPOSIBLE", context);

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(80.f, 620.f);
	backButton->setText("BACK");
	backButton->setCallBack([&]()
		{
			requestStackPop();
			requestStackPush(States::Menu);
		});
	mGUIcontainer.pack(backButton);

}
bool::LevelState::handleEvent(const sf::Event& event)
{
	for (size_t i = 0; i < Player::LevelCount; i++)
	{
		if (mLevelButton[i]->isSelected())
		{
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->setCurrentLevel(static_cast<Player::Level>(i));
				mLevelButton[i]->setCallBack([this]()
					{
						requestStackPop();
						requestStackPush(States::Game);
					});
				break;
			}
		}
	}
	mGUIcontainer.handleEvent(event);
	return false;
}
bool LevelState::update(sf::Time dt)
{
	return true;
}
void::LevelState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIcontainer);
}
void::LevelState::addButton(Player::Level mlevel, float y,const std::string& text,Context context)
{
	mLevelButton[mlevel] = std::make_shared<GUI::Button>(context);
	mLevelButton[mlevel]->setPosition(400.f, y);
	mLevelButton[mlevel]->setText(text);
	if (mlevel <= context.player->GetMaxLevel())
		mLevelButton[mlevel]->setAvailability(true);
	else
		mLevelButton[mlevel]->setAvailability(false);
	mLevelButton[mlevel]->setToggle(true);

	
	mGUIcontainer.pack(mLevelButton[mlevel]);
}


