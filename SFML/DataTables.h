#ifndef DATATABLES_H
#define DATATABLES_H
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<vector>
#include<functional>
#include"ResourceHolder.h"
#include"ResourceIdentifiers.h"
#include"Pickup.h"
class Aircraft;

struct Direction
{
	Direction(float angle, float distance)
		:angle(angle)
		, distance(distance)
	{
	}
	float angle;
	float distance;
};
struct AircraftData
{
	int hitpoints;
	float speed;
	Textures::ID  texture;
	sf::IntRect textureRect;
	std::vector<Direction> directions;
	sf::Time fireInterval;

	bool hasRollAnimation;
};
struct ProjectileData
{
	int damage;
	float speed;
	Textures::ID texture;
	sf::IntRect textureRect;
};
struct PickupData
{
	std::function<void(Aircraft&)>action;
	Textures::ID texture;
	sf::IntRect textureRect;
};
struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};
std::vector<AircraftData> initializeAircraftData();
std::vector<ProjectileData>initializeProjectileData();
std::vector<PickupData>initializePickupData();
std::vector<ParticleData>initializeParticleData();
#endif // !DATATABLES_H




