//Antony Saputra D00197615
//Conor Byrne D00185566

#pragma once
#include "Entity.hpp"
#include "StickmanID.hpp"
#include "ResourceIdentifiers.hpp"
#include "CommandQueue.hpp"
#include "Animation.hpp"

class Stickman : public Entity
{
public:
	Stickman(StickmanID type, const TextureHolder& textures);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;

	void jump();
	void setTimeInAir(sf::Time time);


	void punch();
	void setPunchingStatus(bool punchStatus);
	bool isGetPunch();
	void getPunch();
	void setPunchDirection(float punchDirection);

	//might be useful for animation - keep it for now
	bool isPunching();
	void setDirection(float direction);
	float getDirection();

	void playerLocalSound(CommandQueue& commands, SoundEffectID effect);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void checkIsJumping(sf::Time dt , CommandQueue& commands);
	void checkIsPunched(sf::Time dt, CommandQueue& commands);
	void punchReset(sf::Time dt, CommandQueue& commands);
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

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
	float mPunchDirectionMultiplier;

	//might be useful for animation - keep it for now
	float mFacingDirection;

	Animation mRunningRight;
	Animation mRunningLeft;
	Animation mJump;
	Animation mPunchRight;
	Animation mPunchLeft;
	Animation mGetPunchRight;
	Animation mGetPunchLeft;

};