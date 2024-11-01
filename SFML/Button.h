#ifndef BUTTON_H
#define BUTTON_H

#include"Component.h"
#include"ResourceIdentifiers.h"
#include"ResourceHolder.h"

#include<SFML/Graphics.hpp>
#include<memory>
#include<functional>
#include<vector>
#include<string>
#include"SoundPlayer.h"
#include"State.h"
namespace GUI
{
	class Button: public Component
	{
	public:
		typedef std::shared_ptr<Button> Ptr;
		typedef std::function<void()> CallBack;

		enum Type
		{
			Normal,
			Selected,
			Pressed,
			ButtonCount
		};

	public:
		Button(State::Context context);
		void setCallBack(CallBack callback);
		void setText(const std::string& text);
		void setToggle(bool flag);
		void setAvailability(bool flag);
		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void activate();
		virtual void deactivate();
		virtual void handleEvent(const sf::Event& event);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
		void changeTexture(Type buttonType);
	private:
		CallBack mCallback;
		sf::Sprite mSprite;
		sf::Text mText;
		SoundPlayer& mSound;
		bool mIsToggle;
		bool mIsavailable;
	};
}
#endif // !BUTTON_H


