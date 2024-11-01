#pragma once
#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H
#include<array>
#include<SFML/Graphics.hpp>
#include"State.h"
#include"Player.h"
#include"Container.h"
#include"Button.h"
#include"Label.h"
class SettingsState: public State
{
public:
	SettingsState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);
private:
	void updateLabels();
	void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);
private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, Player::ActionCount> mBindingButtons;
	std::array<GUI::Label::Ptr, Player::ActionCount> mBindingLabels;
};
#endif // !SETTINGSSTATE_H



