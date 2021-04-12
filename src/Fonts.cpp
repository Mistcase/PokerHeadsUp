#include "Fonts.h"

const sf::Font& ApplicationFonts::getFont(FontType fontType)
{
	if (fonts[fontType] == nullptr)
	{
		fonts[fontType] = new sf::Font();
		fonts[fontType]->loadFromFile(Resources::path(FONTS_NAMES[fontType]));
	}

	return *fonts[fontType];
}

sf::Font* ApplicationFonts::fonts[AVAILABLE_FONTS_COUNT];

const string ApplicationFonts::FONTS_NAMES[AVAILABLE_FONTS_COUNT] =
{
	"Arial.ttf",
};
