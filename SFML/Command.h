#ifndef COMMAND_HPP
#define COMMAND_HPP

#include<SFML/System.hpp>
#include<functional>
#include<cassert>
class SceneNode;
struct Command
{
	Command();
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int Category;
};
template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return[=](SceneNode& node, sf::Time dt)
	{
		assert(dynamic_cast<GameObject*> (&node) != nullptr);
		fn(static_cast<GameObject&>(node), dt);
	};
}
#endif // !COMMAND_HPP
