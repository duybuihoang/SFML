#pragma once
#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP
#include"SceneNode.h"
#include<SFML/Graphics.hpp>
class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::Sprite msprite;
};
#endif // !SPRITENODE_HPP
