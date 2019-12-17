#include "World.hpp"

#include <iostream>

World::World(sf::RenderWindow& window) 
	: mWindow(window)
	, mCamera(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mCamera.getSize().x, mCamera.getSize().y)
	, mSpawnPosition(mCamera.getSize().x/2.f, mWorldBounds.height - mCamera.getSize().y/2.f)
	, mGravityVelocity(80.0f)
	, mPlayerStickman(nullptr)
	, mPlayerStickman2(nullptr)
{
	loadTextures();
	buildScene();

	//Prepare the Camera
	mCamera.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	//Scroll the world
	//mCamera.move(0.f, mScrollSpeed * dt.asSeconds());

	mPlayerStickman->setVelocity(0.f, 0.f);
	mPlayerStickman2->setVelocity(0.f, 0.f);

	//Forward commands to the scene graph, adapt velocity
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	adaptPlayerVelocity();

	// Collision detection and response
	handleCollisions();

	//Regular update, adapt position if outisde view	
	mSceneGraph.update(dt);
	outOfBounds();
}

void World::draw()
{
	mWindow.setView(mCamera);
	mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(TextureID::mainIsland, "Media/Textures/Island.png");
	mTextures.load(TextureID::smallIsland1, "Media/Textures/Mini-Island1.png");
	mTextures.load(TextureID::smallIsland2, "Media/Textures/Mini-Island2.png");
	mTextures.load(TextureID::BlueStick, "Media/Textures/BlueStick.png");
	mTextures.load(TextureID::RedStick, "Media/Textures/RedStick.png");
	mTextures.load(TextureID::Level, "Media/Textures/LevelMap.png");
	mTextures.load(TextureID::RunningRight, "Media/Textures/RunningRight.png");
}

void World::buildScene()
{
	//Initialize the different
	for (std::size_t i = 0; i < static_cast<int>(LayerID::LayerCount); ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	//Prepare the tiled background
	sf::Texture& texture = mTextures.get(TextureID::Level);
	sf::IntRect textureRect(mWorldBounds.top,mWorldBounds.left,texture.getSize().x,texture.getSize().y);
	texture.setRepeated(true);
	
	sf::Vector2u mTextureSize = texture.getSize();

	float ScaleX = mWindow.getSize().x / mTextureSize.x;
	float ScaleY = mWindow.getSize().y / mTextureSize.y;

	//Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	//backgroundSprite->setScale(.9f, .9f);
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	
	mSceneLayers[static_cast<int>(LayerID::Background)]->attachChild(std::move(backgroundSprite));



	//Add players Stickman
	std::unique_ptr<Stickman> player1(new Stickman(StickmanID::BlueStickman, mTextures));
	mPlayerStickman = player1.get();
	mPlayerStickman->setPosition(mSpawnPosition);
	
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(player1));

	// Add player2  Stickman
	std::unique_ptr<Stickman> player2(new Stickman(StickmanID::RedStickman, mTextures));
	mPlayerStickman2 = player2.get();
	mPlayerStickman2->setPosition(mSpawnPosition + sf::Vector2f(100, 0));
	mPlayerStickman2->setScale(0.2f, 0.2f);
	//mPlayerAircraft->setVelocity(100.f, mScrollSpeed);
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(player2));


	//Add Islands
	std::unique_ptr<Island> mMainIsland(new Island(IslandID::mainIsland, mTextures));
	mMainIsland->setPosition(mSpawnPosition + sf::Vector2f(-30, 250));
	mMainIsland->setScale(0.8f, 1.0f);
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(mMainIsland));
	
}


