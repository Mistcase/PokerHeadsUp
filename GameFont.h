#pragma once
#ifndef _GAME_FONT_INCLDUED_
#define _GAME_FONT_INCLDUED_

#include <SFML/Graphics.hpp>

class ApplicationFont
{
public:
	static sf::Font* GetDefaultFont();

private:
	static sf::Font defaultFont;
};

#endif