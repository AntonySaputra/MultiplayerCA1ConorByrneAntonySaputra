#include "Stickman.hpp"
#include "ResourceHolder.hpp"
#include "CategoryID.hpp"

#include <iostream>	

TextureID toTextureID(AircraftID type)
{
	switch (type)
	{
	case AircraftID::Eagle:
		return TextureID::Eagle;

	case AircraftID::Raptor:
		return TextureID::Raptor;

	case AircraftID::Stick:
		return TextureID::Stick;
	}
	return TextureID::Eagle;
}

Stickman::Stickman(AircraftID type, const TextureHolder& textures)
	: Entity()
	, mType(type)
	, mSprite(textures.get(toTextureID(type)))
	, mTimeInAir(sf::Time::Zero)
	, mJumpImpulseTime(sf::seconds(0.3f))
	, mJumpImpulseVel(-1300.f)
	, mJumpHangVel(-300.0f)
	, mMaxAirTime(sf::seconds(0.5f))
	, mMaxVelocity(-10.0f)
	, mIsJumping(false)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

unsigned int Stickman::getCategory() const
{
	switch (mType)
	{
	case AircraftID::Eagle:
		return static_cast<int>(CategoryID::PlayerAircraft);
	case AircraftID::Stick:
		return static_cast<int>(CategoryID::PlayerAircraft);

	default:
		return static_cast<int>(CategoryID::EnemyAircraft);
	}
}

void Stickman::jump()
{
	mIsJumping = true;

}

void Stickman::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
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
			mTimeInAir = sf::Time::Zero;
		}
	}


}

void Stickman::updateCurrent(sf::Time dt)
{
	checkIsJumping(dt);
	Entity::updateCurrent(dt);
}
