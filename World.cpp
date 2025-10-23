#include "World.h"
#include "Constants.h"
#include "Ship.h"

#include <iostream>		// for debug
#include <memory>
#include <cmath>


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
	mShotIndicator(),
	mCross(),
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

	mShotIndicator.setRadius(DECK_SIZE / 6);
	mShotIndicator.setFillColor(HALO_COLOR);
	mShotIndicator.setOrigin(mShotIndicator.getRadius(), mShotIndicator.getRadius());

	// cross for killed ship
	mCross.setPrimitiveType(sf::Lines);
	mCross.resize(4);
	mCross[0].color = sf::Color::White;
	mCross[1].color = sf::Color::White;
	mCross[2].color = sf::Color::White;
	mCross[3].color = sf::Color::White;


	// random first turn
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
			// –асстановка кораблей
			setupPlayerFleet(mousePos);
		}
		else if (mGameState == GameState::Playing)
		{
			// ’од игрока
			if (mTurn == "player")
				checkPlayerShot(mousePos);
		}
	}
}

void World::update(sf::Time deltaTime)
{
	if (mGameState == GameState::Preparing)
	{
		// –асстановка кораблей
		if (mFleetPrepare->mIsFleetPrepareReady())
			mGameState = GameState::Playing;
	}
	else
	{
		// »гра
		if (mGameState == GameState::Playing)
		{
			if (mTurn == "computer")
			{
				//std::cout << "Player's turn\n";
				checkComputerShot();
			}
			else
			{
				//std::cout << "Computer's turn\n";
			}
		// else if (mGameState == GameState::GameOver)
			// показать победител€, начать заново
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
		renderCells(window, "player", mPlayerGrid.get(), mCells);
		renderCells(window, "computer", mComputerGrid.get(), mCompCells);
	}
	else if (mGameState == GameState::Preparing)
	{
		mFleetPrepare->render(window);
		renderCells(window, "player", mPlayerGrid.get(), mCells);
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
			// computer's cells
			mCompCells[col][row].setPosition(X2_MARGIN + 1 + col * DECK_SPACE,
				Y_MARGIN + 1 + row * DECK_SPACE);
			mCompCells[col][row].setSize(sf::Vector2f(DECK_SPACE, DECK_SPACE));
		}
	}
}

void World::renderCells(sf::RenderWindow& window, std::string side, Grid* grid,
	std::array<std::array<sf::RectangleShape, CELL_WIDTH>, CELL_HEIGHT>& cells)
{
	float margin = 0;
	if (side == "player")
	{
		margin = X_MARGIN;
	}
	else if (side == "computer")
	{
		margin = X2_MARGIN;
	}

	for (size_t col = 1; col < CELL_WIDTH - 1; col++)
	{
		for (size_t row = 1; row < CELL_HEIGHT - 1; row++)
		{
			if (side == "player" /* && !winner */)	// TODO show computer's ship if it wins
			{
				if (grid->getCell(col, row) == SHIP_CELL)
				{
					cells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
					cells[col][row].setFillColor(DECK_COLOR);
					window.draw(cells[col][row]);
				}
			}
			if (grid->getCell(col, row) == WOUNDED_CELL)
			{
				cells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				cells[col][row].setFillColor(DECK_WOUNDED_COLOR);
				window.draw(cells[col][row]);
			}
			if (grid->getCell(col, row) == KILLED_CELL)
			{
				cells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				cells[col][row].setFillColor(DECK_KILLED_COLOR);
				window.draw(cells[col][row]);
				// draw cross
				mCross[0].position = sf::Vector2f(margin + 1 + col * DECK_SPACE - offset + DECK_SIZE / 2,
					Y_MARGIN + 1 + row * DECK_SPACE - offset + DECK_SIZE / 2);
				mCross[1].position = sf::Vector2f(margin + 1 + col * DECK_SPACE + offset + DECK_SIZE / 2,
					Y_MARGIN + 1 + row * DECK_SPACE + offset + DECK_SIZE / 2);
				mCross[2].position = sf::Vector2f(margin + 1 + col * DECK_SPACE - offset + DECK_SIZE / 2,
					Y_MARGIN + 1 + row * DECK_SPACE + offset + DECK_SIZE / 2);
				mCross[3].position = sf::Vector2f(margin + 1 + col * DECK_SPACE + offset + DECK_SIZE / 2,
					Y_MARGIN + 1 + row * DECK_SPACE - offset + DECK_SIZE / 2);
				window.draw(mCross);
			}
			if (grid->getCell(col, row) == SHOOT_CELL)
			{
				mShotIndicator.setPosition(margin + 1 + col * DECK_SPACE + DECK_SIZE / 2,
					Y_MARGIN + 1 + row * DECK_SPACE + DECK_SIZE / 2);
				window.draw(mShotIndicator);
			}
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
					// получить корабль по координатам
					Ship* ship = mComputerFleet->getShipAtPosition(sf::Vector2i(static_cast<int>(col), static_cast<int>(row)));
					mComputerGrid->woundedOrKilled(ship, col, row);
					std::cout << "Hit!\n";
				}
				else
				{
					std::cout << "Miss!\n";
					mComputerGrid->setCell(col, row, SHOOT_CELL);
					mTurn = "computer"; // switch turn to computer
				}
			}
		}
	}
}

void World::checkComputerShot()
{
	//sf::sleep(sf::milliseconds(500)); // pause for better effect

	// random shot
	int x = getRandNum(1, 10);
	int y = getRandNum(1, 10);
	std::cout << "Computer shot at cell: (" << x << ", " << y << ")\n";
	if (mPlayerGrid->checkShot(x, y))
	{
		// получить корабль по координатам
		Ship* ship = mPlayerFleet->getShipAtPosition(sf::Vector2i(static_cast<int>(x), static_cast<int>(y)));
		mPlayerGrid->woundedOrKilled(ship, x, y);
		std::cout << "Hit!\n";
	}
	else
	{
		std::cout << "Miss!\n";
		mPlayerGrid->setCell(x, y, SHOOT_CELL);
		mTurn = "player"; // switch turn to player
	}
}