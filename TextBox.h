#pragma once
#ifndef _TEXTBOX_FILE_INCLUDED_
#define _TEXTBOX_FILE_INCLUDED_

#include <SFML/Graphics.hpp>

class TextBox : public sf::Drawable
{
public:
	TextBox() = default;
	TextBox(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor, sf::Color outlineColor, float outlineThicness,
            sf::Font* font, sf::Color textColor = sf::Color::White, unsigned int characterSize = 13.f);

    void setStartText(std::string text, sf::Color color);
	void setSelection(bool value);

	void setText(const std::wstring& str);
	std::wstring getText();

	sf::Vector2f getPosition();

	void update(sf::Vector2f mousePos, float deltaTime);
	void updateEvent(sf::Event &ev);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool selected();
	bool empty();

private:
	sf::RectangleShape shape;
	sf::Text text, cursor;
    sf::Color realColor;
	sf::String str;
	bool isSelected = false, vCursor = false, noUsed = true;
};
#endif
