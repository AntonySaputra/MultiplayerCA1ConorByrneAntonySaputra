//Antony Saputra D00197615
//Conor Byrne D00185566

#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Stickman.hpp"
#include "Island.hpp"
#include "IslandID.hpp"
#include "LayerID.hpp"
#include "CommandQueue.hpp"
#include "BloomEffect.hpp"
#include "SoundPlayer.hpp"

#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/Graphics/Texture.hpp"

#include <array>


//Forward declaration
//namespace sf
//{
//	class RenderWindow;
//}

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderTarget& outputTarget , SoundPlayer& sounds);
	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue();
	unsigned int outOfBounds();
	void updateSounds();

private:
	void loadTextures();
	void buildScene();
	void adaptPlayerVelocity();
	void handleCollisions();

	struct SpawnPoint
	{
		SpawnPoint(IslandID type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
		{
		}

		IslandID type;
		float x;
		float y;
	};

private:
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mCamera;
	TextureHolder mTextures;
	SoundPlayer& mSounds;

	SceneNode mSceneGraph;
	std::array<SceneNode*, static_cast<int>(LayerID::LayerCount)> mSceneLayers;
	CommandQueue mCommandQueue;

	//void addIsland(IslandID type, float relX, float relY);

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mGravityVelocity;
	float mScrollSpeed;
	Stickman* mPlayerStickman;
	Stickman* mPlayerStickman2;
	std::vector<SpawnPoint>	mIslandSpawnPoints;

	BloomEffect	mBloomEffect;
};