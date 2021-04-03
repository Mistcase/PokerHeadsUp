#include "TextBox.h"

TextBox::TextBox(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor, sf::Color outlineColor, float outlineThicness, sf::Font* font, sf::Color textColor, unsigned int characterSize)
{
	shape.setPosition(position);
	shape.setSize(size);
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThicness);

	text.setFont(*font);
	text.setFillColor(textColor);
	text.setCharacterSize(characterSize);
	text.setPosition(shape.getPosition() + sf::Vector2f(1.5f, 0.f));

	cursor.setFont(*font);
	cursor.setFillColor(textColor);
	cursor.setCharacterSize(characterSize);
	cursor.setString(L"_");
	cursor.setPosition(text.getPosition() + sf::Vector2f(text.getGlobalBounds().width, 0));

    realColor = textColor;
	//hd.highlightRectangle.setFillColor(sf::Color(0, 50, 120, 150));
}

void TextBox::setStartText(std::string startText, sf::Color color)
{
    text.setString(startText);
    text.setFillColor(color);
}

void TextBox::setSelection(bool value)
{
    if (noUsed)
        text.setString("");
    noUsed = false;
	isSelected = value;
    text.setFillColor(realColor);
}

void TextBox::update(sf::Vector2f mousePos, float deltaTime)
{
	static float time = 0.f;
	time += deltaTime;

	//UpdateCursor
	if ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
	{
        if (shape.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            setSelection(shape.getGlobalBounds().contains(mousePos) ? true : false);
            if (!vCursor)
            {
                cursor.setPosition(text.getPosition() + sf::Vector2f(text.getGlobalBounds().width, 0));
                vCursor = true;
            }
        }
        else
            isSelected = false;
	}
	if (isSelected && time >= 500.f)
	{
		vCursor = !vCursor;
		if (vCursor)
			cursor.setPosition(text.getPosition() + sf::Vector2f(text.getGlobalBounds().width, -2.f)); //-2.f -> local value
		time = 0.f;
	}
}

void TextBox::updateEvent(sf::Event & ev)
{
	if (isSelected && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
	{
		if (ev.type == sf::Event::TextEntered)
		{
			if (ev.text.unicode != 0x08 && ev.text.unicode != 0xD && ev.text.unicode != 96 /*&& text.getString().getSize() <= 64*/)
			{
				str += ev.text.unicode;
				text.setString(str);
			}
			else if (ev.text.unicode == 0x08)
			{
				if (!str.isEmpty())
				{
					str.erase(str.getSize() - 1);
				}
				text.setString(str);
			}
			vCursor = false;
		}
	}
	else if (ev.type == sf::Event::KeyReleased)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
		{
			std::wstring str = L"";
			if (ev.key.code == sf::Keyboard::Key::V)
			{
				str = sf::Clipboard::getString();
				text.setString(text.getString() + str);
			}
		}
	}
}

void TextBox::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(text, states);
	if (vCursor)
		target.draw(cursor);
}

void TextBox::setText(const std::wstring & str)
{
	text.setString(str);
}

std::wstring TextBox::getText()
{
	return noUsed ? L"" : text.getString();
}

sf::Vector2f TextBox::getPosition()
{
	return shape.getPosition();
}

bool TextBox::selected()
{
	return isSelected;
}

bool TextBox::empty()
{
	return str.isEmpty();
}
