#include "MenuState.hpp"

#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "OptionID.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

MenuState::MenuState(StateStack& stack, Context context) : State(stack,context) , mOptions(), mOptionIndex(0)
{
	sf::Texture& texture = context.textures->get(TextureID::TitleScreen);
	sf::Font& font = context.fonts->get(FontID::Main);

	mBackgroundSprite.setTexture(texture);

	float offset = 20;

	//a simple menu
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString("Play");
	centreOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.0f);
	mOptions.push_back(playOption);

	sf::Text settingOption;
	settingOption.setFont(font);
	settingOption.setString("Settings");
	centreOrigin(settingOption);
	sf::FloatRect bounds = playOption.getLocalBounds();
	settingOption.setPosition(context.window->getView().getSize() / 2.0f + sf::Vector2f(0.f, bounds.height + offset));
	mOptions.push_back(settingOption);

	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
	centreOrigin(exitOption);
	bounds = settingOption.getLocalBounds();
	exitOption.setPosition(context.window->getView().getSize() / 2.0f + sf::Vector2f(0.f, bounds.height + offset + offset * 2.5f));
	mOptions.push_back(exitOption);

	
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	for (const sf::Text& text : mOptions)
	{
		window.draw(text);
	}
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == static_cast<int>(OptionID::Play))
		{
			requestStackPop();
			requestStackPush(StateID::Game);
		}
		else if (mOptionIndex == static_cast<int>(OptionID::Setting))
		{
			//For now settings does nothing, later on this will open up another menu
		}
		else if(mOptionIndex == static_cast<int>(OptionID::Exit))
		{
			requestStackPop();
		}
	}

	else if (event.key.code == sf::Keyboard::Up)
	{
		if (mOptionIndex > 0)
		{
			mOptionIndex--;
		}
		else
		{
			mOptionIndex = mOptions.size() - 1;
		}
		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (mOptionIndex < mOptions.size() - 1)
		{
			mOptionIndex++;
		}
		else
		{
			mOptionIndex = 0;
		}
		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
	{
		return;
	}

	//white all the texts
	for (sf::Text& text : mOptions)
	{
		text.setFillColor(sf::Color::White);
	}

	//make the selected text red
	mOptions[mOptionIndex].setFillColor(sf::Color::Red);
}
