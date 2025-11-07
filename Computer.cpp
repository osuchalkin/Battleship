#include "Computer.h"

#include <iostream>


Computer::Computer() :
	PlayerBase(),
	mNumberOfMeethods(10),
	mShipWounded(false),
	mWoundedCells()
{
	mt.seed(std::random_device{}());

	setupCells(X2_MARGIN);
}

void Computer::render(sf::RenderWindow& window)
{
	PlayerBase::renderCells(window, X2_MARGIN);
	if (mEnemyFleet->isFleetDead())
		PlayerBase::renderShips(window, X2_MARGIN);
}

int Computer::getRandNum(int first, int last)
{
	std::uniform_int_distribution<> myNumber{ first, last };
	return myNumber(mt);
}

void Computer::setupFleet()
{
	bool success = false;
	const int MAX_ATTEMPTS = 100; // limit on attempts to arrange fleet
	int attempts = 0;

	// random setup method
	int setup = 7;// getRandNum(0, mNumberOfMeethods - 1);

	do
	{
		// clean computer grid and fleet
		mGrid = std::make_unique<Grid>();
		mFleet = std::make_unique<Fleet>();

		success = setShip(TypeShip::FourDeck, setup);
		if (success)
			success = setShip(TypeShip::ThreeDeck, setup);
		if (success)
			success = setShip(TypeShip::TwoDeck, setup);
		if (success)
			setShip(TypeShip::OneDeck, 0);

		attempts++;
		std::cout << "Setup is method: " << setup << std::endl;
		std::cout << "Computer fleet arrangement attempt: " << attempts << std::endl;
		if (attempts >= MAX_ATTEMPTS) {
			std::cerr << "Critical error: Cannot arrange fleet after " << MAX_ATTEMPTS << " attempts." << std::endl;
			std::cout << "Switching to next arrangement method: " << std::endl;
			attempts = 0;
			setup++;
			//break;
		}
	} while (!success);
}

bool Computer::setShip(TypeShip type, int setup)
{
	// 6 methods of ship arrangement:  random, top, bottom, left, right, edges
	// all grid
	int widthStart = 1;
	int heightStart = 1;
	int widthEdge = CELL_WIDTH - 1;
	int heightEdge = CELL_HEIGHT - 1;
	std::string placement = "Both";	// Vertical and Horizontal

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
				mGrid->setCell(col, row, HALO_CELL);
			}
		}
		break;
	case 6:		// 3 top horizontal rows
		widthStart = 1;
		heightStart = 1;
		widthEdge = CELL_WIDTH - 1;
		heightEdge = CELL_HEIGHT - 8;
		placement = "Horizontal";
		break;
	case 7:		// 3 bottom horizontal rows
		widthStart = 1;
		heightStart = CELL_HEIGHT - 4;
		widthEdge = CELL_WIDTH - 1;
		heightEdge = CELL_HEIGHT - 1;
		placement = "Horizontal";
		break;
	case 8:		// 3 left vertical columns
		widthStart = 1;
		heightStart = 1;
		widthEdge = CELL_WIDTH - 8;
		heightEdge = CELL_HEIGHT - 1;
		placement = "Vertical";
		break;
	case 9:		// 3 right vertical columns
		widthStart = CELL_WIDTH - 4;
		heightStart = 1;
		widthEdge = CELL_WIDTH - 1;
		heightEdge = CELL_HEIGHT - 1;
		placement = "Vertical";
		break;
	default:
		break;
	}

	int ship = static_cast<int>(type);
	int numberOfShips = mFleet->getNumberOfShips(type);

	// condition to check occupied cells
	auto computerCondition = [&](int c, int r) -> bool {
		return mGrid->getCell(c, r) != EMPTY_CELL; };

	for (size_t i = 0; i < numberOfShips; i++)
	{
		std::vector <std::vector<sf::Vector2i>> validPositions = mGrid->getValidPositions(
			computerCondition, ship, placement,
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

		mGrid->setShipPositions(position);
		mFleet->createShip(position);
	}

	// return to EMPTY if setup == grid edges 
	if (setup == 5)
	{
		for (int col = 3; col < CELL_HEIGHT - 3; col++)
		{
			for (int row = 3; row < CELL_WIDTH - 3; row++)
			{
				mGrid->setCell(col, row, EMPTY_CELL);
			}
		}
	}
	return true;
}

