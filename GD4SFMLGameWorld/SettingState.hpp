//Antony Saputra D00197615
//Conor Byrne D00185566

#pragma once
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "Label.hpp"
#include "ActionID.hpp"

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <array>

class SettingState : public State
{
public:
	SettingState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();
	void addButtonLabel(ActionID action, float y, const std::string& text, Context context);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingButtons;
	std::array<GUI::Label::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingLabels;

};