//Antony Saputra D00197615
//Conor Byrne D00185566

#pragma once
#include "SFML/Graphics.hpp"
#include "SceneNode.hpp"
#include "CommandQueue.hpp"

class Entity : public SceneNode
{
public:
	Entity(bool isStatic);

	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);
	sf::Vector2f getVelocity() const;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Vector2f mVelocity;
	float mGravity;
	bool mIsStatic;
};