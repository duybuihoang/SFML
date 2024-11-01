#include"AirCraft.h"
#include"ResourceHolder.h"
#include"DataTables.h"
#include"Utility.h"
#include"CommandQueue.h"
#include<SFML/Graphics.hpp>
#include<cmath>
#include"SoundNode.h"
#include "utility.h"
namespace
{
	std::vector<AircraftData> table = initializeAircraftData();
}

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
	:Entity(table[type].hitpoints)
	, mtype(type)
	, msprite(textures.get(table[mtype].texture), table[type].textureRect)
	, mExplosion(textures.get(Textures::Explosion))
	, mFireCommand()
	, mMissileCommand()
	, mFireCountDown(sf::Time::Zero)
	, misFiring(false)
	, mIsLaunchingMissle(false)
	,mSpawnPickup(false)
	, mShowExplosion(true)
	,mPlayedExplosionSound(false)
	,mFireRateLevel(3)
	,mSpreadLevel(1)
	,mDropPickupCommand()
	,mMissileAmmo(2)
	,mTravelledDistance(0.f)
	,mDirectionIndex(0)
	,mTargetDirection()
	,flag(true)
	,flag1(false)
{

	mExplosion.setFrameSize(sf::Vector2i(256, 256));
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));


	centerOrigin(msprite);
	//centerOrigin(mExplosion);

	mFireCommand.Category = Category::SceneAirLayer;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullet(node, textures);
	};
	mMissileCommand.Category = Category::SceneAirLayer;
	mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectiles::Missile, 0.f, 0.5f,0, textures);
	};

	mDropPickupCommand.Category = Category::SceneAirLayer;
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};
}
void::Aircraft::remove()
{
	Entity::remove();
	mShowExplosion = false;	
}
void::Aircraft::createBullet(SceneNode& node, const TextureHolder& textures)const
{
	Projectiles::Type type = isAllied() ? Projectiles::AlliedBullet : Projectiles::EnemyBullet;
	if ((mtype < Aircraft::Boss2))
	{
		switch (mSpreadLevel)
		{
		case 1:
			createProjectile(node, type, 0.f, 0.5f, 0.f, textures);
			break;
		case 2:
			createProjectile(node, type, -0.33f, 0.33f, 0.f, textures);
			createProjectile(node, type, +0.33f, 0.33f, 0.f, textures);
			break;
		case 3:
			createProjectile(node, type, -0.5f, 0.33f, 0, textures);
			createProjectile(node, type, 0.0f, 0.5f, 0, textures);
			createProjectile(node, type, +0.5f, 0.33f, 0, textures);
			break;
		case 4:
			createProjectile(node, type, -0.15f, 0.5f, 0, textures);
			createProjectile(node, type, -0.33f, 0.33f, 0, textures);
			createProjectile(node, type, 0.33f, 0.33f, 0, textures);
			createProjectile(node, type, +0.15f, 0.5f, 0, textures);
			break;

		}
	}
	else if( mtype >= Aircraft::Boss2 && mtype < Aircraft::TypeCount)
	{
		for (int i = 0; i < mtype * 2 ; i++)
		{
			createProjectile(node, type,randomFloat(-0.5,0.5), 0, randomInt(-45,46), textures);
		}
	
	}
}
void::Aircraft::createProjectile(SceneNode& node, Projectiles::Type type,
	float xOffset, float yOffset,float x_axis, const TextureHolder& textures)const
{
	
	std::unique_ptr<Projectiles> projectile(new Projectiles(type, textures));
	sf::Vector2f offset(
		xOffset * msprite.getGlobalBounds().width,
		yOffset * msprite.getGlobalBounds().height
	);
	
	sf::Vector2f velocity;
	if (isAllied() || mtype >= Aircraft::Boss2)
	{

		velocity.x = projectile->getMaxSpeed() * tan(toRadian(x_axis)) ;
		velocity.y = projectile->getMaxSpeed();
	}
	else
	{
		velocity.x = 0;
		velocity.y = projectile->getMaxSpeed();
	}
	projectile->setRotation(-x_axis);

	float sign = isAllied() ? -1.f : 1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign );
	node.attachChild(std::move(projectile));
	

}
void::Aircraft::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();
	Command command;
	command.Category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>([effect, worldPosition](SoundNode& node, sf::Time)
		{
			node.playSound(effect, worldPosition);
		});

	commands.push(command);
}
void::Aircraft::checkProjectilelaunch(sf::Time dt, CommandQueue& commands)
{
	if (!isAllied())
		fire();
	if (misFiring && mFireCountDown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		playLocalSound(commands, isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);
		mFireCountDown += table[mtype].fireInterval / (mFireRateLevel + 1.f);
		misFiring = false;
	}
	else if (mFireCountDown > sf::Time::Zero)
	{
		mFireCountDown -= dt;
		misFiring = false;
	}
	if (mIsLaunchingMissle)
	{
		commands.push(mMissileCommand);
		playLocalSound(commands, SoundEffect::LaunchMissile);
		mIsLaunchingMissle = false;
		{
			mIsLaunchingMissle = false;
		}
	}
}
void::Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(msprite, states);
}
unsigned int::Aircraft::getCategory()const
{
	if(isAllied())
		return Category::PlayerAircraft;
	else		
		return Category::EnemyAircraft;
	
}
float Aircraft::getMaxSpeed() const
{
	return table[mtype].speed;
}
void::Aircraft::updateMovementPattern(sf::Time dt)
{
	if (!isGuided())
	{
		const std::vector<Direction>& directions = table[mtype].directions;
		if (!directions.empty())
		{
			if (mtype >= Aircraft::Boss2)
			{
				if (mDirectionIndex == 2)
				{
					if (flag)
					{
						table[mtype].directions.erase(table[mtype].directions.begin());
						flag = false;
					}
				}

			}
			if (mTravelledDistance > directions[mDirectionIndex].distance)
			{
				mDirectionIndex = (mDirectionIndex + 1) % directions.size();
				mTravelledDistance = 0.f;
			}
			float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
			float vx = getMaxSpeed() * std::cos(radians);
			float vy = getMaxSpeed() * std::sin(radians);

			setVelocity(vx, vy);
			mTravelledDistance += getMaxSpeed() * dt.asSeconds();
		}
	}
}


