#include"SceneNode.h"
#include<cassert>
#include"Foreach.h"
#include<algorithm>
#include"Command.h"
#include"Utility.h"
SceneNode::SceneNode(Category::Type category)
	: mChildren()
	, mParent(nullptr)
	,mDefaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}


SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;

}

void SceneNode::update(sf::Time dt,CommandQueue&commands)
{
	updateCurrent(dt,commands);
	updateChildren(dt,commands);
}

void SceneNode::updateCurrent(sf::Time,CommandQueue&)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt,CommandQueue& commands)
{
	FOREACH(Ptr & child, mChildren)
		child->update(dt,commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
	//drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	FOREACH(const Ptr & child, mChildren)
		child->draw(target, states);
}
void::SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::FloatRect rect = getBoundingRect();
	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}
sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}
unsigned int::SceneNode::getCategory()const
{
	return mDefaultCategory;
}
void::SceneNode::onCommand(const Command& Command, sf::Time dt)
{
	if (Command.Category & getCategory())
	{
		Command.action(*this, dt);
	}
	FOREACH(Ptr & child, mChildren)
		child->onCommand(Command, dt);   
}
void::SceneNode::checkSceneCollisions(SceneNode& SceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollisions(SceneGraph, collisionPairs);
	FOREACH(Ptr & child, SceneGraph.mChildren)
		checkSceneCollisions(*child, collisionPairs);
}
void::SceneNode::checkNodeCollisions(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));
	FOREACH(Ptr & child, mChildren)
		child->checkNodeCollisions(node, collisionPairs);
}
void::SceneNode::removeWrecks()
{
	// Remove all children which request so

	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());
	//Call function recursively for all remaining children
	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect()const
{
	return sf::FloatRect();
}
bool SceneNode::isMarkedForRemoval()const
{
	// By default, remove node if entity is destroyed
	return isDestroyed();
}
bool SceneNode::isDestroyed()const
{
	// By default, scene node needn't be remove
	return false;
}
bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());

}
float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

