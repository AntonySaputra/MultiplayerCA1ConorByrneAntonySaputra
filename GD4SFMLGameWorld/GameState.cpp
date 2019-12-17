//Antony Saputra D00197615
//Conor Byrne D00185566

#include "GameState.hpp"

GameState::GameState(StateStack& stack, Context context) : State(stack,context),mWorld(*context.window, *context.sound), mPlayer(*context.player), mPlayer2(*context.player2)
{
	context.music->play(MusicID::BattleTheme);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	
	mWorld.update(dt);

	int check = mWorld.outOfBounds();
	if (check == 2) {
		
		requestStackPop();
		requestStackPush(StateID::Player1Win);
	}
	else if (check == 4) {
		requestStackPop();
		requestStackPush(StateID::Player2Win);
	}
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);
	mPlayer2.handleRealtimeInput(commands);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);
	mPlayer2.handleEvent(event, commands);
	//Pause if esc is pressed
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			requestStackPush(StateID::Pause);
		}
	}
	return false;
}
