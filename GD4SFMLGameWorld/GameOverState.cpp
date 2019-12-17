#include "GameOverState.hpp"
#include "ResourceHolder.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "Utility.hpp"


GameOverState::GameOverState(StateStack& stack, Context context) : State(stack, context), mText(), mShowText(true), mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::Player1Win)); //Sets the background image
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
}

bool GameOverState::update(sf::Time dt)
{
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}
	return true;
}
