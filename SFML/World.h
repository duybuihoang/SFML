#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<array>
#include <queue>

#include"ResourceHolder.h"
#include"ResourceIdentifiers.h"
#include"AirCraft.h"
#include"SceneNode.h"
#include"SpriteNode.h"
#include"CommandQueue.h"
#include"Player.h"
#include"PostEffect.h"
#include"BloomEffect.h"
#include"HealthBar.h"
#include"SoundPlayer.h"

namespace sf
{
	class RenderWindow;
}
class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, Player& player,SoundPlayer& sound);
	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue();
	bool hasAlivePlayer()const;
	bool hasPlayerReachedEnd()const;
	void resetData();
	

private:
	void loadTextures();
	void buildScene();
	void adaptPlayerVelocity();
	void adaptPlayerPosition();
	void handleCollisions();
	void spawnEnemies();
	void updateSounds();
	void addEnemies();
	void addEnemy(Aircraft::Type type, float relX,float relY);
	void guideMissile();
	void guideBoss();
	void destroyEntitiesOutsideView();
	sf::FloatRect getBattlefieldBounds();
	sf::FloatRect getViewBounds();



private:
	
	enum Layer
	{
		Background,
		LowerAir,
		UpperAir,
		LayerCount
	};
	struct SpawnPoint
	{
		SpawnPoint(Aircraft::Type type, float x, float y)
			:type(type)
			, x(x)
			, y(y)
		{

		}
		Aircraft::Type type;
		float x;
		float y;
	};
private:
	sf::RenderTarget& mTarget;
	sf::View mWorldView;	
	TextureHolder mTextures;
	FontHolder& mFonts;
	sf::RenderTexture mSceneTexture;
	Player& mplayer;
	SoundPlayer& mSounds;

	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	CommandQueue mCommandQueue;

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed;
	Aircraft* mPlayerAircraft;
	Player::Level mLevel;

	std::vector<SpawnPoint> mEnemySpawnPoints;
	std::vector<Aircraft*> mActiveEnemy;

	BloomEffect mBloomEffect;

	HealthBar* mHealthBarDisplay;
	SpriteNode* mMissileGUIDisplay;
	TextNode* mMissileTextDisplay;

};
#endif // !WORLD_HPP
