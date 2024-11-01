#ifndef HEALTHBAR_H
#define HEALTHBAR_H
#include<SFML/Graphics.hpp>
#include"SceneNode.h"
class HealthBar: public SceneNode
{	
public:
	HealthBar(float Health);
	void setBarSize(int hitpoint);
	float getRatio();
	float getBarSize();
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states)const;
private:
	//sf::RectangleShape mHealthBarBackground;
	sf::RectangleShape mHealthBar;
	sf::RectangleShape mBackgroundBar;
	float mMaxHP;
	float mHPbar;
	float mHPratio;
	
};
#endif // !HEALTHBAR_H