void::Aircraft::updateText(TextNode* text)
{

	if (text)
	{
		text->setString(" X " + std::to_string(mMissileAmmo));
	}
	
}
void::Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
		//updateText();
	//std::cout << mDirectionIndex <<" "<< mTravelledDistance << "\n";
	//std::cout << randomFloat(-0.5, 10.2) << "\n";
	if (isDestroyed())
	{
		checkPickupDrop(commands);
		mExplosion.update(dt);
		if (!mPlayedExplosionSound)
		{
			SoundEffect::ID soundeffect = randomInt(2) == 0 ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
			playLocalSound(commands, soundeffect);
			mPlayedExplosionSound = true;
		}
		return;
	}
	checkProjectilelaunch(dt,commands);
	updateRollAnimation();
	updateMovementPattern(dt);
	if (isGuided())
	{
		flag1 = true;
		const float approachRate = 500.f;
		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		//float angle = std::atan2(newVelocity.y, newVelocity.x);
		//setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
		//std::cout << newVelocity.y << "\n";
	}
	if (flag1)
	{
		clearDirection();
		table[Aircraft::mtype].directions.push_back(Direction(0, 120));
		table[Aircraft::mtype].directions.push_back(Direction(180, 100));
		table[Aircraft::mtype].directions.push_back(Direction(180, 100));
		table[Aircraft::mtype].directions.push_back(Direction(180, 100));
		table[Aircraft::mtype].directions.push_back(Direction(180, 100));
		//std::cout << 1;
		flag = true;
		flag1 = true;

	}
	Entity::updateCurrent(dt, commands);
}
bool::Aircraft::isMarkedForRemoval()const
{
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}
sf::FloatRect Aircraft::getBoundingRect()const
{
	return getWorldTransform().transformRect(msprite.getGlobalBounds());
}
bool::Aircraft::isAllied()const
{
	return mtype == Eagle;
}
void::Aircraft::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}
void::Aircraft::increaseSpread()
{
	if (mSpreadLevel < 4)
		++mSpreadLevel;
}
void::Aircraft::collectMissiles(unsigned int count)
{
	mMissileAmmo += count;
}
void::Aircraft::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (table[mtype].fireInterval != sf::Time::Zero)
		misFiring = true;
}
void::Aircraft::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissle = true;
		--mMissileAmmo;
	}
}
void::Aircraft::checkPickupDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(7) == 0 && !mSpawnPickup)
		commands.push(mDropPickupCommand);
	mSpawnPickup = true;
}
void Aircraft::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));
		
	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}
void::Aircraft::updateRollAnimation()
{
	if (table[mtype].hasRollAnimation)
	{
		sf::IntRect textureRect = table[mtype].textureRect;
		if (getVelocity().x < 0.f)
			textureRect.left += textureRect.width;
		else if (getVelocity().x > 0.f)
			textureRect.left += 2 * textureRect.width;
		msprite.setTextureRect(textureRect);

	}
}
Aircraft::Type Aircraft::getType()
{
	return mtype;
}


std::vector<Direction> Aircraft::getDirection(Type type)
{
	return table[type].directions;
	
}
void Aircraft::clearDirection()
{
	table[mtype].directions.clear();
	//table[mtype].directions.push_back(Direction(0, 0));
}
void Aircraft::defaultDirection()
{

	

}
void::Aircraft::push_direction( float angle, float distance)
{
	table[mtype].directions.push_back(Direction(angle, distance));
}

void::Aircraft::setFireInterval(int time)
{
	table[Aircraft::Avenger].fireInterval = sf::seconds(time);
}

bool::Aircraft::isGuided()const
{
	return mtype >= Type::Boss2 && (getPosition().y <= 1000) || mtype >=Type::Boss2 && getHitpoints() <= 100;
}
void::Aircraft::guideToward(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());

}

