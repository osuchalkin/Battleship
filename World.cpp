#include "World.h"
#include "Constants.h"
#include "Ship.h"

#include <iostream>		// for debug
#include <memory>
#include <cmath>

// TODO : refactor: create class BasePlayer with derived Player and Computer classes

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
	mTurn(""),
	mWinner(""),
	mShipWounded(false),
	mWoundedCells()
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
			// Расстановка кораблей
			setupPlayerFleet(mousePos);
		}
		else if (mGameState == GameState::Playing)
		{
			// Ход игрока
			if (mTurn == "player")
				checkPlayerShot(mousePos);
		}
	}
}

void World::update(sf::Time deltaTime)
{
	if (mGameState == GameState::Preparing)
	{
		// Расстановка кораблей
		if (mFleetPrepare->isFleetDead())
			mGameState = GameState::Playing;
	}
	else if (mGameState == GameState::Playing)
	{
		
		if (mTurn == "computer")
		{
			// std::cout << "Computer's turn\n";
			checkComputerShot();
			if (mPlayerFleet->isFleetDead())
			{
				std::cout << "Computer WINS!\n";
				mWinner = "computer";
				//mGameState = GameState::GameOver;
			}
		}
		else if (mTurn == "player")
		{
			// std::cout << "Player's turn\n";
			if (mComputerFleet->isFleetDead())
			{
				std::cout << "Player WINS!\n";
				mWinner = "player";
				//mGameState = GameState::GameOver;
			}
		}

	else if (mGameState == GameState::GameOver)
			std::cout << "Game Over\n";
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
			if (side == "player" || (side == "computer" && mWinner == "computer"))	// show computer's ship if it wins
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
	bool success = false;
	const int MAX_ATTEMPTS = 100; // limit on attempts to arrange fleet
	int attempts = 0;

	do
	{
		// clean computer grid and fleet
		mComputerGrid = std::make_unique<Grid>();
		mComputerFleet = std::make_unique<Fleet>();

		int setup = getRandNum(0, 5);

		success = setComputerShip(TypeShip::FourDeck, setup);
		if (success)
			success = setComputerShip(TypeShip::ThreeDeck, setup);
		if (success)
			success = setComputerShip(TypeShip::TwoDeck, setup);
		if (success)
			setComputerShip(TypeShip::OneDeck, 0);

		attempts++;
		if (attempts >= MAX_ATTEMPTS) {
			std::cerr << "Critical error: Cannot arrange fleet after " << MAX_ATTEMPTS << " attempts." << std::endl;
			break;
		}
	} while (!success);
}

bool World::setComputerShip(TypeShip type, int setup) // Random choose valid positions on grid
{
	// all grid
	int widthStart = 1;
	int heightStart = 1;
	int widthEdge = CELL_WIDTH - 1;
	int heightEdge = CELL_HEIGHT - 1;

	switch (setup)
	{
	case 1:		// grid top
		widthStart = 1;
		heightStart = 1;
		widthEdge = CELL_WIDTH - 1;
		heightEdge = CELL_HEIGHT - 6;
		break;
	case 2:		// grid bottom
		widthStart = 1;
		heightStart = CELL_HEIGHT - 6;
		widthEdge = CELL_WIDTH - 1;
		heightEdge = CELL_HEIGHT - 1;
		break;
	case 3:		// grid left
		widthStart = 1;
		heightStart = 1;
		widthEdge = CELL_WIDTH - 6;
		heightEdge = CELL_HEIGHT - 1;
		break;
	case 4:		// grid right
		widthStart = CELL_WIDTH - 6;
		heightStart = 1;
		widthEdge = CELL_WIDTH - 1;
		heightEdge = CELL_HEIGHT - 1;
		break;
	case 5:		// grid edges
		for (int col = 2; col < CELL_HEIGHT - 2; col++)
		{
			for (int row = 2; row < CELL_WIDTH - 2; row++)
			{
				mComputerGrid->setCell(col, row, HALO_CELL);
			}
		}
		break;
	default:
		break;
	}

	int ship = static_cast<int>(type);
	int numberOfShips = mComputerFleet->getNumberOfShips(type);
	// condition to check occupied cells
	auto computerCondition = [&](int c, int r) -> bool {
		return mComputerGrid->getCell(c, r) != EMPTY_CELL; };

	for (size_t i = 0; i < numberOfShips; i++)
	{
		std::vector <std::vector<sf::Vector2i>> validPositions = mComputerGrid->getValidPositions(
			computerCondition, ship,
			widthStart, widthEdge, heightStart, heightEdge);

		// no valid positions
		if (validPositions.empty())
		{
			return false;
		}

		int index = 0;
		if (validPositions.size() > 1)
			index = getRandNum(0, validPositions.size() - 1);
		std::vector<sf::Vector2i> position = validPositions[index];

		mComputerGrid->setShipPositions(position);
		mComputerFleet->createShip(position);
	}

	// return to EMPTY if setup == grid edges 
	if (setup == 5)
	{
		for (int col = 3; col < CELL_HEIGHT - 3; col++)
		{
			for (int row = 3; row < CELL_WIDTH - 3; row++)
			{
				mComputerGrid->setCell(col, row, EMPTY_CELL);
			}
		}
	}
	return true;
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
				if (mComputerGrid->checkShot(col, row))
				{
					// get ship by position
					Ship* ship = mComputerFleet->getShipAtPosition(sf::Vector2i(static_cast<int>(col), static_cast<int>(row)));
					mComputerGrid->woundedOrKilled(ship, col, row);
					mComputerFleet->updateFleet();
				}
				else
				{
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

	if (mPlayerFleet->isFleetDead())
		return;

	sf::Vector2i position;
	if (!mShipWounded)
		position = computerShoot(); // computerShootRandom();
	else
		position = computerSeekShip();
		
	std::cout << "Computer shoots at: (" << position.x << ", " << position.y << ")\n";
	std::cout << "Cell: " << mPlayerGrid->getCell(position.x, position.y) << "\n";

	if (mPlayerGrid->checkShot(position.x, position.y))
	{
		// get ship by position
		Ship* ship = mPlayerFleet->getShipAtPosition(position);
		mPlayerGrid->woundedOrKilled(ship, position.x, position.y);
		if (mPlayerGrid->getCell(position.x, position.y) == KILLED_CELL)
		{
			mPlayerFleet->updateFleet();
			mShipWounded = false;
			mWoundedCells.clear();
		}
		else if (mPlayerGrid->getCell(position.x, position.y) == WOUNDED_CELL)
		{
			mShipWounded = true;
			mWoundedCells.push_back({ position.x, position.y });
		}
	}
	else
	{
		mPlayerGrid->setCell(position.x, position.y, SHOOT_CELL);
		mTurn = "player"; // switch turn to player
	}
}

sf::Vector2i World::computerShootRandom()
{
	/*
	// get valid cells
	std::vector <sf::Vector2i> validCells;
	for (int col = 1; col < CELL_WIDTH - 1; col++)
	{
		for (int row = 1; row < CELL_HEIGHT - 1; row++)
		{
			if (mPlayerGrid->getCell(col, row) != SHOOT_CELL &&
				mPlayerGrid->getCell(col, row) != WOUNDED_CELL &&
				mPlayerGrid->getCell(col, row) != KILLED_CELL)
			{
				validCells.push_back({ col, row });
			}
		}
	}

	int index = 0;
	if (validCells.size() > 1)
		index = getRandNum(0, validCells.size() - 1);

	return validCells[index];
	*/
	return mPlayerGrid->getRandomPositionForShot();
}

sf::Vector2i World::computerShoot()
{
	if (mPlayerFleet->isTypeShipExist(TypeShip::FourDeck))
	{
		// shoot FourDeck ship
		return getCellForShot(TypeShip::FourDeck);
	}
	else if (mPlayerFleet->isTypeShipExist(TypeShip::ThreeDeck))
	{
		// shoot ThreeDeck ship
		return getCellForShot(TypeShip::ThreeDeck);
	}
	else if (mPlayerFleet->isTypeShipExist(TypeShip::TwoDeck))
	{
		// shoot TwoDeck ship
		return getCellForShot(TypeShip::TwoDeck);
	}
	else if (mPlayerFleet->isTypeShipExist(TypeShip::OneDeck))
	{
		// shoot OneDeck ship
		return computerShootRandom();
	}
}

sf::Vector2i World::getCellForShot(TypeShip type)
{
	int numDeck = static_cast<int>(type);
	
	// condition to check occupied cells
	auto playerCondition = [&](int c, int r) -> bool {
		// Используем захваченные поля mPlayerGrid, SHOOT_CELL, WOUNDED_CELL, KILLED_CELL
		return mPlayerGrid->getCell(c, r) != SHOOT_CELL &&
			mPlayerGrid->getCell(c, r) != WOUNDED_CELL &&
			mPlayerGrid->getCell(c, r) != KILLED_CELL;
		};

	std::vector <std::vector<sf::Vector2i>> validPositions = mPlayerGrid->getValidPositions(
		playerCondition, numDeck);

	/*
	for (size_t col = 1; col < CELL_WIDTH - 1; col++)
	{
		for (size_t row = 1; row < CELL_HEIGHT - 1; row++)
		{
			// horizontal
			bool isValid = true;
			std::vector<sf::Vector2i> tempPositions;
			for (size_t j = 0; j < numDeck; j++)
			{
				if (col + j < CELL_WIDTH - 1)
				{
					if (mPlayerGrid->getCell(col + j, row) != EMPTY_CELL &&
						mPlayerGrid->getCell(col + j, row) != HALO_CELL &&
						mPlayerGrid->getCell(col + j, row) != SHIP_CELL)
					{
						isValid = false;
						break;
					}
					else
					{
						tempPositions.push_back(sf::Vector2i(static_cast<int>(col + j), static_cast<int>(row)));
					}
				}
				else
				{
					isValid = false;
					break;
				}
			}
			if (isValid)
				validPositions.push_back(tempPositions);
			// vertical
			isValid = true;
			tempPositions.clear();
			for (size_t j = 0; j < numDeck; j++)
			{
				if (row + j < CELL_HEIGHT - 1)
				{
					if (mPlayerGrid->getCell(col, row + j) != EMPTY_CELL &&
						mPlayerGrid->getCell(col, row + j) != HALO_CELL &&
						mPlayerGrid->getCell(col, row + j) != SHIP_CELL)
					{
						isValid = false;
						break;
					}
					else
					{
						tempPositions.push_back(sf::Vector2i(static_cast<int>(col), static_cast<int>(row + j)));
					}
				}
				else
				{
					isValid = false;
					break;
				}
			}
			if (isValid)
				validPositions.push_back(tempPositions);
		}
	}
	*/

	int index = 0;
	if (validPositions.size() > 1)
		index = getRandNum(0, validPositions.size() - 1);
	std::cout << "Index = " << index << "\n";
	if (validPositions.empty())
	{
		std::cout << "validPositions is Empty";
	}
	std::vector<sf::Vector2i> position = validPositions[index];
	int index2 = 0;
	if (position.size() > 1)
		index2 = getRandNum(0, position.size() - 1);
	
	return position[index2];
}

sf::Vector2i World::computerSeekShip()
{
	std::vector<sf::Vector2i> validCells;

	int size = mWoundedCells.size();

	const int dx[] = { -1, 1, 0, 0 };
	const int dy[] = { 0, 0, -1, 1 };
	const int num_neighbors = 4;

	int x = mWoundedCells[0].x;
	int y = mWoundedCells[0].y;

	switch (size)
	{
	case 1:
		for (int i = 0; i < num_neighbors; i++)
		{
			int nx = x + dx[i];
			int ny = y + dy[i];
			bool in_bounds = (nx >= 1 && nx < CELL_WIDTH - 1 &&
				ny >= 1 && ny < CELL_HEIGHT - 1);
			if (in_bounds)
			{
				if (mPlayerGrid->getCell(nx, ny) == EMPTY_CELL || mPlayerGrid->getCell(nx, ny) == HALO_CELL ||
					mPlayerGrid->getCell(nx, ny) == SHIP_CELL)
				{
					validCells.push_back({ nx, ny });
				}
			}
		}
		break;
	case 2: case 3:
		if (mWoundedCells[0].x == mWoundedCells[1].x) // vertical
		{
			// find min y and max y
			int y1 = mWoundedCells[0].y;
			int y2 = mWoundedCells[0].y;
			for (size_t i = 1; i < mWoundedCells.size(); ++i)
			{
				const sf::Vector2i& cell = mWoundedCells[i];
				y1 = std::min(y1, cell.y);
				y2 = std::max(y2, cell.y);
			}
			y1--;
			y2++;
			
			// check valid cells
			bool in_bounds = (y1 >= 1 && y1 < CELL_HEIGHT - 1);
			if (in_bounds)
			{
				if (mPlayerGrid->getCell(x, y1) == EMPTY_CELL || mPlayerGrid->getCell(x, y1) == HALO_CELL ||
					mPlayerGrid->getCell(x, y1) == SHIP_CELL)
				{
					validCells.push_back({ x, y1 });
				}
			}
			in_bounds = (y2 >= 1 && y2 < CELL_HEIGHT - 1);
			if (in_bounds)
			{
				if (mPlayerGrid->getCell(x, y2) == EMPTY_CELL || mPlayerGrid->getCell(x, y2) == HALO_CELL ||
					mPlayerGrid->getCell(x, y2) == SHIP_CELL)
				{
					validCells.push_back({ x, y2 });
				}
			}
		}
		else if (mWoundedCells[0].y == mWoundedCells[1].y) // horizontal
		{
			// find min x and max y
			int x1 = mWoundedCells[0].x;
			int x2 = mWoundedCells[0].x;
			for (size_t i = 1; i < mWoundedCells.size(); ++i)
			{
				const sf::Vector2i& cell = mWoundedCells[i];
				x1 = std::min(x1, cell.x);
				x2 = std::max(x2, cell.x);
			}
			x1--;
			x2++;

			// check valid cells
			bool in_bounds = (x1 >= 1 && x1 < CELL_WIDTH - 1);
			if (in_bounds)
			{
				if (mPlayerGrid->getCell(x1, y) == EMPTY_CELL || mPlayerGrid->getCell(x1, y) == HALO_CELL ||
					mPlayerGrid->getCell(x1, y) == SHIP_CELL)
				{
					validCells.push_back({ x1, y });
				}
			}
			in_bounds = (x2 >= 1 && x2 < CELL_WIDTH - 1);
			if (in_bounds)
			{
				if (mPlayerGrid->getCell(x2, y) == EMPTY_CELL || mPlayerGrid->getCell(x2, y) == HALO_CELL ||
					mPlayerGrid->getCell(x2, y) == SHIP_CELL)
				{
					validCells.push_back({ x2, y });
				}
			}
		}
		break;
	default:
		break;
	}

	// choose random valid cell
	int index = 0; 
	if (validCells.size() > 1)
		index = getRandNum(0, validCells.size() - 1);

	return validCells[index];
}