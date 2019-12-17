#include "Stickman.hpp"
#include "ResourceHolder.hpp"
#include "CategoryID.hpp"
#include "DataTable.hpp"
#include "TextureID.hpp"
#include "Utility.hpp"

#include <iostream>	

namespace
{
	const std::vector<StickmanData> Table = initializeStickmanData();
}


TextureID toTextureID(StickmanID type)
{
	switch (type)
	{

	case StickmanID::BlueStickman:
		return TextureID::BlueStick;
	case StickmanID::RedStickman:
		return TextureID::RedStick;
	}
	return TextureID::BlueStick;
}

Stickman::Stickman(StickmanID type, const TextureHolder& textures)
	: Entity(false)
	, mDamageMultiplier(Table[static_cast<int>(mType)].damageMultiplier)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(mType)].texture))
	, mRunningRight(textures.get(TextureID::RunningRight))
	, mTimeInAir(sf::Time::Zero)
	, mPunchTime(sf::Time::Zero)
	, mJumpImpulseTime(sf::seconds(0.3f))
	, mPunchImpulseTime(mJumpImpulseTime)
	, mJumpImpulseVel(-1300.f)
	, mPunchImpulseVel(-0.5f * mJumpImpulseVel)
	, mJumpHangVel(-300.0f)
	, mPunchHangVel(-mJumpHangVel)
	, mMaxAirTime(Table[static_cast<int>(mType)].maxAirTime)
	, mMaxPunchTime(0.5f * mMaxAirTime)
	, mMaxVelocity(-10.0f)
	, mIsJumping(false)
	, mIsMarkedForRemoval(false)
	, mIsPunching(false)
	, mIsGetPunched(false)
	, mPunchCountUp(sf::Time::Zero)
	, mPunchInterval(sf::milliseconds(1))
	, mPunchDirectionMultiplier(0.0f)
	, mFacingDirection(0.0f)
{

	mRunningRight.setFrameSize(sf::Vector2i(70, 90));
	mRunningRight.setNumFrames(6);
	mRunningRight.setDuration(sf::seconds(0.5));
	mRunningRight.setRepeating(true);


	centreOrigin(mRunningRight);
	centreOrigin(mSprite);
}

unsigned int Stickman::getCategory() const
{
	switch (mType)
	{

	case StickmanID::BlueStickman:
		return static_cast<int>(CategoryID::PlayerStickman1);
	case StickmanID::RedStickman:
		return static_cast<int>(CategoryID::PlayerStickman2);
	default:
		return static_cast<int>(CategoryID::EnemyAircraft);
	}
}

sf::FloatRect Stickman::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Stickman::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Stickman::jump()
{
	if (!mIsJumping && mTimeInAir == sf::Time::Zero)
		mIsJumping = true;

}

void Stickman::setTimeInAir(sf::Time time)
{
	mTimeInAir = time;
}

void Stickman::punchReset(sf::Time dt)
{
	if (mIsPunching)
	{
		mPunchCountUp += dt;
		if (mPunchCountUp >= mPunchInterval)
		{
			mIsPunching = false;
		}
	}
}

void Stickman::punch()
{
	mIsPunching = true;
}

void Stickman::setPunchingStatus(bool punchStatus)
{
	mIsPunching = punchStatus;
}

bool Stickman::isGetPunch()
{
	return mIsGetPunched;
}

void Stickman::getPunch()
{
	mIsGetPunched = true;
}

bool Stickman::isPunching()
{
	return mIsPunching;
}

void Stickman::setDirection(float direction)
{
	mFacingDirection = direction;
}

float Stickman::getDirection()
{
	return mFacingDirection;
}

void Stickman::setPunchDirection(float punchDirection)
{
	mPunchDirectionMultiplier = punchDirection;
}



void Stickman::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (getVelocity().x > 0)
		target.draw(mRunningRight, states);
	else
		target.draw(mSprite, states);
}


// CheckIsJumping solution is inspired by user AndrewEkeren
// URL of original code: https://gamedev.stackexchange.com/questions/121389/how-i-can-make-better-jump-to-my-game-c-sfml
// The logic is exactly the same, the implementation is customize in order work with derived action
// Logic: when the stickman is jumping, the mTimeInAir will start counting up
// the jump will consist of 3 part, Impulse (jumping fast), Hanging (slowly goes to the peak), then falling down
// each phase will start if timeInAir is reach the timing condition (if less than impulse time, do impulse, after that do hang, after that do fall)
void Stickman::checkIsJumping(sf::Time dt)
{

	if (mIsJumping)
	{
		//Jump impulse: jump very fast first
		mTimeInAir += dt;
		if (mTimeInAir < mJumpImpulseTime)
		{
			std::cout << "Jumping Impulse" << std::endl;
			accelerate(0.f, mJumpImpulseVel);
		}
		else if (mTimeInAir < mMaxAirTime)
		{
			std::cout << "Jumping Accel" << std::endl;
			accelerate(0.f, mJumpHangVel);
		}
		else
		{
			std::cout << "stop jumping" << std::endl;
			mIsJumping = false;
			//mTimeInAir = sf::Time::Zero;
		}
	}


}



void Stickman::checkIsPunched(sf::Time dt)
{
	if (mIsGetPunched)
	{
		mPunchTime += dt;
		if (mPunchTime < mPunchImpulseTime * mDamageMultiplier)
		{
			std::cout << "Jumping Impulse" << std::endl;
			accelerate(mPunchImpulseVel * mPunchDirectionMultiplier, 0.f);
		}
		else if (mPunchTime < mMaxPunchTime * mDamageMultiplier)
		{
			std::cout << "Jumping Accel" << std::endl;
			accelerate(mPunchHangVel * mPunchDirectionMultiplier, 0.f);
		}
		else
		{
			mDamageMultiplier += 0.5f;
			std::cout << "stop jumping" << std::endl;
			mIsGetPunched = false;
			mPunchTime = sf::Time::Zero;
		}


	}
}

void Stickman::updateCurrent(sf::Time dt)
{

	if (getVelocity().x > 0)
	{
		mRunningRight.update(dt);
	}

	checkIsJumping(dt);
	checkIsPunched(dt);
	punchReset(dt);
	Entity::updateCurrent(dt);
}
