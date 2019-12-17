#include "Application.hpp"
#include "Utility.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SettingState.hpp"
#include "Player1WinState.hpp"
#include "Player2WinState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(1200, 720),"Stack States", sf::Style::Close),
	mTextures(),
	mFonts(),
	mSoundPlayer(),
	mMusic(),
	mPlayer(PlayerID::Player1),
	mPlayer2(PlayerID::Player2),
	mStateStack(State::Context(mWindow,mTextures,mFonts,mPlayer,mPlayer2,mSoundPlayer, mMusic)),
	mStatisticText(),
	mStatisticsUpdateTime(),
	mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(FontID::Main, "Media/Sansation.ttf");
	mTextures.load(TextureID::TitleScreen, "Media/Textures/Title.png"); 
	mTextures.load(TextureID::ButtonNormal, "Media/Textures/Mini-Island1.png");
	mTextures.load(TextureID::ButtonSelected, "Media/Textures/Mini-Island1-Selected.png");
	mTextures.load(TextureID::ButtonPressed, "Media/Textures/ButtonPressed.png");
	mTextures.load(TextureID::Player1Win, "Media/Textures/Player1Win.png");
	mTextures.load(TextureID::Player2Win, "Media/Textures/Player2Win.png");
	mStatisticText.setFont(mFonts.get(FontID::Main));
	mStatisticText.setPosition(5.f, 5.f);
	mStatisticText.setCharacterSize(20);

	registerStates();
	mStateStack.pushState(StateID::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);

			//Check if the StateStack is empty
			if (mStateStack.isEmpty())
			{
				mWindow.close();
			}
		}
		updateStatistics(elapsedTime);
		draw();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event)) //while the window is putting events on the event queue do the following loop, NOTE:poll Event puts event on top of Event Queue
	{
		mStateStack.handleEvent(event); //

		if (event.type == sf::Event::Closed) //if the event is "Closed" than close the window.
		{
			mWindow.close();
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::draw()
{
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticText);
	mWindow.display();
	
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticText.setString("Frames/Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time/Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(StateID::Title);
	mStateStack.registerState<MenuState>(StateID::Menu);
	mStateStack.registerState<SettingState>(StateID::Settings);
	mStateStack.registerState<GameState>(StateID::Game);
	mStateStack.registerState<PauseState>(StateID::Pause);
	mStateStack.registerState<Player1WinState>(StateID::Player1Win);
	mStateStack.registerState<Player2WinState>(StateID::Player2Win);
}
