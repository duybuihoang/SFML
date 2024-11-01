#include "Button.h"
#include"Utility.h"
namespace GUI
{
	Button::Button(State::Context context)
		:mCallback()
		,mSprite(context.textures->get(Textures::Buttons))
		,mText("",context.fonts->get(Fonts::Main), 16)
		,mIsToggle(false)
		,mIsavailable(true)
		,mSound(*context.sound)
	{
		changeTexture(Normal);
		sf::FloatRect Bounds = mSprite.getLocalBounds();
		mText.setPosition(Bounds.width / 2.f, Bounds.height / 2.f);
	}
	void Button::setCallBack(CallBack callback)
	{
		mCallback = std::move(callback);
	}
	void Button::setText(const std::string& text)
	{
		mText.setString(text);
		centerOrigin(mText);
	}
	void Button::setToggle(bool flag)
	{
		mIsToggle = flag;
	}
	bool Button::isSelectable()const
	{
		return true;
	}
	void Button::select()
	{
		if (mIsavailable)
		{
			Component::select();
			changeTexture(Selected);
		}
		else
		{
			
		}
	}
	void Button::deselect()
	{
		Component::deselect();
		changeTexture(Normal);
	}
	void Button::activate()
	{
		if (mIsavailable)
		{
			Component::activate();
			if (mIsToggle)
				changeTexture(Pressed);
			if (mCallback)
				mCallback();
			if (!mIsToggle)
				deactivate();
			mSound.play(SoundEffect::Button);
		}
	}
	void Button::deactivate()
	{
		Component::deactivate();
		if (mIsToggle)
		{
			if (isSelected())
				changeTexture(Selected);

			else
				changeTexture(Normal);
			
		}
	}
	void Button::handleEvent(const sf::Event& )
	{
		
	}
	void Button::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		states.transform *= getTransform();
		target.draw(mSprite, states);
		target.draw(mText, states);
	}
	void Button::changeTexture(Type buttonType)
	{
		sf::IntRect textureRect(0, 50* buttonType, 200, 50);
		mSprite.setTextureRect(textureRect);
	}
	void Button::setAvailability(bool flag)
	{
		mIsavailable = flag;
	}
}