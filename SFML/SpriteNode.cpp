#include"SpriteNode.h"
SpriteNode::SpriteNode(sf::Texture& texture)
	:msprite(texture)
{
	texture.setSmooth(true);
}
SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
	:msprite(texture,textureRect)
{
	
}
void::SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(msprite, states);
}