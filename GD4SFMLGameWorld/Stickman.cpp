#include "Stickman.hpp"
#include "ResourceHolder.hpp"
#include "CategoryID.hpp"
#include "DataTable.hpp"

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
	: Entity()
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(mType)].texture))
	, mTimeInAir(sf::Time::Zero)
	, mJumpImpulseTime(sf::seconds(0.3f))
	, mJumpImpulseVel(-1300.f)
	, mJumpHangVel(-300.0f)
	, mMaxAirTime(Table[static_cast<int>(mType)].maxAirTime)
	, mMaxVelocity(-10.0f)
	, mIsJumping(false)
	, mIsMarkedForRemoval(false)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
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
