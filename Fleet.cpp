#include "Fleet.h"

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
