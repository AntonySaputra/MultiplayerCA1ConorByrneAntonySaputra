#include "Stickman.hpp"
#include "ResourceHolder.hpp"
#include "CategoryID.hpp"
#include "DataTable.hpp"
#include "TextureID.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "SoundNode.hpp"

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
	, mRunningRight(textures.get(Table[static_cast<int>(mType)].runRight))
	, mRunningLeft(textures.get(Table[static_cast<int>(mType)].runLeft))
	, mJump(textures.get(Table[static_cast<int>(mType)].jump))
	, mPunchRight(textures.get(Table[static_cast<int>(mType)].punchRight))
	, mPunchLeft(textures.get(Table[static_cast<int>(mType)].punchLeft))
	, mGetPunchRight(textures.get(Table[static_cast<int>(mType)].getPunchRight))
	, mGetPunchLeft(textures.get(Table[static_cast<int>(mType)].getPunchLeft))
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
	, mPunchInterval(Table[static_cast<int>(mType)].punchInterval)
	, mPunchDirectionMultiplier(0.0f)
	, mFacingDirection(+1.0f)
{

	mRunningRight.setFrameSize(sf::Vector2i(70, 90));
	mRunningRight.setNumFrames(6);
	mRunningRight.setDuration(sf::seconds(0.5));
	mRunningRight.setRepeating(true);

	mRunningLeft.setFrameSize(sf::Vector2i(70, 90));
	mRunningLeft.setNumFrames(6);
	mRunningLeft.setDuration(sf::seconds(0.5));
	mRunningLeft.setRepeating(true);

	mJump.setFrameSize(sf::Vector2i(60, 90));
	mJump.setNumFrames(1);
	mJump.setDuration(sf::seconds(1.2f));
	mJump.setRepeating(true);

	mPunchRight.setFrameSize(sf::Vector2i(60, 90));
	mPunchRight.setNumFrames(3);
	mPunchRight.setDuration(sf::seconds(0.5f));
	mPunchRight.setRepeating(true);

	mPunchLeft.setFrameSize(sf::Vector2i(60, 90));
	mPunchLeft.setNumFrames(3);
	mPunchLeft.setDuration(sf::seconds(0.5f));
	mPunchLeft.setRepeating(true);

	mGetPunchRight.setFrameSize(sf::Vector2i(60, 90));
	mGetPunchRight.setNumFrames(1);
	mGetPunchRight.setDuration(sf::seconds(0.5f));
	mGetPunchRight.setRepeating(true);

	mGetPunchLeft.setFrameSize(sf::Vector2i(60, 90));
	mGetPunchLeft.setNumFrames(1);
	mGetPunchLeft.setDuration(sf::seconds(0.5f));
	mGetPunchLeft.setRepeating(true);

	centreOrigin(mGetPunchLeft);
	centreOrigin(mGetPunchRight);
	centreOrigin(mPunchLeft);
	centreOrigin(mPunchRight);
	centreOrigin(mJump);
	centreOrigin(mRunningRight);
	centreOrigin(mRunningLeft);
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
			mPunchCountUp = sf::Time::Zero;
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

void Stickman::playerLocalSound(CommandQueue& commands, SoundEffectID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = static_cast<int>(CategoryID::SoundEffect);
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition](SoundNode& node, sf::Time)
		{
			node.playSound(effect, worldPosition);
		});

	commands.push(command);
}

void Stickman::setPunchDirection(float punchDirection)
{
	mPunchDirectionMultiplier = punchDirection;
}


void Stickman::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mIsGetPunched && mPunchDirectionMultiplier > 0)
		target.draw(mGetPunchRight, states);
	else if (mIsGetPunched && mPunchDirectionMultiplier < 0)
		target.draw(mGetPunchLeft, states);
	else if (mIsPunching && mFacingDirection > 0)
		target.draw(mPunchRight, states);
	else if (mIsPunching && mFacingDirection < 0)
		target.draw(mPunchLeft, states);
	else if (mTimeInAir > sf::Time::Zero)
		target.draw(mJump, states);
	else if (getVelocity().x > 0 && !mIsGetPunched)
		target.draw(mRunningRight, states);
	else if (getVelocity().x < 0 && !mIsGetPunched)
		target.draw(mRunningLeft, states);
	else
		target.draw(mSprite, states);
}


// CheckIsJumping solution is inspired by user AndrewEkeren
// URL of original code: https://gamedev.stackexchange.com/questions/121389/how-i-can-make-better-jump-to-my-game-c-sfml
// The logic is exactly the same, the implementation is customize in order work with derived action
// Logic: when the stickman is jumping, the mTimeInAir will start counting up
// the jump will consist of 3 part, Impulse (jumping fast), Hanging (slowly goes to the peak), then falling down
// each phase will start if timeInAir is reach the timing condition (if less than impulse time, do impulse, after that do hang, after that do fall)
void Stickman::checkIsJumping(sf::Time dt, CommandQueue& commands)
{
	if (!mIsGetPunched)
	{
		if (mIsJumping)
		{
			playerLocalSound(commands, SoundEffectID::Jump);
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

			}
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

void Stickman::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (mIsGetPunched && mPunchDirectionMultiplier > 0)
	{
		mGetPunchRight.update(dt);
	}
	else if (mIsGetPunched && mPunchDirectionMultiplier < 0)
	{
		mGetPunchLeft.update(dt);
	}
	else if (mIsPunching && mFacingDirection > 0)
	{
		mPunchRight.update(dt);
		if (mPunchRight.isFinished())
			setPunchingStatus(false);
	}
	else if (mIsPunching && mFacingDirection < 0)
	{
		mPunchLeft.update(dt);
		if (mPunchLeft.isFinished())
			setPunchingStatus(false);
	}

	else if (getVelocity().x > 0 && !mIsGetPunched)
	{
		mRunningRight.update(dt);
	}
	else if (getVelocity().x < 0 && !mIsGetPunched)
	{
		mRunningLeft.update(dt);
	}

	else if (mTimeInAir > sf::Time::Zero)
	{

		mJump.update(dt);
	}


	checkIsJumping(dt, commands);
	checkIsPunched(dt);
	punchReset(dt);
	Entity::updateCurrent(dt,commands);
}
