#include "Player2WinState.hpp"
#include "ResourceHolder.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "Utility.hpp"


Player2WinState::Player2WinState(StateStack& stack, Context context) : State(stack, context), mText(), mShowText(true), mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::Player2Win)); //Sets the background image
}

void Player2WinState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
}

bool Player2WinState::update(sf::Time dt)
{
	return false;
}

bool Player2WinState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}
	return true;
}
