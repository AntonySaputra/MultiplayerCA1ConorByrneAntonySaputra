#include "Player.hpp"
#include "Stickman.hpp"
#include "CommandQueue.hpp"
#include "DataTable.hpp"



namespace
{
	const std::vector<PlayerData> Table = initializePlayerData();
}

struct StickmanMover
{
	StickmanMover(float vx, float vy) : velocity(vx, vy) {}

	void operator()(Stickman& stickman, sf::Time) const
	{
		if(!stickman.isGetPunch())
			stickman.accelerate(velocity);
	}
	sf::Vector2f velocity;
};



Player::Player(PlayerID type) : mType(type)
{
	//Set initial key bindings
	/*mKeyBinding[sf::Keyboard::A] = ActionID::MoveLeft;
	mKeyBinding[sf::Keyboard::D] = ActionID::MoveRight;
	mKeyBinding[sf::Keyboard::W] = ActionID::Jump;
	mKeyBinding[sf::Keyboard::S] = ActionID::MoveDown;*/

	mKeyBinding = Table[static_cast<int>(mType)].mKeyBinding;

	//Set initial action bindings
	initializeActions();

	//Assign all categories to the player's aircraft
	for (auto& pair : mActionBindings)
	{
		pair.second.category = static_cast<int>(Table[static_cast<int>(mType)].categoryID);
	}
}

void Player::initializeActions()
{
	const float playerSpeed = 400.f;
	mActionBindings[ActionID::MoveLeft].action = derivedAction<Stickman>(StickmanMover(-playerSpeed, 0.f));
	mActionBindings[ActionID::MoveRight].action = derivedAction<Stickman>(StickmanMover(playerSpeed, 0.f));
	mActionBindings[ActionID::Jump].action = derivedAction<Stickman>([](Stickman& a, sf::Time) {a.jump(); });
	mActionBindings[ActionID::MoveDown].action = derivedAction<Stickman>(StickmanMover(0.f, playerSpeed));
	mActionBindings[ActionID::Punch].action = derivedAction<Stickman>([](Stickman& a, sf::Time) {a.punch(); });
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		//Check if key pressed is in the key bindings, trigger event if it is
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
		{
			commands.push(mActionBindings[found->second]);
		}
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	//Check all assigned keys and see if they are pressed
	for (auto pair : mKeyBinding)
	{
		//if the key is pressed, look up the action and trigger the corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(mActionBindings[pair.second]);
		}
	}
}

void Player::assignKey(ActionID action, sf::Keyboard::Key key)
{
	//Remove all keys that already map to an action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
	{
		if (itr->second == action)
		{
			mKeyBinding.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(ActionID action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
		{
			return pair.first;
		}
	}
	return sf::Keyboard::Unknown;
}

bool Player::isRealtimeAction(ActionID action)
{
	switch (action)
	{
	case ActionID::MoveLeft:
	case ActionID::MoveRight:
		//case ActionID::MoveUp:
	case ActionID::MoveDown:
		return true;

	case ActionID::Jump:
		return false;

	default:
		return false;
	}
}
