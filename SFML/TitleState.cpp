#include "TitleState.h"
#include"ResourceHolder.h"
#include"Utility.h"
#include<fstream>
#include"Player.h"
#include"MusicPlayer.h"
TitleState::TitleState(StateStack& stack,Context context)
	:State(stack,context)
	,mText()
	,mTextEffectTime(sf::Time::Zero)
	,mShowText(true)
{

	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	
	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setString("PRESS ANY KEY TO START");
	centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));

	context.music->play(Musics::MenuTheme);


}
bool::TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed )
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}
	return false;
}
bool::TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}
	return true;
}
void::TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	if (mShowText)
		window.draw(mText);
}
