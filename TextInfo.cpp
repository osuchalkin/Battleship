#include "TextInfo.h"
#include "Constants.h"


TextInfo::TextInfo()
	: mFont(),
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
	mInfoText.setFillColor(TEXT_COLOR);
	mInfoText.setString("I - info text\tN - new game\tS - sound on / off\tQ - quit");
	//mInfoText.setStyle(sf::Text::Bold);
	mInfoText.setPosition((SCREEN_WIDTH - mInfoText.getLocalBounds().width) / 2, SCREEN_HEIGHT - Y_MARGIN / 2);
	mInfoText.setOutlineColor(TEXT_SHADOW_COLOR);
	mInfoText.setOutlineThickness(1);
}

void TextInfo::setWinnerText(const std::string& winner)
{
	std::string winnerCopy = winner;
	winnerCopy[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(winnerCopy[0])));

	mInfoText.setString(winnerCopy + " wins! Press N to start a new game.");
	mInfoText.setFillColor(DECK_WOUNDED_COLOR);
	mInfoText.setPosition((SCREEN_WIDTH - mInfoText.getLocalBounds().width) / 2, SCREEN_HEIGHT - Y_MARGIN / 2);
}

void TextInfo::render(sf::RenderWindow& window)
{
	window.draw(mTitle);
	window.draw(mInfoText);
}