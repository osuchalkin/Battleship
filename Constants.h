#pragma once

#include<SFML/Graphics.hpp>


constexpr float PI = 3.14159265358979323846f;

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

//sounds
inline std::string SHOT_SOUND = "data/past.wav";
inline std::string WOUNDED_SOUND = "data/wounded.wav";
inline std::string SUNK_SOUND = "data/killed.wav";
inline std::string SET_DECK_SOUND = "data/gobble.wav";

// Colors
const sf::Color BCG_COLOR(0, 51, 78);
const sf::Color TEXT_COLOR(77, 210, 255);
const sf::Color TEXT_SHADOW_COLOR(0, 40, 60);
const sf::Color TEXT_INFO_COLOR(136, 221, 238);
const sf::Color TEXT_WINNER_COLOR(255, 112, 67);

const sf::Color DECK_COLOR(76, 110, 245);
const sf::Color DECK_WOUNDED_COLOR(230, 57, 70);
const sf::Color DECK_KILLED_COLOR(109, 7, 26);
const sf::Color KILLED_MARK_COLOR(255, 204, 213);

const sf::Color HALO_COLOR(58, 80, 107);

// Deck
constexpr float DECK_SIZE = 40;
constexpr float DECK_SPACE = 41;

enum class DeckState
{
	Normal,
	Wounded
};
enum class ShipState
{
	Alive,
	Sunk
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
constexpr int TOTAL_DECKS_COMPUTER = 20;

constexpr char EMPTY_CELL = 'E';
constexpr char SHIP_CELL = 'S';
constexpr char HALO_CELL = 'H';
constexpr char WOUNDED_CELL = 'W';
constexpr char KILLED_CELL = 'K';
constexpr char SHOOT_CELL = 'X';

enum class SoundType
{
	SetDeck,
	Shot,
	Wounded,
	Sunk
};