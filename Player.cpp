#include "Player.h"

#include <iostream>		// for debug

Player::Player() :
	PlayerBase(),
	mShipBuild(false)
{
	setupCells(X_MARGIN);
}

void Player::render(sf::RenderWindow& window)
{
	PlayerBase::renderCells(window, X_MARGIN);
	PlayerBase::renderShips(window, X_MARGIN);
}

void Player::setupFleet(sf::Vector2i mousePos)
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
				mGrid->setDeck(col, row);
				if (mGrid->mIsShipReady)
				{
					mFleet->createShip(mGrid->mPositions);
					// remove ship from preparing fleet
					mShipBuild = true;
					mGrid->mIsShipReady = false;
					// clear positions for next ship
					mGrid->mPositions.clear();
				}
			}
		}
	}
}

void Player::checkShot(sf::Vector2i mousePos)
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
			if (mEnemyCells->at(col).at(row).getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				// process shot
				if (mEnemyGrid->checkShot(col, row))
				{
					// get ship by position
					Ship* ship = mEnemyFleet->getShipAtPosition(sf::Vector2i(static_cast<int>(col), static_cast<int>(row)));
					mEnemyGrid->woundedOrKilled(ship, col, row);
					mEnemyFleet->updateFleet();
				}
				else
				{
					mEnemyGrid->setCell(col, row, SHOOT_CELL);
					mMissedTarget = true;
				}
			}
		}
	}
}