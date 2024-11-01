#include "MenuState.h"
#include"Utility.h"
#include"ResourceHolder.h"
#include"Foreach.h"
#include"Component.h"
#include"Button.h"
#include"MusicPlayer.h"
MenuState::MenuState(StateStack & stack, Context context)
	:State(stack, context)
	,mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);
	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(100, 300);
	playButton->setText("PLAY");
	playButton->setCallBack([this]()
		{
			requestStackPop();
			requestStackPush(States::Level);
		});
	
	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(100, 350);
	settingsButton->setText("SETTINGS");
	settingsButton->setCallBack([this]()
		{
			requestStackPush(States::Settings);
		});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(100, 400);
	exitButton->setText("EXIT");
	exitButton->setCallBack([this]()
		{
			requestStackPop();
			
		});
	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);
	
	
}
bool::MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
bool MenuState::update(sf::Time dt)
{
	return true;
}
void::MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}
