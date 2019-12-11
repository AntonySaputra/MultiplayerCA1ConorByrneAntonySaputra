#pragma once
#include "Entity.hpp"
#include "AircraftID.hpp"
#include "ResourceIdentifiers.hpp"
#include "CommandQueue.hpp"

class Aircraft : public Entity
{
public:
	Aircraft(AircraftID type, const TextureHolder& textures);
	virtual unsigned int getCategory() const;
	void jump();

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkIsJumping(sf::Time dt);
	virtual void updateCurrent(sf::Time dt);

private:
	AircraftID mType;
	sf::Sprite mSprite;
	sf::Time mTimeInAir;
	sf::Time mJumpImpulseTime;
	float mJumpImpulseVel;
	float mJumpHangVel;
	sf::Time mMaxAirTime;
	float mMaxVelocity;
	bool mIsJumping;

};