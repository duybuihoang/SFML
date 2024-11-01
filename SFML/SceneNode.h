#pragma once
#ifndef SCENENODE_HPP
#define SCENENODE_HPP
#include"Category.h"
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include"CommandQueue.h"
#include<vector>
#include<memory>
#include<set>
#include<utility>
struct Command;
class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	SceneNode(Category::Type category = Category::None);
	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);
	void update(sf::Time dt,CommandQueue& commands);

	virtual unsigned int getCategory()const;
	void onCommand(const Command& command, sf::Time dt);
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;
	void checkSceneCollisions(SceneNode& SceneGraph, std::set<Pair>& collisionPairs);
	void checkNodeCollisions(SceneNode& node, std::set<Pair>& collisionPairs);
	void removeWrecks();
	virtual sf::FloatRect getBoundingRect()const;
	virtual bool isMarkedForRemoval()const;
	virtual bool isDestroyed()const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const ;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt,CommandQueue& commands);
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states)const;
	void updateChildren(sf::Time dt,CommandQueue& commands);
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states)const;
private:
	std::vector<Ptr> mChildren;
	SceneNode* mParent;
	Category::Type mDefaultCategory;
};
bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);
#endif // !SCENENODE_HPP
