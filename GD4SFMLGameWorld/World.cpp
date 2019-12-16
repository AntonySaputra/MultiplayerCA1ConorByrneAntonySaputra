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
	adaptPlayerPosition();
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
	mPlayerStickman->setScale(0.4f, 0.4f);
	//mPlayerAircraft->setVelocity(100.f, mScrollSpeed);
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(player1));

	// Add player2  Stickman
	std::unique_ptr<Stickman> player2(new Stickman(StickmanID::RedStickman, mTextures));
	mPlayerStickman2 = player2.get();
	mPlayerStickman2->setPosition(mSpawnPosition + sf::Vector2f(100, 0));
	mPlayerStickman2->setScale(0.4f, 0.4f);
	//mPlayerAircraft->setVelocity(100.f, mScrollSpeed);
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(player2));


	//Add Islands
	std::unique_ptr<Island> mMainIsland(new Island(IslandID::mainIsland, mTextures));
	mMainIsland->setPosition(mSpawnPosition + sf::Vector2f(-30, 250));
	mMainIsland->setScale(0.8f, 1.0f);
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(mMainIsland));
	
}


void World::adaptPlayerPosition()
{
	//Keep the player's position inside the screen bounds
	sf::FloatRect viewBounds(mCamera.getCenter() - mCamera.getSize() / 2.f, mCamera.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerStickman->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerStickman->setPosition(position);

	sf::Vector2f position2 = mPlayerStickman2->getPosition();
	position2.x = std::max(position2.x, viewBounds.left + borderDistance);
	position2.x = std::min(position2.x, viewBounds.left + viewBounds.width - borderDistance);
	position2.y = std::max(position2.y, viewBounds.top + borderDistance);
	position2.y = std::min(position2.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerStickman2->setPosition(position2);
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
				std::cout << "player1 punching" << std::endl;
				player2.getPunch();
				player1.setPunchingStatus(false);
			}
			else if (!player1.isPunching() && player2.isPunching())
			{
				std::cout << "player2 punching" << std::endl;
				player1.getPunch();
				player2.setPunchingStatus(false);
			}
			else if(player1.isPunching() && player2.isPunching())
			{
				std::cout << "both punching" << std::endl;
				player1.getPunch();
				player2.getPunch();
				player1.setPunchingStatus(false);
				player2.setPunchingStatus(false);
			}
		}
	}
}


