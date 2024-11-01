#ifndef ENTITY_HPP
#define ENTITY_HPP

#include"SceneNode.h"

class Entity: public SceneNode
{
public:
	explicit Entity(int hitpoints);
	void repair(int hitpoints);
	void damage(int hitpoints);
	void destroy();
	bool isDestroyed()const;
	int getHitpoints()const;


	void setVelocity(sf::Vector2f vector);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity() const;
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);

	virtual void remove();
protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	int mHitpoints;
	sf::Vector2f mVelocity;

};
#endif // !ENTITY_HPP
