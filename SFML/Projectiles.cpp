#include "Projectiles.h"
#include"DataTables.h"
#include"Utility.h"
#include"ResourceHolder.h"
#include<SFML/Graphics.hpp>
#include<cmath>
#include"EmitterNode.h"
#include<cassert>
namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}
Projectiles::Projectiles(Type type, const TextureHolder& textures)
	:Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	,mTargetDirection()
{
	centerOrigin(mSprite);

	if (isGuided())
	{
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));

		std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant));
		propellant->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(propellant));

	}
}
bool::Projectiles::isGuided()const
{
	return mType == Missile  ;
}
void::Projectiles::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}
void::Projectiles::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isGuided())
	{
		const float approachRate = 2000.f;
		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}
	Entity::updateCurrent(dt, commands);

}
void::Projectiles::drawCurrent(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(mSprite, states);
}
unsigned int Projectiles::getCategory()const
{
	if (mType == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}
float::Projectiles::getMaxSpeed()const
{
	return Table[mType].speed;
}
int::Projectiles::getDamage()const
{
	return Table[mType].damage;
}
sf::FloatRect Projectiles::getBoundingRect()const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}