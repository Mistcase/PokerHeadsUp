#include "GameFont.h"

sf::Font* ApplicationFont::GetDefaultFont()
{
    static bool flag = false;

    if (!flag)
    {
        //Loading
        defaultFont.loadFromFile("Arial.ttf");
    }

    return &defaultFont;
}

sf::Font ApplicationFont::defaultFont;