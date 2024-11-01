#include"World.h"
#include"Foreach.h"
#include<cmath>
#include<algorithm>
#include"Pickup.h"
#include"DataTables.h"
#include"ParticleNode.h"
#include"EmitterNode.h"
#include<iostream>
#include"Utility.h"
#include"SoundPlayer.h"
#include"SoundNode.h"
World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, Player& player,SoundPlayer& sound)
	:mTarget(outputTarget)
	,mplayer(player)
	, mFonts(fonts)
	,mSounds(sound)
	, mWorldView(outputTarget.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mActiveEnemy()
	, mScrollSpeed(-100.f)
	, mPlayerAircraft(nullptr)
	, mMissileGUIDisplay(nullptr)
	, mHealthBarDisplay(nullptr)
	,mMissileTextDisplay(nullptr)
	, mEnemySpawnPoints()
{
	//mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 7000.f)
	//mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	mWorldBounds.left = 0;
	mWorldBounds.top = 0;	
	mWorldBounds.width = mWorldView.getSize().x;
	if (player.GetCurrentLevel() == Player::Easy)
		mWorldBounds.height = 6000;
	else if (player.GetCurrentLevel() == Player::Normal)
		mWorldBounds.height = 7000;
	else if (player.GetCurrentLevel() == Player::Challenged)
		mWorldBounds.height = 8000;
	else if (player.GetCurrentLevel() == Player::Hard)
	{
		mWorldBounds.height = 13000;
	}
	else if (player.GetCurrentLevel() == Player::Incredible)
	{
		mWorldBounds.height = 20000;
	}
	mSpawnPosition.x = mWorldView.getSize().x / 2.f;
	mSpawnPosition.y = mWorldBounds.height - mWorldView.getSize().y / 2.f;

	


	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
	loadTextures();
	buildScene();
	mWorldView.setCenter(mSpawnPosition);
}
void::World::loadTextures()
{
	
	mTextures.clear();
	if (mplayer.GetCurrentLevel() == 1 || mplayer.GetCurrentLevel() == 4)
	{
		mTextures.load(Textures::Entities, "data/Entities_2.png");
		mTextures.load(Textures::Particle, "data/Particle.png");
		mTextures.load(Textures::Jungle, "data/Space.png");
		mTextures.load(Textures::FinishLine, "data/FinishLine.png");
		mTextures.load(Textures::Explosion, "data/Explosion.png");
		mTextures.load(Textures::Boss_2, "data/Long_boss.png");
	}
	else if(mplayer.GetCurrentLevel() == 0 || mplayer.GetCurrentLevel() == 2 || mplayer.GetCurrentLevel() == 3)
	{
		mTextures.load(Textures::Entities, "data/Entities.png");
		mTextures.load(Textures::Particle, "data/Particle.png");
		mTextures.load(Textures::Jungle, "data/Jungle.png");
		mTextures.load(Textures::FinishLine, "data/FinishLine.png");
		mTextures.load(Textures::Explosion, "data/Explosion.png");
		mTextures.load(Textures::Boss_2, "data/long_airplane.png");
	}
}
void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}
	//											generate stuff
	// Prepare the tiled background
	sf::Texture& jungletexture = mTextures.get(Textures::Jungle);
	jungletexture.setRepeated(true);

	float viewHeight = mWorldView.getSize().y;
	sf::IntRect textureRect(mWorldBounds);
	textureRect.height += static_cast<int>(viewHeight);
	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(jungletexture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add finish line to the Scene
	sf::Texture& finishtexture = mTextures.get(Textures::FinishLine);
	std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishtexture));
	finishSprite->setPosition(0.f, -76.f);
	mSceneLayers[Background]->attachChild(std::move(finishSprite));
	
	

	// Add particle node to Scene
	std::unique_ptr<ParticleNode> smokenode(new ParticleNode(Particle::Smoke, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(smokenode));

	// Add propellant particle node to the scene
	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));



	// Add player's aircraft
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[UpperAir]->attachChild(std::move(player));

	//										GUI
	// Add player's healthBar

	std::unique_ptr<HealthBar> health(new HealthBar(mPlayerAircraft->getHitpoints()));
	health->setPosition(10, mSpawnPosition.y - mWorldView.getSize().y/2 + 30 );
	mHealthBarDisplay = health.get();
	mSceneLayers[UpperAir]->attachChild(std::move(health));

	//Add player's missiles
	sf::Texture& mMissile = mTextures.get(Textures::Entities);
	std::unique_ptr<SpriteNode> mMissleDisplay(new SpriteNode(mMissile, sf::IntRect(160, 64, 15, 32)));
	mMissleDisplay->setPosition(10, mSpawnPosition.y - mWorldView.getSize().y / 2 + 60);
	mMissileGUIDisplay = mMissleDisplay.get();
	mSceneLayers[UpperAir]->attachChild(std::move(mMissleDisplay));
	
	// Add player's missle text
		std::unique_ptr<TextNode> missileText(new TextNode(mFonts, ""));
		missileText->setPosition(50, mSpawnPosition.y - mWorldView.getSize().y / 2 + 60 + 16) ;
		mMissileTextDisplay = missileText.get();
		mSceneLayers[UpperAir]->attachChild(std::move(missileText));

	//										MUSIC AND SOUND
	//add sound node
		std::unique_ptr<SoundNode> soundnode(new SoundNode(mSounds));
		mSceneGraph.attachChild(std::move(soundnode));


	//									add enemy
	addEnemies();
	
}

