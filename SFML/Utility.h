#pragma once
#ifndef UTILITY_H
#define UTILITY_H
#include<SFML/Graphics.hpp>
#include<sstream>
#include<string>
namespace sf
{
	class Sprite;
	class Text;
}
template<typename T>
std::string tostring(const T& value);
//convert enumerator to string
std::string tostring(sf::Keyboard::Key Key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
float toDegree(float radian);
float toRadian(float degree);

int randomInt(int exclusiveMax);
int randomInt(int exclusiveMin,int exclusiveMax);
float randomFloat(float exclusiveMin, float exclusiveMax);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);


#endif // !UTILITY_H
