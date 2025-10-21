#include "Fleet.h"

#include <iostream>		// for debug

Fleet::Fleet() :
	mShips()
{
}


void Fleet::render(sf::RenderWindow& window)
{
	for (auto& ship : mShips)
	{
		ship->render(window);
	}
}

void Fleet::createShipForPreparing(TypeShip type, int beginX, int beginY)
{
	float x = X2_MARGIN + DECK_SPACE * beginX;
	float y = Y_MARGIN + DECK_SPACE * beginY;

	std::vector<sf::Vector2f> positions;
	std::vector<DeckState> states;
	for (int i = 1; i <= static_cast<int>(type); i++)
	{
		if (type == TypeShip::ThreeDeck)
			positions.push_back({ x, y + DECK_SPACE * i });
		else
			positions.push_back({ x + DECK_SPACE * i, y });

		states.push_back(DeckState::Normal);
	}
	auto ship = std::make_shared<Ship>(type, positions, states);
	addShip(ship);
}

void Fleet::createFleetForPreparing()
{
	createShipForPreparing(TypeShip::FourDeck, 2, 1);
	
	createShipForPreparing(TypeShip::ThreeDeck, 3, 3);
	createShipForPreparing(TypeShip::ThreeDeck, 6, 3);

	createShipForPreparing(TypeShip::TwoDeck, 0, 9);
	createShipForPreparing(TypeShip::TwoDeck, 3, 9);
	createShipForPreparing(TypeShip::TwoDeck, 6, 9);

	createShipForPreparing(TypeShip::OneDeck, 0, 1);
	createShipForPreparing(TypeShip::OneDeck, 7, 1);
	createShipForPreparing(TypeShip::OneDeck, 0, 5);
	createShipForPreparing(TypeShip::OneDeck, 7, 5);
}

void Fleet::removePreparedShip()
{
	if (!mShips.empty())
		mShips.erase(mShips.begin());
}

void Fleet::createShip(std::vector<sf::Vector2i>& positions)
{
	TypeShip type;
	switch (positions.size())
	{
	case 1:
		type = TypeShip::OneDeck;
		break;
	case 2:
		type = TypeShip::TwoDeck;
		break;
	case 3:
		type = TypeShip::ThreeDeck;
		break;
	case 4:
		type = TypeShip::FourDeck;
		break;
	default:
		return;
	}
	std::vector<sf::Vector2f> pos_float;
	std::vector<DeckState> states;
	for (const auto& pos : positions)
	{
		pos_float.push_back({ static_cast<float>(pos.x), static_cast<float>(pos.y) });
		states.push_back(DeckState::Normal);
	}
	auto ship = std::make_shared<Ship>(type, pos_float, states);
	addShip(ship);
}

TypeShip Fleet::getTypeShip(sf::Vector2i position)
{
	for (const auto& ship : mShips)
	{
		auto shipPositions = ship->getPositions();
		for (const auto& pos : shipPositions)
		{
			if (static_cast<int>(pos.x) == position.x && static_cast<int>(pos.y) == position.y)
			{
				return ship->getType(); 
			}
		}
	}
	return TypeShip::OneDeck; // Default return value if no ship found at position - ??
}