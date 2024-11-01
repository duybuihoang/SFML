#include"Entity.h"
#include<cassert>
Entity::Entity(int hitpoints)
	:mVelocity()
	, mHitpoints(hitpoints)
{

}
void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}
void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}
sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}
void::Entity::updateCurrent(sf::Time dt,CommandQueue&)
{
	move(mVelocity * dt.asSeconds());
}	
void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}
void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}
int::Entity::getHitpoints()const
{
	return mHitpoints;
}
void::Entity::repair(int points)
{
	assert(points > 0);
	if (mHitpoints + points > 100)
		mHitpoints = 100;
	else
		mHitpoints += points;
}
void::Entity::damage(int points)
{
	//assert(points > 0);
	if (mHitpoints > points)
		mHitpoints -= points;
	else
		mHitpoints = 0;
}
void::Entity::destroy()
{
	mHitpoints = 0;
}
bool::Entity::isDestroyed()const
{
	return mHitpoints <= 0;
}
void::Entity::remove()
{
	destroy();
}
