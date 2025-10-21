#include "World.h"
#include "Constants.h"

#include <iostream>		// for debug
#include <memory>


World::World() :
	mOceanGridTexture(),
	mOceanGridSprite(),
	mRadarGridTexture(),
	mRadarGridSprite(),
	mFont(),
	mTitle(),
	mInfoText(),
	mGameState(GameState::Preparing),
	mFleetPrepare(std::make_unique<Fleet>()),
	mPlayerGrid(std::make_unique<Grid>()),
	mCells(),
	mComputerGrid(std::make_unique<Grid>()),
	mCompCells(),
	mPlayerFleet(std::make_unique<Fleet>()),
	mComputerFleet(std::make_unique<Fleet>()),
	mTurn("")
{
	mt.seed(std::random_device{}());

	setupCells();
	setupText();
	generateWorld();
}

int World::getRandNum(int first, int last)
{
	std::uniform_int_distribution<> myNumber{ first, last };
	return myNumber(mt);
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

	if (getRandNum(0, 1) == 0)
		mTurn = "player";
	else
		mTurn = "computer";

	setupComputerFleet();
	mFleetPrepare->createFleetForPreparing();
}

void World::checkEvents(sf::RenderWindow& window, const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
	{
		// new game
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
	{
		window.close();
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
	{
		// info
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
	{
		// sound on / off
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		if (mGameState == GameState::Preparing)
		{
			// Расстановка кораблей
			setupPlayerFleet(mousePos);
		}
		else if (mGameState == GameState::Playing)
		{
			// Ход игрока
			checkPlayerShot(mousePos);
		}
	}
}

void World::update(sf::Time deltaTime)
{
	if (mGameState == GameState::Preparing)
	{
		// Расстановка кораблей
		if (mFleetPrepare->mIsFleetPrepareReady())
			mGameState = GameState::Playing;
	}
	else
	{
		// Игра
		if (mGameState == GameState::Playing)
		{
			if (mTurn == "player")
			{
				//std::cout << "Player's turn\n";
			}
			else
			{
				//std::cout << "Computer's turn\n";
			}
		// else if (mGameState == GameState::GameOver)
			// показать победителя, начать заново
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	window.draw(mTitle);
	window.draw(mInfoText);

	window.draw(mOceanGridSprite);
	if (mGameState == GameState::Playing)
	{
		window.draw(mRadarGridSprite);
		renderCells(window);
	}
	else if (mGameState == GameState::Preparing)
	{
		mFleetPrepare->render(window);
		renderCells(window);
	}
}

void World::setupCells()
{
	for (size_t col = 0; col < CELL_WIDTH; col++)
	{
		for (size_t row = 0; row < CELL_HEIGHT; row++)
		{
			mCells[col][row].setPosition(X_MARGIN + 1 + col * DECK_SPACE,
				Y_MARGIN + 1 + row * DECK_SPACE);
			mCells[col][row].setSize(sf::Vector2f(DECK_SPACE, DECK_SPACE));
			// computer cells
			mCompCells[col][row].setPosition(X2_MARGIN + 1 + col * DECK_SPACE,
				Y_MARGIN + 1 + row * DECK_SPACE);
			mCompCells[col][row].setSize(sf::Vector2f(DECK_SPACE, DECK_SPACE));
		}
	}
}

void World::renderCells(sf::RenderWindow& window)
{
	for (size_t col = 0; col < CELL_WIDTH; col++)
	{
		for (size_t row = 0; row < CELL_HEIGHT; row++)
		{
			if (mPlayerGrid->getCell(col, row) == SHIP_CELL)
			{
				//mCells[col][row].setPosition(X_MARGIN + 1 + col * DECK_SPACE,
				//	Y_MARGIN + 1 + row * DECK_SPACE);
				mCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCells[col][row].setFillColor(DECK_COLOR); 
				window.draw(mCells[col][row]);
			}
			if (mComputerGrid->getCell(col, row) == WOUNDED_CELL)
			{
				mCompCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCompCells[col][row].setFillColor(DECK_WOUNDED_COLOR);
				window.draw(mCompCells[col][row]);
			}
			if (mComputerGrid->getCell(col, row) == KILLED_CELL)
			{
				mCompCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCompCells[col][row].setFillColor(DECK_KILLED_COLOR);
				window.draw(mCompCells[col][row]);
			}
			/*
			if (mComputerGrid->getCell(col, row) == 'S')
			{
				//mCompCells[col][row].setPosition(X2_MARGIN + 1 + col * DECK_SPACE,
				//	Y_MARGIN + 1 + row * DECK_SPACE);
				mCompCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCompCells[col][row].setFillColor(DECK_COLOR);
				window.draw(mCompCells[col][row]);
			}
			*/
		}
	}
}

void World::setupText()
{
	if (!mFont.loadFromFile(TTF))
		throw std::runtime_error("Failed to load ttf-file from data.");
	
	// Title
	mTitle.setFont(mFont);
	mTitle.setCharacterSize(60);
	mTitle.setFillColor(TEXT_COLOR);
	mTitle.setString("BATTLESHIP");
	mTitle.setStyle(sf::Text::Bold);
	float textHeight = mTitle.getLocalBounds().height;
	mTitle.setPosition((SCREEN_WIDTH - mTitle.getLocalBounds().width) / 2, Y_MARGIN / 2 - textHeight);
	mTitle.setOutlineColor(TEXT_SHADOW_COLOR);
	mTitle.setOutlineThickness(2);

	// Info text
	mInfoText.setFont(mFont);
	mInfoText.setCharacterSize(20);
	mInfoText.setFillColor(TEXT_COLOR);
	mInfoText.setString("I - info text\tN - new game\tS - sound on / off\tQ - quit");
	//mInfoText.setStyle(sf::Text::Bold);
	mInfoText.setPosition((SCREEN_WIDTH - mInfoText.getLocalBounds().width) / 2, SCREEN_HEIGHT - Y_MARGIN / 2);
	mInfoText.setOutlineColor(TEXT_SHADOW_COLOR);
	mInfoText.setOutlineThickness(1);
}

void World::setupPlayerFleet(sf::Vector2i mousePos)
{
	// check mouse position in left grid
	if (mousePos.x < X_MARGIN + DECK_SPACE || mousePos.x > X_MARGIN + GRID_WIDTH ||
		mousePos.y < Y_MARGIN + DECK_SPACE || mousePos.y > Y_MARGIN + GRID_WIDTH)
		return;

	// check which cell is clicked
	for (size_t col = 0; col < CELL_HEIGHT; col++)
	{
		for (size_t row = 0; row < CELL_WIDTH; row++)
		{
			if (mCells[col][row].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				mPlayerGrid->setDeck(col, row);
				if (mPlayerGrid->mIsShipReady)
				{
					mPlayerFleet->createShip(mPlayerGrid->mPositions);
					// remove ship from preparing fleet
					mFleetPrepare->removePreparedShip();
					mPlayerGrid->mIsShipReady = false;
					// clear positions for next ship
					mPlayerGrid->mPositions.clear();
				}
			}
		}
	}
}

void World::setupComputerFleet()
{
	while (mComputerGrid->getDeckCount() < TOTAL_DECKS_COMPUTER)
	{
		int x = getRandNum(1, 10);
		int y = getRandNum(1, 10);
		mComputerGrid->setDeck(x, y);

		if (mComputerGrid->mIsShipReady)
		{
			mComputerFleet->createShip(mComputerGrid->mPositions);
			mComputerGrid->mIsShipReady = false;
			mComputerGrid->mPositions.clear();
		}
	}
}

void World::checkPlayerShot(sf::Vector2i mousePos)
{
	// check mouse position in left grid
	if (mousePos.x < X2_MARGIN + DECK_SPACE || mousePos.x > X2_MARGIN + GRID_WIDTH ||
		mousePos.y < Y_MARGIN + DECK_SPACE || mousePos.y > Y_MARGIN + GRID_WIDTH)
		return;
	// check which cell is clicked
	for (size_t col = 0; col < CELL_HEIGHT; col++)
	{
		for (size_t row = 0; row < CELL_WIDTH; row++)
		{
			if (mCompCells[col][row].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				// process shot
				std::cout << "Player shot at cell: (" << col << ", " << row << ")\n";
				if (mComputerGrid->checkShot(col, row))
				{
					// получить тип корабля по координатам
					TypeShip type = mComputerFleet->getTypeShip(sf::Vector2i(static_cast<int>(col), static_cast<int>(row)));
					mComputerGrid->woundedOrKilled(type, col, row);
					std::cout << "Hit! " << static_cast<int>(type) << "-deck ship.\n";
				}
				else
				{
					std::cout << "Miss!\n";
					// mTurn = "computer"; // switch turn to computer
				}
			}
		}
	}
}