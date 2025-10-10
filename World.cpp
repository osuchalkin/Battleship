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
	mCells()
{
	setupCells();
	setupText();
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
		if (mGameState == GameState::Preparing)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			// Расстановка кораблей
			setupPlayerFleet(mousePos);
		}
		else if (mGameState == GameState::Playing)
		{
			// Ход игрока
		}
	}
}

void World::update(sf::Time deltaTime)
{
	if (mGameState == GameState::Preparing)
	{
		// Расстановка кораблей
	}
	else
	{
		// Игра
		if (mGameState == GameState::Playing)
		{
			// if (ход игрока - mTurn == "player")
				// получить ход игрока
			// else
				// получить ход компьютера
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
		window.draw(mRadarGridSprite);
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
			mCells[col][row].setPosition(X_MARGIN + row * DECK_SPACE,
				Y_MARGIN + col * DECK_SPACE);
			mCells[col][row].setSize(sf::Vector2f(DECK_SPACE, DECK_SPACE));
		}
	}
}

void World::renderCells(sf::RenderWindow& window)
{
	for (size_t col = 0; col < CELL_WIDTH; col++)
	{
		for (size_t row = 0; row < CELL_HEIGHT; row++)
		{
			if (mPlayerGrid->getCell(row, col) == 'S')
			{
				mCells[col][row].setPosition(X_MARGIN + 1 + row * DECK_SPACE,
					Y_MARGIN + 1 + col * DECK_SPACE);
				mCells[col][row].setSize(sf::Vector2f(DECK_SIZE, DECK_SIZE));
				window.draw(mCells[col][row]);
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
	for (size_t col = 0; col < CELL_WIDTH; col++)
	{
		for (size_t row = 0; row < CELL_HEIGHT; row++)
		{
			if (mCells[col][row].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				mPlayerGrid->setDeck(row, col);
				if (mPlayerGrid->getCell(row, col) == 'S')
					mCells[col][row].setFillColor(DECK_COLOR);
			}
		}
	}
}