void::World::draw()
{
	//if (PostEffect::isSupported())
	//{
	//	mSceneTexture.clear();
	//	mSceneTexture.setView(mWorldView);
	//	mSceneTexture.draw(mSceneGraph);
	//	mSceneTexture.display();
	//	mBloomEffect.apply(mSceneTexture, mTarget);
	//}
	//else
	//{
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
	
	//}
	
	
}
void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	//std::cout << mWorldBounds.left << " " << mWorldBounds.top << " " << mWorldBounds.width << " " << mWorldBounds.height << "\n";
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);
	mPlayerAircraft->updateText(mMissileTextDisplay);

	destroyEntitiesOutsideView();
	guideBoss();
	guideMissile();
	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();
	handleCollisions();
	mSceneGraph.removeWrecks();
	spawnEnemies();
	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt,mCommandQueue);
	adaptPlayerPosition();

	mHealthBarDisplay->setBarSize(mPlayerAircraft->getHitpoints());
	mHealthBarDisplay->setPosition(mHealthBarDisplay->getPosition().x,
		mHealthBarDisplay->getPosition().y + mScrollSpeed * dt.asSeconds());

	mMissileGUIDisplay->setPosition(mMissileGUIDisplay->getPosition().x,
		mMissileGUIDisplay->getPosition().y + mScrollSpeed * dt.asSeconds());

	mMissileTextDisplay->setPosition(mMissileTextDisplay->getPosition().x,
		mMissileTextDisplay->getPosition().y + mScrollSpeed * dt.asSeconds());

	/*if (mActiveEnemy.empty())
		std::cout << 1;
	else
		std::cout << 'a';
	if (mEnemySpawnPoints.empty())
		std::cout << 2;
	else
		std::cout << 'b';
	std::cout << "\n";*/

	//if (mActiveEnemy.empty())
	//{
	//		mScrollSpeed -= 200;
	//}
	//else
	//		mScrollSpeed = -100.f;
	//std::cout << mScrollSpeed << "\n";
	if (mActiveEnemy.empty() && mEnemySpawnPoints.empty())
		mScrollSpeed = -200;
	else
		mScrollSpeed = -100;

	updateSounds();
}
CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}
void World::adaptPlayerPosition()
{
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);
}
void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
	mPlayerAircraft->accelerate(0.f, mScrollSpeed);

}
sf::FloatRect World::getViewBounds()
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}
sf::FloatRect World::getBattlefieldBounds()
{
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;//300
	return bounds;
}
void::World::spawnEnemies()
{
	while (!mEnemySpawnPoints.empty()&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top )
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();
		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
		if (spawn.type == Aircraft::Avenger)
		{
			enemy->setFireInterval(randomInt(10, 21));
		}

		//if (spawn.type >= Aircraft::Boss2)
		//{
		//	std::cout<< enemy->getPosition().x <<" " << enemy->getPosition().y;
		//}
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);
		mSceneLayers[UpperAir]->attachChild(std::move(enemy));
		mEnemySpawnPoints.pop_back();
	}

}
void::World::addEnemy(Aircraft::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
	
}

