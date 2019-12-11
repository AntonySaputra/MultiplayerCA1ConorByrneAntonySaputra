#include "Entity.hpp"

Entity::Entity()
	: mGravity(500.0f)
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	//mVelocity.y += mGravity;
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	//mVelocity.y += mGravity;
	mVelocity.x += vx;
	mVelocity.y += vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::updateCurrent(sf::Time dt)
{
	accelerate(0.f, mGravity);
	move(mVelocity * dt.asSeconds());
}
