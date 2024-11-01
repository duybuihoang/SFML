#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP
#include<SFML/Graphics.hpp>
#include"Entity.h"
#include"ResourceIdentifiers.h"
#include"TextNode.h"
#include"CommandQueue.h"
#include"Projectiles.h"
#include"Animation.h"
#include"DataTables.h"
#include"HealthBar.h"

class Aircraft : public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
		Avenger,
		Boss2,
		Boss3,
		Boss4,
		Boss5,
		TypeCount,
	};

public:
	Aircraft(Type type,const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int getCategory()const;
	virtual sf::FloatRect getBoundingRect()const;
	virtual void remove();
	virtual bool isMarkedForRemoval()const;
	bool isAllied()const;
	float getMaxSpeed()const;

	void increaseFireRate();
	void increaseSpread();
	void collectMissiles(unsigned int count);

	void fire();
	void launchMissile();

	std::vector<Direction> getDirection(Type type);
	void clearDirection();
	void defaultDirection();	
	void push_direction( float angle, float distance);
	void setFireInterval(int time);
	void updateText(TextNode* text);

	Aircraft::Type getType();
	void guideToward(sf::Vector2f position);
	bool isGuided()const;

	void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateMovementPattern(sf::Time dt);
	void checkPickupDrop(CommandQueue& commands);
	void checkProjectilelaunch(sf::Time dt, CommandQueue& commands);
	
	void createBullet(SceneNode& node, const TextureHolder& textures)const;
	void createProjectile(SceneNode& node, Projectiles::Type type, float xOffset, float yOffset,float x_axis, const TextureHolder& textures)const;
	void createPickup(SceneNode& node, const TextureHolder& textures)const;

	void updateRollAnimation();

private:
	Type mtype;
	sf::Sprite msprite;
	Animation mExplosion;
	Command mFireCommand;
	Command mMissileCommand;
	sf::Time mFireCountDown;

	bool misFiring;
	bool mIsLaunchingMissle;
	bool mSpawnPickup;
	bool mShowExplosion;
	bool mPlayedExplosionSound;

	int mFireRateLevel;
	int mSpreadLevel;
	int mMissileAmmo;
	
	Command mDropPickupCommand;

	float mTravelledDistance;
	std::size_t mDirectionIndex;


	sf::Vector2f mTargetDirection;
	bool flag, flag1;
	float mAngle;
};



#endif // !AIRCRAFT_HPP
