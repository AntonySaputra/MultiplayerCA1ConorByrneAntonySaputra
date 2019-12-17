#pragma once
#include "Command.hpp"
#include "ActionID.hpp"
#include "PlayerID.hpp"
#include "FightStatus.hpp"

#include <SFML/Window/Event.hpp>
#include <map>

class CommandQueue;

class Player
{
public:
	Player(PlayerID mType);
	bool isOutOfBounds = false;

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(ActionID action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(ActionID action) const;
	void setFightStatus(FightStatus status);
	
private:
	void initializeActions();
	static bool isRealtimeAction(ActionID action);


	FightStatus getFightStatus() const;

private:
	std::map<sf::Keyboard::Key, ActionID> mKeyBinding;
	std::map<ActionID, Command> mActionBindings;
	FightStatus mCurrentFightStatus;

	PlayerID mType;
};