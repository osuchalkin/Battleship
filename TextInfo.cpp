#include "TextInfo.h"
#include "Constants.h"


TextInfo::TextInfo()
	: mShowInfo(false),
	mFont(),
	mTitle(),
	mInfoText()
{
	if (!mFont.loadFromFile(TTF))
		throw std::runtime_error("Failed to load ttf-file from data.");

	// Title
	setTitle();

	// Info text
	setInfoText();
}

void TextInfo::setTitle()
{
	mTitle.setFont(mFont);
	mTitle.setCharacterSize(60);
	mTitle.setFillColor(TEXT_COLOR);
	mTitle.setString("BATTLESHIP");
	mTitle.setStyle(sf::Text::Bold);
	float textHeight = mTitle.getLocalBounds().height;
	mTitle.setPosition((SCREEN_WIDTH - mTitle.getLocalBounds().width) / 2, Y_MARGIN / 2 - textHeight);
	mTitle.setOutlineColor(TEXT_SHADOW_COLOR);
	mTitle.setOutlineThickness(2);
}

void TextInfo::setInfoText()
{
	mInfoText.setFont(mFont);
	mInfoText.setCharacterSize(20);
	mInfoText.setFillColor(TEXT_INFO_COLOR);
	mInfoText.setString("I - info text\tN - new game\tS - sound on / off\tQ - quit");
	mInfoText.setPosition((SCREEN_WIDTH - mInfoText.getLocalBounds().width) / 2, SCREEN_HEIGHT - Y_MARGIN / 2);
}

void TextInfo::setWinnerText(const std::string& winner, int moves)
{
	std::string winnerCopy = winner;
	winnerCopy[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(winnerCopy[0])));

	std::string movesText = std::to_string(moves);
	mInfoText.setFillColor(TEXT_WINNER_COLOR);
	mInfoText.setString(winnerCopy + " won in " + movesText + " moves! Press N to start a new game.");
	mInfoText.setPosition((SCREEN_WIDTH - mInfoText.getLocalBounds().width) / 2, SCREEN_HEIGHT - Y_MARGIN / 2);
}

void TextInfo::render(sf::RenderWindow& window)
{
	window.draw(mTitle);
	window.draw(mInfoText);
	if (mShowInfo)
		showInfo(window);
}

void TextInfo::showInfo(sf::RenderWindow& window)
{
	sf::Text title("About BattleShip", mFont, 50);
	title.setStyle(sf::Text::Bold);
	float textMiddleWidth = title.getLocalBounds().width / 2;
	title.setPosition(SCREEN_WIDTH / 2 - textMiddleWidth, Y_MARGIN / 2);
	title.setFillColor(TEXT_COLOR);
	title.setOutlineColor(TEXT_SHADOW_COLOR);
	title.setOutlineThickness(2);

	std::vector<sf::Text> infoes;

	std::vector<std::string> messages = {
		"Sea Battle (or Battleship)",
		"is known worldwide as a pencil and paper game which dates from World War I.",
		"",
		"Maps created by Molly 'Cougarmint' Willits:",
		"https://www.deviantart.com/cougarmint",
		"",
		"Sounds are from https://zvukipro.com/1748-zvuki-vystrela-iz-korabelnyh-pushek.html",
		"",
		"Programmed in C++ with SFML library.",
		"",
		"© Oleh Suchalkin  2025"
	};

	for (size_t i = 0; i < messages.size(); ++i)
	{
		sf::Text text(messages[i], mFont, 20);
		float textMiddleWidth = text.getLocalBounds().width / 2;
		text.setPosition(SCREEN_WIDTH / 2 - textMiddleWidth, SCREEN_HEIGHT / 3 + i * 30);
		text.setFillColor(TEXT_INFO_COLOR);

		infoes.push_back(text);
	}

	bool finished = false;

	while (!finished)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				finished = true;
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				mShowInfo = false;
				finished = true;
			}	
		}

		window.clear(BCG_COLOR);
		window.draw(title);
		for (const auto& text : infoes)
			window.draw(text);
		window.display();
	}
}