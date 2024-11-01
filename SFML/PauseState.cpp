#include "PauseState.h"
#include"Utility.h"
#include"ResourceHolder.h"
#include"Button.h"
PauseState::PauseState(StateStack& stack, Context context)
	:State(stack, context)
	, mPausedText()
	, mBackgroundSprite()
	, mGUIContainer()
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getView().getSize().x, context.window->getView().getSize().y);

	mPausedText.setFont(font);
	mPausedText.setString("Game Paused");
	mPausedText.setCharacterSize(70);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	auto returnButton = std::make_shared<GUI::Button>(context);
	returnButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	returnButton->setText("return");
	returnButton->setCallBack([this]()
		{
			requestStackPop();
		});
	auto backToMenuButton = std::make_shared<GUI::Button>(context);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backToMenuButton->setText("back to menu");
	backToMenuButton->setCallBack([this]()
		{
			requestStateClear();
			requestStackPush(States::Menu);
		});
	mGUIContainer.pack(returnButton);
	mGUIContainer.pack(backToMenuButton);
}
void::PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
	window.draw(mGUIContainer);
}
bool::PauseState::update(sf::Time )
{
	return false;
}
bool::PauseState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}