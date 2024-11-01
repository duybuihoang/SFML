#include "SoundNode.h"
#include"SoundPlayer.h"
SoundNode::SoundNode(SoundPlayer& player)
	:SceneNode()
	, msounds(player)
{
}
void::SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
	msounds.play(sound, position);
}
unsigned int SoundNode::getCategory()const
{
	return Category::SoundEffect;
}