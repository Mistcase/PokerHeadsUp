#pragma once
#ifndef _TEXTBOX_FILE_INCLUDED_
#define _TEXTBOX_FILE_INCLUDED_

#include "Types.h"
#include "Fonts.h"

class TextBox : public sf::Drawable
{
public:
	TextBox() = default;
	TextBox(const String& defaultText, const Color& color);
	TextBox(const Vector2f& position, const Vector2f& size, const Color& fillColor, const Color& outlineColor, float outlineThicness,
			const String& defaultString = "", ApplicationFonts::FontType fontType = ApplicationFonts::ARIAL, const Color& textColor = Color::White,
			unsigned int characterSize = 13.f);

	/*void setPosition(const Vector2f& position);
	void setSize(const Vector2f& size);
	void setBackgroundColor(const Color& backColor);
	void setOutlineColor(const Color& outlineColor);
	void setTextColor(const Color& textColor);
	void setOutlineThickness(int value);
	void setFont(ApplicationFonts::FontType fontType);
	void setText(const String& str);
	void setCharacterSize(unsigned int value);*/


	const String& getText() const;
	const Vector2f& getPosition() const;
	bool selected() const;
	bool empty() const;

	//Update loop
	void update(sf::Vector2f mousePos, float deltaTime);
	void updateEvent(sf::Event &ev);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RectangleShape shape;
	sf::Text text, defaultText, cursor;
    Color backColor;
	String displayedString;
	bool isSelected = false, vCursor = false;

private:
	static const int BACKSPACE_TEXT_UNICODE_VALUE = 0x08;
};
#endif
