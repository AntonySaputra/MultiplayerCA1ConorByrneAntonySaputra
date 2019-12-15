#pragma once
#include "Entity.hpp"
#include "StickmanID.hpp"
#include "ResourceIdentifiers.hpp"
#include "CommandQueue.hpp"

class Stickman : public Entity
{
public:
	Stickman(StickmanID type, const TextureHolder& textures);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;

	void jump();

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkIsJumping(sf::Time dt);
	virtual void updateCurrent(sf::Time dt);

private:
	StickmanID mType;
	sf::Sprite mSprite;
	sf::Time mTimeInAir;
	sf::Time mJumpImpulseTime;
	float mJumpImpulseVel;
	float mJumpHangVel;
	sf::Time mMaxAirTime;
	float mMaxVelocity;
	bool mIsJumping;
	bool mIsMarkedForRemoval;

};