#include "Grid.h"

#include <iostream>		// for debug	

static bool isNumberInSet(int number, const std::string& set_name) {
	// Находим нужный набор по имени
	auto it = mDeckSet.find(set_name);

	if (it == mDeckSet.end())
		return false;

	// Выполняем быструю O(1) проверку
	return it->second.count(number);
}


Grid::Grid() :
	mIsShipReady(false),
	mGridCells(),
	mDeckCount(0),
	mSoundOn(true),
	mRecorder(std::make_unique<SoundsRecorder>())
{
	//clearGrid();
	for (auto& row : mGridCells)
	{
		row.fill(EMPTY_CELL);
	}
}

void Grid::setDeck(int x, int y)
{
	if (mGridCells[x][y] != EMPTY_CELL)
		return;
	else if (checkPlaceShip(x, y))
	{
		mGridCells[x][y] = SHIP_CELL;

		if (mSoundOn)
			mRecorder->playSound(SoundType::SetDeck);

		mDeckCount++;
		mPositions.push_back({ x, y });
		if (isNumberInSet(mDeckCount, "ShipReady"))
		{
			setHalo(SHIP_CELL);
			mIsShipReady = true;
		}
	}
}

void Grid::setShipPositions(const std::vector<sf::Vector2i>& positions)
{
	for (const auto& pos : positions)
	{
		mGridCells[pos.x][pos.y] = SHIP_CELL;
	}
	setHalo(SHIP_CELL);
}

bool Grid::checkPlaceShip(int x, int y)
{
	if (mGridCells[x][y] != EMPTY_CELL)
		return false;

	mGridCells[x][y] = SHIP_CELL;
	if (isNumberInSet(mDeckCount, "SetA"))
	{
		return true;
	}
	else if (isNumberInSet(mDeckCount, "SetB"))
	{
		if (mGridCells[x][y] == mGridCells[x + 1][y] || mGridCells[x][y] == mGridCells[x - 1][y]
			|| mGridCells[x][y] == mGridCells[x][y + 1] || mGridCells[x][y] == mGridCells[x][y - 1])
			return true;
	}
	else if (isNumberInSet(mDeckCount, "SetC"))
	{
		if (x > 1 && x < 10)
		{
			if ((mGridCells[x - 1][y] == SHIP_CELL && mGridCells[x - 2][y] == SHIP_CELL)
				|| (mGridCells[x + 1][y] == SHIP_CELL && mGridCells[x + 2][y] == SHIP_CELL))
				return true;
		}
		if (y > 1 && y < 10)
		{
			if ((mGridCells[x][y - 1] == SHIP_CELL && mGridCells[x][y - 2] == SHIP_CELL)
				|| (mGridCells[x][y + 1] == SHIP_CELL && mGridCells[x][y + 2] == SHIP_CELL))
				return true;
		}
		if (x == 1)
		{
			if (mGridCells[x + 1][y] == SHIP_CELL && mGridCells[x + 2][y] == SHIP_CELL)
				return true;
		}
		if (x == 10)
		{
			if (mGridCells[x - 1][y] == SHIP_CELL && mGridCells[x - 2][y] == SHIP_CELL)
				return true;
		}
		if (y == 1)
		{
			if (mGridCells[x][y + 1] == SHIP_CELL && mGridCells[x][y + 2] == SHIP_CELL)
				return true;
		}
		if (y == 10)
		{
			if (mGridCells[x][y - 1] == SHIP_CELL && mGridCells[x][y - 2] == SHIP_CELL)
				return true;
		}
	}
	// not valid place for ship
	mGridCells[x][y] = EMPTY_CELL;
	return false;
}

void Grid::setHalo(char status)
{
	// Массивы смещений для 8 соседних ячеек (Moore neighborhood)
	const int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	const int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	const int num_neighbors = 8;
	
	// Определяем статус, который нужно установить
	const char status_to_set = (status == SHIP_CELL) ? HALO_CELL : SHOOT_CELL;

	for (int x = 1; x < CELL_WIDTH - 1; x++)
	{
		for (int y = 1; y < CELL_HEIGHT - 1; y++)
		{
			if (mGridCells[x][y] == status)
			{// Проверяем все 8 соседних ячеек
				for (int i = 0; i < num_neighbors; i++)
				{
					int nx = x + dx[i];
					int ny = y + dy[i];

					if (mGridCells[nx][ny] == EMPTY_CELL || mGridCells[nx][ny] == HALO_CELL)
					{
						mGridCells[nx][ny] = status_to_set;
					}
				}
			}
		}
	}
}

bool Grid::checkShot(int x, int y)
{
	if (mGridCells[x][y] == SHIP_CELL || mGridCells[x][y] == WOUNDED_CELL || mGridCells[x][y] == KILLED_CELL)
		return true;
	else
	{
		if (mSoundOn)
			mRecorder->playSound(SoundType::Shot);
		return false;
	}
}

void Grid::woundedOrKilled(Ship* ship, int x, int y)
{
	ship->registerHit({ static_cast<float>(x), static_cast<float>(y) });
	if (ship->isSunk())
	{
		for (const auto& pos : ship->getPositions())
		{
			mGridCells[pos.x][pos.y] = KILLED_CELL;
			if (mSoundOn)
				mRecorder->playSound(SoundType::Sunk);
		}
		ship->setShipState(ShipState::Sunk);
		setHalo(KILLED_CELL);
	}
	else
	{
		mGridCells[x][y] = WOUNDED_CELL;
		if (mSoundOn)
			mRecorder->playSound(SoundType::Wounded);
	}
}

std::vector <std::vector<sf::Vector2i>> Grid::getValidPositions(ConditionPredicate checkCondition, int ship,
	std::string placement, int colStart, int colFinish, int rowStart, int rowFinish)
{
	std::vector<std::vector<sf::Vector2i>> validPositions;
	// find valid positions for ship type
	for (int col = colStart; col < colFinish; col++)
	{
		for (int row = rowStart; row < rowFinish; row++)
		{
			bool isValid = true;
			std::vector<sf::Vector2i> tempPositions;
			// horizontal
			if (placement != "Vertical")
			{
				for (size_t j = 0; j < ship; j++)
				{
					if (col + j < colFinish)
					{
						if (checkCondition(col + j, row))
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
			}	
			// vertical
			if (placement != "Horizontal")
			{
				isValid = true;
				tempPositions.clear();
				for (size_t j = 0; j < ship; j++)
				{
					if (row + j < rowFinish)
					{
						if (checkCondition(col, row + j))
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
	}
	return validPositions;
}
