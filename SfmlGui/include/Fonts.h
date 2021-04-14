#pragma once
#ifndef _GAME_FONT_INCLDUED_
#define _GAME_FONT_INCLDUED_

#include "Resources.h"

#include <SFML/Graphics.hpp>

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
	static const std::string FONTS_NAMES[AVAILABLE_FONTS_COUNT];
};

#endif