void::World::addEnemies()
{

	


	if (mplayer.GetCurrentLevel() == Player::Level::Normal)
	{
		addEnemy(Aircraft::Avenger, 0.f, 1000.f);

		addEnemy(Aircraft::Avenger, +200.f, 1150.f);
		addEnemy(Aircraft::Avenger, -200.f, 1150.f);
		addEnemy(Aircraft::Avenger, +300.f, 1150.f);
		addEnemy(Aircraft::Avenger, -300.f, 1150.f);

		addEnemy(Aircraft::Raptor, 0.f, 1300.f);
		addEnemy(Aircraft::Raptor, +100.f, 1300.f);
		addEnemy(Aircraft::Raptor, -100.f, 1300.f);

		addEnemy(Aircraft::Raptor, +400.f, 1300.f);
		addEnemy(Aircraft::Raptor, -400.f, 1300.f);

		addEnemy(Aircraft::Avenger, 70.f, 1500.f);
		addEnemy(Aircraft::Avenger, -70.f, 1500.f);
		addEnemy(Aircraft::Avenger, -70.f, 1710.f);
		addEnemy(Aircraft::Avenger, 70.f, 1700.f);
		addEnemy(Aircraft::Avenger, 30.f, 1850.f);
		addEnemy(Aircraft::Raptor, 300.f, 2200.f);
		addEnemy(Aircraft::Raptor, -300.f, 2200.f);
		addEnemy(Aircraft::Raptor, 0.f, 2200.f);
		addEnemy(Aircraft::Raptor, 0.f, 2500.f);
		addEnemy(Aircraft::Avenger, -300.f, 2700.f);
		addEnemy(Aircraft::Avenger, -300.f, 2700.f);
		addEnemy(Aircraft::Raptor, 0.f, 3000.f);
		addEnemy(Aircraft::Raptor, 250.f, 3250.f);
		addEnemy(Aircraft::Raptor, -250.f, 3250.f);
		addEnemy(Aircraft::Avenger, 0.f, 3500.f);
		addEnemy(Aircraft::Avenger, 0.f, 3700.f);
		addEnemy(Aircraft::Raptor, 0.f, 3800.f);
		addEnemy(Aircraft::Avenger, 0.f, 4000.f);
		addEnemy(Aircraft::Avenger, -200.f, 4200.f);
		addEnemy(Aircraft::Raptor, 200.f, 4200.f);

		addEnemy(Aircraft::Avenger, -70.f, 4700.f);
		addEnemy(Aircraft::Avenger, -70.f, 4800.f);
		addEnemy(Aircraft::Avenger, -70.f, 4900.f);

		addEnemy(Aircraft::Avenger, -200.f, 5300.f);
		addEnemy(Aircraft::Avenger, -100.f, 5300.f);
		addEnemy(Aircraft::Avenger, 0.f, 5300.f);
		addEnemy(Aircraft::Avenger, 100.f, 5300.f);
		addEnemy(Aircraft::Avenger, -400.f, 5300.f);
		addEnemy(Aircraft::Avenger, -300.f, 5300.f);
		addEnemy(Aircraft::Avenger, 200.f, 5300.f);
		addEnemy(Aircraft::Avenger, 300.f, 5300.f);
		addEnemy(Aircraft::Avenger, 400.f, 5300.f);


		addEnemy(Aircraft::Raptor, mPlayerAircraft->getWorldPosition().x - 512, 5700.f);
		addEnemy(Aircraft::Raptor, mPlayerAircraft->getWorldPosition().x - 512, 5800.f);
		addEnemy(Aircraft::Raptor, mPlayerAircraft->getWorldPosition().x - 512, 5900.f);

		//addEnemy(Aircraft::Boss2, 0.f, 1100.f);
	}
	else if (mplayer.GetCurrentLevel() == Player::Level::Easy)
	{
		addEnemy(Aircraft::Raptor, 0.f, 700.f);
		addEnemy(Aircraft::Raptor, 0.f, 750.f);
		addEnemy(Aircraft::Raptor, 0.f, 800.f);
		addEnemy(Aircraft::Raptor, 0.f, 850.f);
		addEnemy(Aircraft::Raptor, 0.f, 900.f);

		addEnemy(Aircraft::Avenger, 0.f, 1200.f);
		addEnemy(Aircraft::Avenger, 100.f, 1100.f);
		addEnemy(Aircraft::Avenger, -100.f, 1100.f);

		addEnemy(Aircraft::Raptor, 0.f, 1700.f);
		addEnemy(Aircraft::Avenger, -100.f, 1790.f);

		addEnemy(Aircraft::Raptor, 0.f, 2750.f);
		addEnemy(Aircraft::Raptor, 0.f, 2800.f);
		addEnemy(Aircraft::Raptor, 0.f, 2850.f);
		addEnemy(Aircraft::Raptor, 0.f, 2900.f);

		addEnemy(Aircraft::Avenger, 0.f, 3700.f);
		addEnemy(Aircraft::Avenger, -300.f, 3700.f);
		addEnemy(Aircraft::Avenger, 300.f, 3700.f);
		addEnemy(Aircraft::Avenger, -400.f, 3500.f);
		addEnemy(Aircraft::Avenger, 400.f, 3500.f);
		addEnemy(Aircraft::Raptor, -100.f, 3500.f);
		addEnemy(Aircraft::Raptor,100.f, 3500.f);
		addEnemy(Aircraft::Raptor, -100.f, 3550.f);
		addEnemy(Aircraft::Raptor, 100.f, 3550.f);
		addEnemy(Aircraft::Raptor, -100.f, 3600.f);
		addEnemy(Aircraft::Raptor, 100.f, 3600.f);
		addEnemy(Aircraft::Raptor, -100.f, 3650.f);
		addEnemy(Aircraft::Raptor, 100.f, 3650.f);
		addEnemy(Aircraft::Avenger, 0.f, 3700.f);
		addEnemy(Aircraft::Raptor, -100.f, 3700.f);
		addEnemy(Aircraft::Raptor, 100.f, 3700.f);
		addEnemy(Aircraft::Raptor, -100.f, 3750.f);
		addEnemy(Aircraft::Raptor, 100.f, 3750.f);

	}
	else if (mplayer.GetCurrentLevel() == Player::Challenged)
	{
		addEnemy(Aircraft::Raptor, 0.f, 900.f);
		addEnemy(Aircraft::Raptor, -100.f, 900.f);
		addEnemy(Aircraft::Raptor, 100.f, 900.f);
		addEnemy(Aircraft::Raptor, -200.f, 900.f);
		addEnemy(Aircraft::Raptor, 200.f, 900.f);
		addEnemy(Aircraft::Raptor, 300.f, 1000.f);
		addEnemy(Aircraft::Raptor, -300.f, 1000.f);

		addEnemy(Aircraft::Raptor, 0, 1200.f);
		addEnemy(Aircraft::Raptor, 0, 1300.f);
		addEnemy(Aircraft::Raptor, 0, 1400.f);
		addEnemy(Aircraft::Raptor, 0, 1500.f);
		addEnemy(Aircraft::Raptor, 0, 1600.f);

		addEnemy(Aircraft::Avenger, 0, 1800);
		addEnemy(Aircraft::Avenger, 100, 1900);
		//addEnemy(Aircraft::Avenger, mPlayerAircraft->getWorldPosition().x - 512, 1800);
		addEnemy(Aircraft::Boss3, 0.f, 2200);


		addEnemy(Aircraft::Raptor, 0, 2500.f);
		addEnemy(Aircraft::Raptor, -100 , 2600.f);
		addEnemy(Aircraft::Raptor, +100, 2600.f);
		addEnemy(Aircraft::Raptor, -200, 2700.f);
		addEnemy(Aircraft::Raptor, +200, 2700.f);
		addEnemy(Aircraft::Raptor, -300, 2800.f);
		addEnemy(Aircraft::Raptor, +300, 2800.f);
		addEnemy(Aircraft::Raptor, -400, 2900.f);
		addEnemy(Aircraft::Raptor, +400, 2900.f);

		addEnemy(Aircraft::Avenger, 0, 3100);
		addEnemy(Aircraft::Avenger, -100, 3100);
		addEnemy(Aircraft::Avenger, 100, 3100);
		addEnemy(Aircraft::Avenger, 0, 3500);

		addEnemy(Aircraft::Raptor, 0, 3700);
		addEnemy(Aircraft::Raptor, 100, 3700);
		addEnemy(Aircraft::Raptor, 200, 3700);
		addEnemy(Aircraft::Raptor, 300, 3700);
		addEnemy(Aircraft::Raptor, 400, 3700);

		addEnemy(Aircraft::Raptor, 0, 4000);
		addEnemy(Aircraft::Raptor, -100, 4000);
		addEnemy(Aircraft::Raptor, -200, 4000);
		addEnemy(Aircraft::Raptor,-300, 4000);
		addEnemy(Aircraft::Raptor,- 400, 4000);

		addEnemy(Aircraft::Raptor, 0, 4500);
		addEnemy(Aircraft::Raptor, -100, 4600);
		addEnemy(Aircraft::Raptor, -200, 4700);
		addEnemy(Aircraft::Raptor, -300, 4800);
		addEnemy(Aircraft::Raptor, -400, 4900);

		addEnemy(Aircraft::Raptor, 0, 5000);
		addEnemy(Aircraft::Raptor, 100, 5100);
		addEnemy(Aircraft::Raptor, 200, 5200);
		addEnemy(Aircraft::Raptor, 300, 5300);
		addEnemy(Aircraft::Raptor, 400, 5400);

		addEnemy(Aircraft::Avenger, 70, 6000);
		addEnemy(Aircraft::Avenger, -70, 6000);
		addEnemy(Aircraft::Avenger, 210, 6000);
		addEnemy(Aircraft::Avenger, -210, 6000);

		addEnemy(Aircraft::Avenger, 170, 6200);
		addEnemy(Aircraft::Avenger, -170, 6200);
		addEnemy(Aircraft::Avenger, 310, 6400);
		addEnemy(Aircraft::Avenger, -490, 6400);

	}
	else if (mplayer.GetCurrentLevel() == Player::Hard)
	{
	addEnemy(Aircraft::Raptor, 0, 700);
	addEnemy(Aircraft::Raptor, -100, 700);
	addEnemy(Aircraft::Raptor, 100, 700);
	addEnemy(Aircraft::Raptor, -200, 700);
	addEnemy(Aircraft::Raptor, 200, 700);
	addEnemy(Aircraft::Raptor, 300, 700);
	addEnemy(Aircraft::Raptor, -300, 700);
	addEnemy(Aircraft::Raptor, -400, 700);
	addEnemy(Aircraft::Raptor, 400, 700);
	addEnemy(Aircraft::Raptor, -500, 700);
	addEnemy(Aircraft::Raptor, 500, 700);


	addEnemy(Aircraft::Raptor, 0, 900);
	addEnemy(Aircraft::Raptor, -100, 900);
	addEnemy(Aircraft::Raptor, 100, 900);
	addEnemy(Aircraft::Raptor, -200, 900);
	addEnemy(Aircraft::Raptor, 200, 900);
	addEnemy(Aircraft::Raptor, 300, 900);
	addEnemy(Aircraft::Raptor, -300, 900);
	addEnemy(Aircraft::Raptor, -400, 900);
	addEnemy(Aircraft::Raptor, 400, 900);
	addEnemy(Aircraft::Raptor, -500, 900);
	addEnemy(Aircraft::Raptor, 500, 900	);


		addEnemy(Aircraft::Raptor, 0, 1200.f);
		addEnemy(Aircraft::Raptor, 0, 1300.f);
		addEnemy(Aircraft::Raptor, 0, 1400.f);
		addEnemy(Aircraft::Raptor, 0, 1500.f);
		addEnemy(Aircraft::Raptor, 0, 1600.f);


		addEnemy(Aircraft::Raptor, 0, 2500.f);
		addEnemy(Aircraft::Raptor, -100, 2600.f);
		addEnemy(Aircraft::Raptor, +100, 2600.f);
		addEnemy(Aircraft::Raptor, -200, 2700.f);
		addEnemy(Aircraft::Raptor, +200, 2700.f);
		addEnemy(Aircraft::Raptor, -300, 2800.f);
		addEnemy(Aircraft::Raptor, +300, 2800.f);
		addEnemy(Aircraft::Raptor, -400, 2900.f);
		addEnemy(Aircraft::Raptor, +400, 2900.f);

		addEnemy(Aircraft::Avenger, 0, 3100);
		addEnemy(Aircraft::Avenger, -100, 3100);
		addEnemy(Aircraft::Avenger, 100, 3100);
		addEnemy(Aircraft::Avenger, 0, 3500);



		addEnemy(Aircraft::Raptor, 0, 4500);
		addEnemy(Aircraft::Raptor, -100, 4600);
		addEnemy(Aircraft::Raptor, -200, 4700);
		addEnemy(Aircraft::Raptor, -300, 4800);
		addEnemy(Aircraft::Raptor, -400, 4900);

		addEnemy(Aircraft::Raptor, 0, 5000);
		addEnemy(Aircraft::Raptor, 100, 5100);
		addEnemy(Aircraft::Raptor, 200, 5200);
		addEnemy(Aircraft::Raptor, 300, 5300);
		addEnemy(Aircraft::Raptor, 400, 5400);

		addEnemy(Aircraft::Avenger, 70, 6000);
		addEnemy(Aircraft::Avenger, -70, 6000);
		addEnemy(Aircraft::Avenger, 210, 6000);
		addEnemy(Aircraft::Avenger, -210, 6000);

		addEnemy(Aircraft::Avenger, 170, 6200);
		addEnemy(Aircraft::Avenger, -170, 6200);
		addEnemy(Aircraft::Avenger, 310, 6400);
		addEnemy(Aircraft::Avenger, -470, 6400);

		addEnemy(Aircraft::Avenger, -470, 7800);
		addEnemy(Aircraft::Avenger, 470, 7800);

		addEnemy(Aircraft::Avenger, -320, 8000);
		addEnemy(Aircraft::Avenger, 290, 8000);

		addEnemy(Aircraft::Raptor, 0, 8500);
		addEnemy(Aircraft::Raptor, -100, 8500);
		addEnemy(Aircraft::Raptor, 100, 8500);
		addEnemy(Aircraft::Raptor, -200, 8500);
		addEnemy(Aircraft::Raptor,200, 8500);
		addEnemy(Aircraft::Raptor, 300, 8500);
		addEnemy(Aircraft::Raptor, -300, 8500);
		addEnemy(Aircraft::Raptor, -400, 8500);
		addEnemy(Aircraft::Raptor, 400, 8500);
		addEnemy(Aircraft::Raptor, -500, 8500);
		addEnemy(Aircraft::Raptor, 500, 8500);


		addEnemy(Aircraft::Raptor, 0, 8700);
		addEnemy(Aircraft::Raptor, -100, 8700);
		addEnemy(Aircraft::Raptor, 100, 8700);
		addEnemy(Aircraft::Raptor, -200, 8700);
		addEnemy(Aircraft::Raptor, 200, 8700);
		addEnemy(Aircraft::Raptor, 300, 8700);
		addEnemy(Aircraft::Raptor, -300, 8700);
		addEnemy(Aircraft::Raptor, -400, 8700);
		addEnemy(Aircraft::Raptor, 400, 8700);
		addEnemy(Aircraft::Raptor, -500, 8700);
		addEnemy(Aircraft::Raptor, 500, 8700);

		addEnemy(Aircraft::Boss4, 0.f, 9200);

	}
	else if (mplayer.GetCurrentLevel() == Player::Incredible)
	{
	addEnemy(Aircraft::Raptor, 0, 700);
	addEnemy(Aircraft::Raptor, -100, 700);
	addEnemy(Aircraft::Raptor, 100, 700);
	addEnemy(Aircraft::Raptor, -200, 700);
	addEnemy(Aircraft::Raptor, 200, 700);
	addEnemy(Aircraft::Raptor, 300, 700);
	addEnemy(Aircraft::Raptor, -300, 700);
	addEnemy(Aircraft::Raptor, -400, 700);
	addEnemy(Aircraft::Raptor, 400, 700);


	addEnemy(Aircraft::Raptor, 0, 800);
	addEnemy(Aircraft::Raptor, -100, 800);
	addEnemy(Aircraft::Raptor, 100, 800);
	addEnemy(Aircraft::Raptor, -200, 800);
	addEnemy(Aircraft::Raptor, 200, 800);
	addEnemy(Aircraft::Raptor, 300, 800);
	addEnemy(Aircraft::Raptor, -300, 800);
	addEnemy(Aircraft::Raptor, -400, 800);
	addEnemy(Aircraft::Raptor, 400, 800);
	



//	addEnemy(Aircraft::Avenger, 0, 1200);
	addEnemy(Aircraft::Avenger, -100, 1400);
	addEnemy(Aircraft::Avenger, -200, 1400);
	addEnemy(Aircraft::Avenger, -300, 1400);
	addEnemy(Aircraft::Avenger, 100, 1400);
	addEnemy(Aircraft::Avenger, 200, 1400);
	addEnemy(Aircraft::Avenger, 300, 1400);

	addEnemy(Aircraft::Avenger, 0, 1700);
	addEnemy(Aircraft::Avenger, -100, 1700);
	addEnemy(Aircraft::Avenger, -200, 1700);
	//addEnemy(Aircraft::Avenger, -300, 1200);
	addEnemy(Aircraft::Avenger, 100, 1700);
	addEnemy(Aircraft::Avenger, 200, 1700);
	addEnemy(Aircraft::Avenger, 300, 1700);

	addEnemy(Aircraft::Avenger, 0, 2000);
	addEnemy(Aircraft::Avenger, -100, 2000);
	addEnemy(Aircraft::Avenger, -200, 2000);
	addEnemy(Aircraft::Avenger, -300, 2000);
	addEnemy(Aircraft::Avenger, 100, 2000);
	//addEnemy(Aircraft::Avenger, 200, 1200);
	addEnemy(Aircraft::Avenger, 300, 2000);

	addEnemy(Aircraft::Avenger, 0, 2400);
	addEnemy(Aircraft::Avenger, -100, 2400);
	addEnemy(Aircraft::Avenger, -200, 2400);
	addEnemy(Aircraft::Avenger, -300, 2400);
	addEnemy(Aircraft::Avenger, -400, 2400);
	addEnemy(Aircraft::Avenger, 100, 2400);
	addEnemy(Aircraft::Avenger, 200, 2400);
	//addEnemy(Aircraft::Avenger, 300, 1200);
	addEnemy(Aircraft::Avenger, 400, 2400);

	addEnemy(Aircraft::Raptor, 0, 3000);
	addEnemy(Aircraft::Raptor, 0, 3100);
	addEnemy(Aircraft::Raptor, 0, 3200);
	addEnemy(Aircraft::Raptor, 0, 3300);
	addEnemy(Aircraft::Raptor, 0, 3400);
	addEnemy(Aircraft::Raptor, 0, 3500);

	addEnemy(Aircraft::Raptor, -100, 4000);
	addEnemy(Aircraft::Raptor, -200, 4100);
	addEnemy(Aircraft::Raptor, -300, 4200);
	addEnemy(Aircraft::Raptor, -400, 4300);
	addEnemy(Aircraft::Raptor, -0, 3900);
	addEnemy(Aircraft::Raptor, 100, 4000);
	addEnemy(Aircraft::Raptor, 200, 4100);
	addEnemy(Aircraft::Raptor, 300, 4200);
	addEnemy(Aircraft::Raptor, 400, 4300);

	addEnemy(Aircraft::Boss5, 0, 5000);
//	addEnemy(Aircraft::Raptor, 400, 5000);



	}





	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});

}
void::World::updateSounds()
{
	mSounds.setListenerPosition(mPlayerAircraft->getWorldPosition());
	mSounds.removeStoppedSounds();
}
void::World::guideBoss()
{
	Command bossGuider;
	bossGuider.Category = Category::EnemyAircraft;
	bossGuider.action = derivedAction<Aircraft>([this](Aircraft& boss, sf::Time)
		{
			if (!boss.isGuided())
			{
				return;
			}
			else
			{
				boss.guideToward(mPlayerAircraft->getWorldPosition());
				//std::cout << 1;
			}
		});

	mCommandQueue.push(bossGuider);

}
void::World::guideMissile()
{
	Command enemyCollector;
	enemyCollector.Category = Category::EnemyAircraft;
	enemyCollector.action = derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
		{
			if (!enemy.isDestroyed())
				mActiveEnemy.push_back(&enemy);
		});
	

	Command missileGuider;
	missileGuider.Category = Category::AlliedProjectile;
	missileGuider.action = derivedAction<Projectiles>([this](Projectiles& missle, sf::Time)
		{
			if (!missle.isGuided())
			{
				return;
			}
			float minDistance = std::numeric_limits<float>::max();
			Aircraft* closestEnemy = nullptr;
			FOREACH(Aircraft * enemy, mActiveEnemy)
			{
				float enemyDistance = distance(missle, *enemy);
				if (enemyDistance < minDistance)
				{
					closestEnemy = enemy;
					minDistance = enemyDistance;
				}
			}
			if (closestEnemy)
				missle.guideTowards(closestEnemy->getWorldPosition());
		});
	// Push commands, reset active enemies
	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemy.clear();
}
bool::World::hasAlivePlayer()const
{
	return !mPlayerAircraft->isMarkedForRemoval();
}
void::World::resetData()
{
	mPlayerAircraft->defaultDirection();
}
bool::World::hasPlayerReachedEnd()const
{
	return (!mWorldBounds.contains(mPlayerAircraft->getPosition()));
}
bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}
void::World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollisions(mSceneGraph, collisionPairs);
	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}
		else if (matchesCategories(pair, Category::PlayerAircraft, Category::PickUp))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			pickup.apply(player);
			pickup.destroy();
			player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
		}
		else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile) ||
			matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
		{
			auto& aircraft = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectiles&> (*pair.second);

			if (projectile.getPosition().y < aircraft.getPosition().y)
			{
				aircraft.damage(projectile.getDamage());
				projectile.destroy();
			}
		}
		
	}
}
void::World::destroyEntitiesOutsideView()
{
	Command command;
	command.Category = Category::Projectile | Category::EnemyAircraft;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
		{

			if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			{
				e.destroy();
			}
		});
	mCommandQueue.push(command);
}
