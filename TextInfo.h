#pragma once
#include <SFML/Graphics.hpp>

class TextInfo
{
public:
	TextInfo();
	//void setFont(const sf::Font& font);
	//void setPosition(float x, float y);
	void setWinnerText(const std::string& winner);
	void render(sf::RenderWindow& window);
private:
	sf::Font mFont;
	sf::Text mTitle;
	sf::Text mInfoText;
private:
	void setTitle();
	void setInfoText();
};

