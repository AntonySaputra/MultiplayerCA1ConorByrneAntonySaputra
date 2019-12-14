#include "World.hpp"

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

	//Forward commands to the scene graph, adapt velocity
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	adaptPlayerVelocity();
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
	mTextures.load(TextureID::Stick, "Media/Textures/BlueStick.png");
	mTextures.load(TextureID::Raptor, "Media/Textures/Raptor.png");
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
	std::unique_ptr<Stickman> leader(new Stickman(AircraftID::Stick, mTextures));
	mPlayerStickman = leader.get();
	mPlayerStickman->setPosition(mSpawnPosition);
	mPlayerStickman->setScale(0.4f, 0.4f);
	//mPlayerAircraft->setVelocity(100.f, mScrollSpeed);
	mSceneLayers[static_cast<int>(LayerID::Air)]->attachChild(std::move(leader));

	
	
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
	//add the scrolling velocity
	//mPlayerAircraft->accelerate(0.f, mGravityVelocity);
}
