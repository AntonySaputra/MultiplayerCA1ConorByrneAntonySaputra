//Antony Saputra D00197615
//Conor Byrne D00185566

#pragma once

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Container.hpp"
#include "Button.hpp"

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);



private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
};