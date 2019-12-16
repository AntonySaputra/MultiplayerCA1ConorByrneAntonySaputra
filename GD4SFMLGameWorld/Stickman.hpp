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
	

	void punch();
	void setPunchingStatus(bool punchStatus);
	void getPunch();
	bool isPunching();

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkIsJumping(sf::Time dt);
	void checkIsPunched(sf::Time dt);
	void punchReset(sf::Time dt);
	virtual void updateCurrent(sf::Time dt);

private:
	float mDamageMultiplier;
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
	bool mIsPunching;
	bool mIsGetPunched;
	sf::Time mPunchInterval;
	sf::Time mPunchCountUp;
	sf::Time mPunchTime;
	sf::Time mPunchImpulseTime;
	sf::Time mMaxPunchTime;
	float mPunchImpulseVel;
	float mPunchHangVel;
};