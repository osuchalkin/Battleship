#include "PlayerBase.h"

#include <iostream>		// for debug

PlayerBase::PlayerBase() :
	mMissedTarget(false),
	mShotIndicator(),
	mCross(),
	mCells(),
	mGrid(std::make_unique<Grid>()),
	mFleet(std::make_unique<Fleet>()),
	mEnemyGrid(nullptr),
	mEnemyFleet(nullptr),
	mEnemyCells(nullptr)
{
	mShotIndicator.setRadius(DECK_SIZE / 6);
	mShotIndicator.setFillColor(HALO_COLOR);
	mShotIndicator.setOrigin(mShotIndicator.getRadius(), mShotIndicator.getRadius());

	// cross for killed ship
	mCross.setPrimitiveType(sf::Lines);
	mCross.resize(4);
	mCross[0].color = KILLED_MARK_COLOR;
	mCross[1].color = KILLED_MARK_COLOR;
	mCross[2].color = KILLED_MARK_COLOR;
	mCross[3].color = KILLED_MARK_COLOR;
}

void PlayerBase::render(sf::RenderWindow& window)
{
}

void PlayerBase::setupCells(int margin)
{
	for (size_t col = 0; col < CELL_WIDTH; col++)
	{
		for (size_t row = 0; row < CELL_HEIGHT; row++)
		{
			mCells[col][row].setPosition(margin + 1 + col * DECK_SPACE,
				Y_MARGIN + 1 + row * DECK_SPACE);
			mCells[col][row].setSize(sf::Vector2f(DECK_SPACE, DECK_SPACE));
		}
	}
}

void PlayerBase::renderCells(sf::RenderWindow& window, int margin)
{
	for (size_t col = 1; col < CELL_WIDTH - 1; col++)
	{
		for (size_t row = 1; row < CELL_HEIGHT - 1; row++)
		{			
			if (mGrid->getCell(col, row) == WOUNDED_CELL)
			{
				mCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCells[col][row].setFillColor(DECK_WOUNDED_COLOR);
				window.draw(mCells[col][row]);
			}
			if (mGrid->getCell(col, row) == KILLED_CELL)
			{
				mCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCells[col][row].setFillColor(DECK_KILLED_COLOR);
				window.draw(mCells[col][row]);
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
			if (mGrid->getCell(col, row) == SHOOT_CELL)
			{
				mShotIndicator.setPosition(margin + 1 + col * DECK_SPACE + DECK_SIZE / 2,
					Y_MARGIN + 1 + row * DECK_SPACE + DECK_SIZE / 2);
				window.draw(mShotIndicator);
			}
			
		}
	}
}

void PlayerBase::renderShips(sf::RenderWindow& window, int margin)
{
	for (size_t col = 1; col < CELL_WIDTH - 1; col++)
	{
		for (size_t row = 1; row < CELL_HEIGHT - 1; row++)
		{
			if (mGrid->getCell(col, row) == SHIP_CELL)
			{
				mCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				mCells[col][row].setFillColor(DECK_COLOR);
				window.draw(mCells[col][row]);
			}
		}
	}
}

void PlayerBase::setSound(bool soundOn)
{
	mGrid->setSound(soundOn);
}
