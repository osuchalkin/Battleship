#pragma once
#include <SFML/Graphics.hpp>

class TextInfo
{
public:
	TextInfo();
	void setWinnerText(const std::string& winner, int moves);
	void render(sf::RenderWindow& window);
	void showInfo(sf::RenderWindow& window);
public:
	bool mShowInfo;
private:
	sf::Font mFont;
	sf::Text mTitle;
	sf::Text mInfoText;
private:
	void setTitle();
	void setInfoText();
};

