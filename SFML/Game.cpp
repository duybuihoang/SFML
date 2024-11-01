//#include"game.h"
//
//const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
//
//Game::Game()
//	: mwindow(sf::VideoMode(640, 480), "SFML", sf::Style::Default)
//	, mWorld(mwindow)
//{
//
//}
//void Game::run()
//{
//	sf::Clock clock;
//	sf::Time timeSinceLastUpdate = sf::Time::Zero;
//	while (mwindow.isOpen())
//	{
//		sf::Time elapsedTime = clock.restart();
//		timeSinceLastUpdate += elapsedTime;
//		while (timeSinceLastUpdate > TimePerFrame)
//		{
//			timeSinceLastUpdate -= TimePerFrame;
//			processEvents();
//			update(TimePerFrame);
//		}
//		render();
//	}
//}
//void Game::processEvents()
//{
//	CommandQueue& commands = mWorld.getCommandQueue();
//	sf::Event event;
//	while (mwindow.pollEvent(event))
//	{
//		mPlayer.handleEvent(event, commands);
//
//		if (event.type == sf::Event::Closed)
//			mwindow.close();
//	}
//	mPlayer.handleRealtimeInput(commands);
//
//}
//void Game::update(sf::Time deltaTime)
//{
//	mWorld.update(deltaTime);
//}
//void Game::render()
//{
//	mwindow.clear();
//	mWorld.draw();
//	mwindow.setView(mwindow.getDefaultView());
//	mwindow.display();
//}
