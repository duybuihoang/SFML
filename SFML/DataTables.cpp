#include "DataTables.h"
#include"AirCraft.h"
#include"Projectiles.h"
#include"Particle.h"
using namespace std::placeholders;

std::vector<AircraftData> initializeAircraftData()
{
	std::vector<AircraftData> data(Aircraft::TypeCount);

	data[Aircraft::Eagle].hitpoints = 100;
	data[Aircraft::Eagle].speed = 400.f;
	data[Aircraft::Eagle].fireInterval = sf::seconds(1);
	data[Aircraft::Eagle].texture = Textures::Entities;
	data[Aircraft::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
	data[Aircraft::Eagle].hasRollAnimation = true;


	data[Aircraft::Raptor].hitpoints = 20;
	data[Aircraft::Raptor].speed = 80.f;
	data[Aircraft::Raptor].texture = Textures::Entities;
	data[Aircraft::Raptor].textureRect = sf::IntRect(144, 0, 83, 63);

	data[Aircraft::Raptor].directions.push_back(Direction(90, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(45, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(0, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(-45, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(-90, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(-90 - 45, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(-180, 40));
	data[Aircraft::Raptor].directions.push_back(Direction(-180 - 45, 40));
	data[Aircraft::Raptor].hasRollAnimation = false;

	data[Aircraft::Eagle].fireInterval = sf::seconds(1);


	data[Aircraft::Avenger].hitpoints = 40;
	data[Aircraft::Avenger].speed = 100.f;
	data[Aircraft::Avenger].texture = Textures::Entities;
	data[Aircraft::Avenger].textureRect = sf::IntRect(228, 0, 59, 58);
	data[Aircraft::Avenger].directions.push_back(Direction(0, 70));
	data[Aircraft::Avenger].directions.push_back(Direction(180, 400));
	data[Aircraft::Avenger].directions.push_back(Direction(0, 40));
	data[Aircraft::Avenger].fireInterval = sf::seconds(10);
	data[Aircraft::Avenger].hasRollAnimation = false;
		

	data[Aircraft::Boss2].hitpoints = 500;
	data[Aircraft::Boss2].fireInterval = sf::seconds(5);
	data[Aircraft::Boss2].hasRollAnimation = false;
	data[Aircraft::Boss2].speed = 100.f;
	data[Aircraft::Boss2].texture = Textures::Boss_2;
	data[Aircraft::Boss2].textureRect = sf::IntRect(0, 0, 555, 544);
	data[Aircraft::Boss2].directions.push_back(Direction(0, 120));
	data[Aircraft::Boss2].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss2].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss2].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss2].directions.push_back(Direction(180, 100));



	data[Aircraft::Boss3].hitpoints = 1500;
	data[Aircraft::Boss3].fireInterval = sf::seconds(4);
	data[Aircraft::Boss3].hasRollAnimation = false;
	data[Aircraft::Boss3].speed = 100.f;
	data[Aircraft::Boss3].texture = Textures::Boss_2;
	data[Aircraft::Boss3].textureRect = sf::IntRect(0, 0, 755, 409);
	data[Aircraft::Boss3].directions.push_back(Direction(0, 120));
	data[Aircraft::Boss3].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss3].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss3].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss3].directions.push_back(Direction(180, 100));

	data[Aircraft::Boss4].hitpoints = 3500;
	data[Aircraft::Boss4].fireInterval = sf::seconds(3);
	data[Aircraft::Boss4].hasRollAnimation = false;
	data[Aircraft::Boss4].speed = 100.f;
	data[Aircraft::Boss4].texture = Textures::Boss_2;
	data[Aircraft::Boss4].textureRect = sf::IntRect(0, 0, 755, 409);
	data[Aircraft::Boss4].directions.push_back(Direction(0, 120));
	data[Aircraft::Boss4].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss4].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss4].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss4].directions.push_back(Direction(180, 100));

	data[Aircraft::Boss5].hitpoints = 5000;// :))) trâu như chó :Đ
	data[Aircraft::Boss5].fireInterval = sf::seconds(3);
	data[Aircraft::Boss5].hasRollAnimation = false;
	data[Aircraft::Boss5].speed = 100.f;
	data[Aircraft::Boss5].texture = Textures::Boss_2;
	data[Aircraft::Boss5].textureRect = sf::IntRect(0, 0, 555, 544);
	data[Aircraft::Boss5].directions.push_back(Direction(0, 120));;
	data[Aircraft::Boss5].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss5].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss5].directions.push_back(Direction(180, 100));
	data[Aircraft::Boss5].directions.push_back(Direction(180, 100));


	return data;
}
std::vector<ProjectileData>initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectiles::TypeCount);

	data[Projectiles::AlliedBullet].damage = 10;
	data[Projectiles::AlliedBullet].speed = 1000.f;
	data[Projectiles::AlliedBullet].texture = Textures::Entities;
	data[Projectiles::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

	data[Projectiles::EnemyBullet].damage = 10;
	data[Projectiles::EnemyBullet].speed = 200.f;
	data[Projectiles::EnemyBullet].texture = Textures::Entities;
	data[Projectiles::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);

	data[Projectiles::Missile].damage = 200;
	data[Projectiles::Missile].speed = 200.f;
	data[Projectiles::Missile].texture = Textures::Entities;
	data[Projectiles::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}
std::vector<PickupData>initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::Entities;
	data[Pickup::HealthRefill].action = [](Aircraft& a) {a.repair(25); };
	data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);

	data[Pickup::MissileRefill].texture = Textures::Entities;
	data[Pickup::MissileRefill].action = std::bind(&Aircraft::collectMissiles, _1, 1);
	data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);


	data[Pickup::FireSpread].texture = Textures::Entities;
	data[Pickup::FireSpread].action = std::bind(&Aircraft::increaseSpread, _1);
	data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);


	data[Pickup::FireRate].texture = Textures::Entities;
	data[Pickup::FireRate].action = std::bind(&Aircraft::increaseFireRate, _1);
	data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);

	return data;
}
std::vector<ParticleData>initializeParticleData()
{
	std::vector<ParticleData> data(Particle::ParticleCount);
	data[Particle::Propellant].color = sf::Color(255, 255, 50);
	data[Particle::Propellant].lifetime = sf::seconds(0.6f);

	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].lifetime = sf::seconds(4.f);
	return data;
}
