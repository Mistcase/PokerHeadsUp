#include "TextBox.h"

sfml_gui::TextBox::TextBox(const String & defaultText, const Color & color)
{
	this->defaultText.setString(defaultText);
	this->defaultText.setFillColor(color);
}

sfml_gui::TextBox::TextBox(const Vector2f & position, const Vector2f & size, const Color & fillColor, const Color & outlineColor, float outlineThicness,
	const String& defaultString, ApplicationFonts::FontType fontType, const Color & textColor, unsigned int characterSize)
{
	shape.setPosition(position);
	shape.setSize(size);
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThicness);

	text.setFont(ApplicationFonts::getFont(fontType));
	text.setFillColor(textColor);
	text.setCharacterSize(characterSize);
	text.setPosition(shape.getPosition() + Vector2f(1.5f, 0.f));

	cursor.setFont(ApplicationFonts::getFont(fontType));
	cursor.setFillColor(textColor);
	cursor.setCharacterSize(characterSize);
	cursor.setString(L"_");
	cursor.setPosition(text.getPosition() + Vector2f(text.getGlobalBounds().width, 0));

	defaultText.setFont(ApplicationFonts::getFont(fontType));
	defaultText.setFillColor(Color::White);
	defaultText.setCharacterSize(characterSize);
	defaultText.setString(defaultString);
	defaultText.setPosition(shape.getPosition() + Vector2f(1.5f, 0.f));
}

const sf::String & sfml_gui::TextBox::getText() const
{
	return displayedString;
}

const sf::Vector2f & sfml_gui::TextBox::getPosition() const
{
	return shape.getPosition();
}

void sfml_gui::TextBox::update(sf::Vector2f mousePos, float deltaTime)
{
	static float time = 0.f;
	time += deltaTime;

	if ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
		isSelected = shape.getGlobalBounds().contains(mousePos);

	if (isSelected && time >= 500.f)
	{
		vCursor = !vCursor;
		if (vCursor)
			cursor.setPosition(text.getPosition() + Vector2f(text.getGlobalBounds().width, -2.f)); //-2.f -> local value
		time = 0.f;
	}
	else if (!isSelected)
		vCursor = false;
}

void sfml_gui::TextBox::updateEvent(sf::Event & ev)
{
	if (!isSelected || ev.type != sf::Event::TextEntered)
		return;

	if (ev.text.unicode != BACKSPACE_TEXT_UNICODE_VALUE)
	{
		displayedString += ev.text.unicode;
		text.setString(displayedString);
	}
	else
	{
		if (!displayedString.isEmpty())
			displayedString.erase(displayedString.getSize() - 1);

		text.setString(displayedString);
	}
	//If this is not done, the new text will appear on the cursor
	vCursor = false;
}

void sfml_gui::TextBox::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!active)
		return;

	const sf::Text& drawableText = text.getString().isEmpty() && !isSelected ? defaultText : text;

	target.draw(shape, states);
	target.draw(drawableText, states);
	if (vCursor)
		target.draw(cursor);
}


bool sfml_gui::TextBox::selected() const
{
	return isSelected;
}

bool sfml_gui::TextBox::empty() const
{
	return displayedString.isEmpty();
}
