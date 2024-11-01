#include"Application.h"
const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);
Application::Application()
	:mWindow(sf::VideoMode(1024,768),"game", sf::Style::Default)
	,mTextures()
	,mFonts()
	,mPlayer()
	,mMusic()		
	,mSound()
	,mStateStack(State::Context(mWindow,mTextures,mFonts,mPlayer,mMusic,mSound))
	,mStatisticsText()
	,mStatisticsUpdateTime()
	,mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	mFonts.load(Fonts::Main, "data/Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "data/TitleScreen.png");
	mTextures.load(Textures::Buttons, "data/Buttons.png");
	
	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);

	mMusic.setVolume(25.f);
}
void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}
void::Application::render()
{
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);
	mWindow.display();
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
			if (mStateStack.isEmpty())
				mWindow.close();

		}
		updateStatistics(elapsedTime);
		render();
	}
}
void::Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}
void::Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + tostring(mStatisticsNumFrames));
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}
void::Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<LoadingState>(States::Loading);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
	mStateStack.registerState<LevelState>(States::Level);
}