unsigned int World::outOfBounds()
{
	//Keep the player's position inside the screen bounds
	sf::FloatRect viewBounds(mCamera.getCenter() - mCamera.getSize() / 2.f, mCamera.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerStickman->getPosition();
	sf::Vector2f position1 = mPlayerStickman->getPosition();
	sf::Vector2f position2 = mPlayerStickman2->getPosition();
	sf::Vector2f position3 = mPlayerStickman2->getPosition();


	if (position1.x > std::min(position.x, viewBounds.left + viewBounds.width - borderDistance) ||
		position1.x < std::max(position.x, viewBounds.left + borderDistance) ||
		position1.y > std::min(position.y, viewBounds.top + viewBounds.height - borderDistance) ||
		position1.y < std::max(position.y, viewBounds.top + borderDistance)) {
		std::cout << static_cast<int>(CategoryID::PlayerStickman2) << std::endl;
		return static_cast<int>(CategoryID::PlayerStickman2); //returns winning stickman
	}

	if (position3.x > std::min(position2.x, viewBounds.left + viewBounds.width - borderDistance) || 
		position3.x < std::max(position2.x, viewBounds.left + borderDistance) || 
		position3.y > std::min(position2.y, viewBounds.top + viewBounds.height - borderDistance) || 
		position3.y < std::max(position2.y, viewBounds.top + borderDistance))
	{
		std::cout << static_cast<int>(CategoryID::PlayerStickman1) << std::endl;
		return static_cast<int>(CategoryID::PlayerStickman1);
	}
}

void World::adaptPlayerVelocity()
{
	//Don't give the player an advantage of they move diagonally
	sf::Vector2f velocity = mPlayerStickman->getVelocity();

	//If moving diagonally, reduce the velocity by root 2
	if (velocity.x != 0 && velocity.y != 0)
	{
		mPlayerStickman->setVelocity(velocity / std::sqrt(2.f));
	}

	sf::Vector2f velocity2 = mPlayerStickman2->getVelocity();

	//If moving diagonally, reduce the velocity by root 2
	if (velocity2.x != 0 && velocity2.y != 0)
	{
		mPlayerStickman2->setVelocity(velocity2 / std::sqrt(2.f));
	}
	//add the scrolling velocity
	//mPlayerAircraft->accelerate(0.f, mGravityVelocity);
}

bool matchesCategories(SceneNode::Pair& colliders, CategoryID type1, CategoryID type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (((static_cast<int>(type1))& category1) && ((static_cast<int>(type2))& category2))
	{
		return true;
	}
	else if (((static_cast<int>(type1))& category2) && ((static_cast<int>(type2))& category1))
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair, CategoryID::PlayerStickman1, CategoryID::PlayerStickman2))
		{
			auto& player1 = static_cast<Stickman&>(*pair.first);
			auto& player2 = static_cast<Stickman&>(*pair.second);

			if (player1.isPunching() && !player2.isPunching())
			{
				if (player1.getPosition().x < player2.getPosition().x )
				{
					std::cout << "player1 punching" << std::endl;
					player2.setPunchDirection(+1.0f);
					player2.getPunch();
					player1.setPunchingStatus(false);
				}
				else if (player1.getPosition().x > player2.getPosition().x )
				{
					std::cout << "player1 punching" << std::endl;
					player2.setPunchDirection(-1.0f);
					player2.getPunch();
					player1.setPunchingStatus(false);
				}
			}
			else if (!player1.isPunching() && player2.isPunching())
			{
				if (player2.getPosition().x < player1.getPosition().x)
				{
					std::cout << "player2 punching" << std::endl;
					player1.setPunchDirection(+1.0f);
					player1.getPunch();
					player2.setPunchingStatus(false);
				}
				else if (player2.getPosition().x > player1.getPosition().x)
				{
					std::cout << "player2 punching" << std::endl;
					player1.setPunchDirection(-1.0f);
					player1.getPunch();
					player2.setPunchingStatus(false);
				}
			}
			else if (player1.isPunching() && player2.isPunching())
			{
				if (player1.getPosition().x < player2.getPosition().x)
				{
					std::cout << "both punching" << std::endl;
					player1.setPunchDirection(-1.0f);
					player2.setPunchDirection(+1.0f);
					player1.getPunch();
					player2.getPunch();
					player1.setPunchingStatus(false);
					player2.setPunchingStatus(false);
				}
			}
		}

		else if (matchesCategories(pair, CategoryID::mainIsland, CategoryID::PlayerStickman1))
		{
			auto& player = static_cast<Stickman&>(*pair.second);
			player.accelerate(0.f, -500.0f);
			player.setTimeInAir(sf::Time::Zero);
			//std::cout << "Player 1 colliding with island" << std::endl;
		}

		else if (matchesCategories(pair, CategoryID::mainIsland, CategoryID::PlayerStickman2))
		{
			auto& player = static_cast<Stickman&>(*pair.second);
			player.accelerate(0.f, -500.0f);
			player.setTimeInAir(sf::Time::Zero);

			//std::cout << "Player 2 colliding with island" << std::endl;

		}
	}
}


