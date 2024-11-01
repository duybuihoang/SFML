#ifndef PARTICLE_H
#define PARTICLE_H
#include<SFML/System.hpp>
#include<SFML/Graphics/Color.hpp>
struct Particle
{
	enum Type
	{
		Propellant,
		Smoke,
		ParticleCount,
	};
	sf::Vector2f position;
	sf::Color color;
	sf::Time lifetime;
};
#endif // !PARTICLE_H
