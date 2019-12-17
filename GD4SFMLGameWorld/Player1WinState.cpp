#include "Player1WinState.hpp"
#include "ResourceHolder.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "Utility.hpp"


Player1WinState::Player1WinState(StateStack& stack, Context context) : State(stack, context), mText(), mShowText(true), mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::Player1Win)); //Sets the background image
	context.music->stop();
}

void Player1WinState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
}

bool Player1WinState::update(sf::Time dt)
{
	return false;
}

bool Player1WinState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}
	return true;
}
