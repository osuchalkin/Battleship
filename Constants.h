#pragma once

#include<SFML/Graphics.hpp>

// FPS
const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);

constexpr float SCREEN_WIDTH = 1200;
constexpr float SCREEN_HEIGHT = 800;

constexpr float GRID_WIDTH = 452;	// png size
constexpr float X_MARGIN = (SCREEN_WIDTH / 2 - GRID_WIDTH) / 2;
constexpr float X2_MARGIN = SCREEN_WIDTH - (GRID_WIDTH + X_MARGIN);
constexpr float Y_MARGIN = (SCREEN_HEIGHT - GRID_WIDTH) / 2;

// resources
inline std::string ICON_IMAGE = "data/wheel.png";
inline std::string TTF = "data/Roboto-Regular.ttf";
inline std::string OCEAN_GRID = "data/oceangrid.png";
inline std::string RADAR_GRID = "data/radargrid.png";

// Colors
const sf::Color BCG_COLOR(0, 170, 255);
const sf::Color TEXT_COLOR(255, 255, 0);
const sf::Color TEXT_SHADOW_COLOR(170, 170, 0);

const sf::Color DECK_COLOR(255, 255, 0);
const sf::Color DECK_WOUNDED_COLOR(255, 0, 0);
const sf::Color DECK_KILLED_COLOR(129, 129, 129);

const sf::Color HALO_COLOR(150, 150, 150);

// Deck
constexpr float DECK_SIZE = 40;
constexpr float DECK_SPACE = 41;

enum class DeckState
{
	Normal,
	Wounded,
	Killed
};

// ships
enum class TypeShip
{
	OneDeck = 1,
	TwoDeck,
	ThreeDeck,
	FourDeck
};

//grid
constexpr int CELL_WIDTH = 12;
constexpr int CELL_HEIGHT = 12;

constexpr char EMPTY_CELL = 'E';
constexpr char SHIP_CELL = 'S';
constexpr char HALO_CELL = 'H';
constexpr char WOUNDED_CELL = 'W';
constexpr char KILLED_CELL = 'K';
constexpr char SHOOT_CELL = 'X';	// ?