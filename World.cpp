#include "World.h"
#include "Constants.h"

#include <iostream>		// for debug
#include <memory>


World::World() :
	mOceanGridTexture(),
	mOceanGridSprite(),
	mRadarGridTexture(),
	mRadarGridSprite(),
	mTextInfo(std::make_unique<TextInfo>()),
	mGameState(GameState::Preparing),
	mFleetPrepare(std::make_unique<Fleet>()),
	mTurn(""),
	mWinner(""),
	mPlayer(std::make_unique<Player>()),
	mComputer(std::make_unique<Computer>()),
	mSoundOn(true)
{
	generateWorld();
}

void World::generateWorld()
{
	if (!mOceanGridTexture.loadFromFile(OCEAN_GRID))
		throw std::runtime_error("Failed to load png-file from data.");
	mOceanGridSprite.setTexture(mOceanGridTexture);
	if (!mRadarGridTexture.loadFromFile(RADAR_GRID))
		throw std::runtime_error("Failed to load png-file from data.");
	mRadarGridSprite.setTexture(mRadarGridTexture);
	mOceanGridSprite.setPosition(X_MARGIN, Y_MARGIN);
	mRadarGridSprite.setPosition(X2_MARGIN, Y_MARGIN);

	startInit();
}

void World::startInit()
{
	// random first turn
	if (mComputer->getRandNum(0, 1) == 0)
		mTurn = "player";
	else
		mTurn = "computer";

	// setup fleets
	mComputer->setupFleet();

	mFleetPrepare->createFleetForPreparing();
}

void World::checkEvents(sf::RenderWindow& window, const sf::Event& event) 
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
	{
		newGame();
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
	{
		window.close();
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
	{
		// info about this game
		mTextInfo->mShowInfo = true;
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
	{
		// sound on / off
		mSoundOn = !mSoundOn;
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		if (mGameState == GameState::Preparing)
		{
			// Arrangement ships
			mPlayer->setupFleet(mousePos);
		}
		else if (mGameState == GameState::Playing)
		{
			// Player's move
			if (mTurn == "player")
				mPlayer->checkShot(mousePos);
		}
	}
}

void World::update(sf::Time deltaTime)
{
	//sound update
	mPlayer->setSound(mSoundOn);
	mComputer->setSound(mSoundOn);

	if (mGameState == GameState::Preparing)
	{
		if (mPlayer->mShipBuild)
		{
			mFleetPrepare->removePreparedShip();
			mPlayer->mShipBuild = false;
		}
			
		if (mFleetPrepare->isFleetDead())
		{
			mPlayer->setupEnemyData(
				mComputer->getGridPtr().get(), 
				mComputer->getFleetPtr().get(),
				&mComputer->getCells()
			);
			mComputer->setupEnemyData(
				mPlayer->getGridPtr().get(), 
				mPlayer->getFleetPtr().get(),
				&mPlayer->getCells()
			);

			mGameState = GameState::Playing;
		}			
	}
	else if (mGameState == GameState::Playing)
	{
		if (mTurn == "computer")
		{
			if (isDelayActive) {
				// 1. Накапливаем прошедшее время
				timeAccumulator += deltaTime;

				// 2. Проверяем, достигнута ли целевая задержка
				if (timeAccumulator >= moveDelay) {
					// Задержка окончена
					isDelayActive = false;
					timeAccumulator = sf::Time::Zero; // Сброс аккумулятора

					// 3. Выполняем действие, ради которого ждали (ход игрока)
					mComputer->checkShot();
				}

				// 4. Важно: выходим, чтобы не выполнять основную логику, пока идет задержка
				return;
			}
			
			if (mPlayer->isFleetSank())
			{
				mWinner = "computer";
			}
			else if (mComputer->mMissedTarget)
			{
				mComputer->mMissedTarget = false;
				mTurn = "player"; // switch turn to player
			}
			else 
			{
				// Запускаем задержку перед следующим ходом компьютера
				isDelayActive = true;
				timeAccumulator = sf::Time::Zero; // Сброс аккумулятора
			}
		}
		else if (mTurn == "player")
		{
			if (mComputer->isFleetSank())
			{
				mWinner = "player";
			}
			else if (mPlayer->mMissedTarget)
			{
				mPlayer->mMissedTarget = false;
				mTurn = "computer"; // switch turn to computer
			}
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	if (mWinner != "")
		mTextInfo->setWinnerText(mWinner);

	mTextInfo->render(window);

	window.draw(mOceanGridSprite);
	if (mGameState == GameState::Playing)
	{
		// hide mouse cursor for computer turn
		if (mTurn == "computer")
			window.setMouseCursorVisible(false);
		else
			window.setMouseCursorVisible(true);

		window.draw(mRadarGridSprite);
		mPlayer->render(window);
		mComputer->render(window);
	}
	else if (mGameState == GameState::Preparing)
	{
		window.setMouseCursorVisible(true);
		mFleetPrepare->render(window);
		mPlayer->render(window);
	}
}

void World::newGame()
{
	mTextInfo = std::make_unique<TextInfo>();
	mGameState = GameState::Preparing;
	mTurn = "";
	mWinner = "";
	mPlayer = std::make_unique<Player>();
	mComputer = std::make_unique<Computer>();
	mFleetPrepare = std::make_unique<Fleet>();
	startInit();
}