void Computer::checkShot()
{
	if (mEnemyFleet->isFleetDead())
		return;

	sf::Vector2i position;
	if (!mShipWounded)
		position = shooting();
	else
		position = seekShip();

	if (mEnemyGrid->checkShot(position.x, position.y))
	{
		// get ship by position
		Ship* ship = mEnemyFleet->getShipAtPosition(position);
		mEnemyGrid->woundedOrKilled(ship, position.x, position.y);
		if (mEnemyGrid->getCell(position.x, position.y) == KILLED_CELL)
		{
			mEnemyFleet->updateFleet();
			mShipWounded = false;
			mWoundedCells.clear();
		}
		else if (mEnemyGrid->getCell(position.x, position.y) == WOUNDED_CELL)
		{
			mShipWounded = true;
			mWoundedCells.push_back({ position.x, position.y });
		}
	}
	else
	{
		mEnemyGrid->setCell(position.x, position.y, SHOOT_CELL);
		mMissedTarget = true;
	}
}

sf::Vector2i Computer::shooting()
{
	if (mEnemyFleet->isTypeShipExist(TypeShip::FourDeck))
	{
		// shoot FourDeck ship
		return getCellForShot(TypeShip::FourDeck);
	}
	else if (mEnemyFleet->isTypeShipExist(TypeShip::ThreeDeck))
	{
		// shoot ThreeDeck ship
		return getCellForShot(TypeShip::ThreeDeck);
	}
	else if (mEnemyFleet->isTypeShipExist(TypeShip::TwoDeck))
	{
		// shoot TwoDeck ship
		return getCellForShot(TypeShip::TwoDeck);
	}
	else if (mEnemyFleet->isTypeShipExist(TypeShip::OneDeck))
	{
		// shoot OneDeck ship
		return getCellForShot(TypeShip::OneDeck);
	}
}

sf::Vector2i Computer::getCellForShot(TypeShip type)
{
	int numDeck = static_cast<int>(type);

	// condition to check occupied cells
	auto playerCondition = [&](int c, int r) -> bool {
		return mEnemyGrid->getCell(c, r) != EMPTY_CELL &&
			mEnemyGrid->getCell(c, r) != HALO_CELL &&
			mEnemyGrid->getCell(c, r) != SHIP_CELL;
		};

	std::vector <std::vector<sf::Vector2i>> validPositions = mEnemyGrid->getValidPositions(
		playerCondition, numDeck);

	int index = 0;
	if (validPositions.size() > 1)
		index = getRandNum(0, validPositions.size() - 1);

	std::vector<sf::Vector2i> position = validPositions[index];
	int index2 = 0;
	if (position.size() > 1)
		index2 = getRandNum(0, position.size() - 1);

	return position[index2];
}

sf::Vector2i Computer::seekShip()
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
				if (mEnemyGrid->getCell(nx, ny) == EMPTY_CELL || mEnemyGrid->getCell(nx, ny) == HALO_CELL ||
					mEnemyGrid->getCell(nx, ny) == SHIP_CELL)
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
				if (mEnemyGrid->getCell(x, y1) == EMPTY_CELL || mEnemyGrid->getCell(x, y1) == HALO_CELL ||
					mEnemyGrid->getCell(x, y1) == SHIP_CELL)
				{
					validCells.push_back({ x, y1 });
				}
			}
			in_bounds = (y2 >= 1 && y2 < CELL_HEIGHT - 1);
			if (in_bounds)
			{
				if (mEnemyGrid->getCell(x, y2) == EMPTY_CELL || mEnemyGrid->getCell(x, y2) == HALO_CELL ||
					mEnemyGrid->getCell(x, y2) == SHIP_CELL)
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
				if (mEnemyGrid->getCell(x1, y) == EMPTY_CELL || mEnemyGrid->getCell(x1, y) == HALO_CELL ||
					mEnemyGrid->getCell(x1, y) == SHIP_CELL)
				{
					validCells.push_back({ x1, y });
				}
			}
			in_bounds = (x2 >= 1 && x2 < CELL_WIDTH - 1);
			if (in_bounds)
			{
				if (mEnemyGrid->getCell(x2, y) == EMPTY_CELL || mEnemyGrid->getCell(x2, y) == HALO_CELL ||
					mEnemyGrid->getCell(x2, y) == SHIP_CELL)
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