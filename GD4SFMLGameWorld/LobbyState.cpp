#include "LobbyState.hpp"

LobbyState::LobbyState(StateStack& stack, Context context) 
	: State(stack, context)
	, mGUIContainer()
{
}

void LobbyState::draw()
{
}

bool LobbyState::update(sf::Time dt)
{
	return false;
}

bool LobbyState::handleEvent(const sf::Event& event)
{
	return false;
}
