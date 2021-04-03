#pragma once
#ifndef _GAME_FONT_INCLDUED_
#define _GAME_FONT_INCLDUED_

#include <SFML/Graphics.hpp>
#include <string>

using std::string;

class ApplicationFonts
{
public:
	enum FontType
	{
		ARIAL,
		AVAILABLE_FONTS_COUNT
	};

public:
	static const sf::Font& getFont(FontType fontType);

private:
	static sf::Font* fonts[AVAILABLE_FONTS_COUNT];
	static const string FONTS_NAMES[AVAILABLE_FONTS_COUNT];
};

#endif