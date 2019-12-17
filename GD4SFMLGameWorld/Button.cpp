#include "Button.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"


GUI::Button::Button(State::Context context)
	: mCallback()
	, mNormalTexture(context.textures->get(TextureID::ButtonNormal))
	, mSelectedTexture(context.textures->get(TextureID::ButtonSelected))
	, mPressedTexture(context.textures->get(TextureID::ButtonPressed))
	, mSprite()
	, mText("", context.fonts->get(FontID::Main), 16)
	, mIsToggle(false)
	, mSound(*context.sound)
{
	mSprite.setTexture(mNormalTexture);

	sf::FloatRect bounds = mSprite.getLocalBounds();
	mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void GUI::Button::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void GUI::Button::setText(const std::string& text)
{
	mText.setString(text);
	centreOrigin(mText);
}

void GUI::Button::setToggle(bool flag)
{
	mIsToggle = flag;
}

bool GUI::Button::isSelectable() const
{
	return true;
}

void GUI::Button::select()
{
	Component::select();
	mSprite.setTexture(mSelectedTexture);
}

void GUI::Button::deselect()
{
	Component::deselect();
	mSprite.setTexture(mNormalTexture);
}

void GUI::Button::activate()
{
	Component::activate();
	mSound.play(SoundEffectID::Button);
	//if we toggle then we should show the button as pressed/toggled
	if (mIsToggle)
	{
		mSprite.setTexture(mPressedTexture);
	}

	if (mCallback)
	{
		mCallback();
	}

	if (!mIsToggle)
	{
		deactivate();
	}
}

void GUI::Button::deactivate()
{
	Component::deactivate();
	if (mIsToggle)
	{
		//reset the texture
		if (isSelected())
		{
			mSprite.setTexture(mSelectedTexture);
		}
		else
		{
			mSprite.setTexture(mNormalTexture);
		}

	}
}

void GUI::Button::handleEvent(const sf::Event& event)
{
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}
