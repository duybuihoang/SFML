#include "HealthBar.h"
#include"Entity.h"
#include"Utility.h"
HealthBar::HealthBar(float health)
	:mHealthBar()
	,mHPbar(health*3)
	,mMaxHP(health)
	,mHPratio(mMaxHP/mHPbar)
	,mBackgroundBar()

{
	mHealthBar.setFillColor(sf::Color::Red); 
	mBackgroundBar.setFillColor(sf::Color::White);
	mBackgroundBar.setPosition(-2.f,-2.f);
	//centerOrigin(mHealthBar);
}
void::HealthBar::drawCurrent(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(mBackgroundBar, states);
	target.draw(mHealthBar, states);
}	
void::HealthBar::setBarSize(int hitpoint)
{
	mHealthBar.setSize(sf::Vector2f(hitpoint/mHPratio,10));
	if(hitpoint > 0)
		mBackgroundBar.setSize(sf::Vector2f(hitpoint / mHPratio+4, 14));
	else
		mBackgroundBar.setSize(sf::Vector2f(0, 0));

}
float::HealthBar::getRatio()
{
	return mHPratio;
}
float::HealthBar::getBarSize()
{
	return mHPbar;
}
