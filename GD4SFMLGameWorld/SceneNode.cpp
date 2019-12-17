//Antony Saputra D00197615
//Conor Byrne D00185566
#include "SceneNode.hpp"
#include "CategoryID.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <algorithm>
#include <cmath>

SceneNode::SceneNode():mChildren(), mParent(nullptr)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) {return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		transform = node->getTransform() * transform;
	}
	return transform;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for (Ptr& child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for (Ptr& child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

unsigned int SceneNode::getCategory() const
{
	//flag for trouble later on for collision
	return static_cast<int>(CategoryID::Scene);
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	//Command current node, if the category matches
	//Is this command for me?
	if (command.category & getCategory())
	{
		command.action(*this, dt);
	}
	//Send the command on to the children
	for (Ptr& child : mChildren)
	{
		child->onCommand(command, dt);
	}
}

bool SceneNode::isDestroyed() const
{
	return false;
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
	// Do nothing by default
}


void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (const Ptr& child : mChildren)
		child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Apply the transform to the current node
	states.transform *= getTransform();
	//Draw node and the children
	drawCurrent(target, states);
	drawChildren(target, states);

	// Draw bounding rectangle - disabled by default
	drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Ptr& child : mChildren)
	{
		child->draw(target, states);
	}
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const
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

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
