#include "TextBox.h"

TextBox::TextBox(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor, sf::Color outlineColor, float outlineThicness, sf::Font* font, sf::Color textColor, unsigned int characterSize)
{
	this->shape.setPosition(position);
	this->shape.setSize(size);
	this->shape.setFillColor(fillColor);
	this->shape.setOutlineColor(outlineColor);
	this->shape.setOutlineThickness(outlineThicness);

	this->text.setFont(*font);
	this->text.setFillColor(textColor);
	this->text.setCharacterSize(characterSize);
	this->text.setPosition(this->shape.getPosition() + sf::Vector2f(1.5f, 0.f));

	this->cursor.setFont(*font);
	this->cursor.setFillColor(textColor);
	this->cursor.setCharacterSize(characterSize);
	this->cursor.setString(L"_");
	this->cursor.setPosition(this->text.getPosition() + sf::Vector2f(this->text.getGlobalBounds().width, 0));

    realColor = textColor;
	//this->hd.highlightRectangle.setFillColor(sf::Color(0, 50, 120, 150));
}

void TextBox::setStartText(std::string text, sf::Color color)
{
    this->text.setString(text);
    this->text.setFillColor(color);
}

void TextBox::setSelection(bool value)
{
    if (noUsed)
        this->text.setString("");
    this->noUsed = false;
	this->isSelected = value;
    this->text.setFillColor(realColor);
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
            setSelection(this->shape.getGlobalBounds().contains(mousePos) ? true : false);
            if (!vCursor)
            {
                this->cursor.setPosition(this->text.getPosition() + sf::Vector2f(this->text.getGlobalBounds().width, 0));
                this->vCursor = true;
            }
        }
        else
            isSelected = false;
	}
	if (this->isSelected && time >= 500.f)
	{
		vCursor = !vCursor;
		if (vCursor)
			this->cursor.setPosition(this->text.getPosition() + sf::Vector2f(this->text.getGlobalBounds().width, -2.f)); //-2.f -> local value
		time = 0.f;
	}
}

void TextBox::updateEvent(sf::Event & ev)
{
	if (this->isSelected && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
	{
		if (ev.type == sf::Event::TextEntered)
		{
			if (ev.text.unicode != 0x08 && ev.text.unicode != 0xD && ev.text.unicode != 96 /*&& this->text.getString().getSize() <= 64*/)
			{
				str += ev.text.unicode;
				this->text.setString(str);
			}
			else if (ev.text.unicode == 0x08)
			{
				if (!str.isEmpty())
				{
					str.erase(str.getSize() - 1);
				}
				this->text.setString(str);
			}
			this->vCursor = false;
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
				this->text.setString(this->text.getString() + str);
			}
		}
	}
}

void TextBox::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(this->shape, states);
	target.draw(this->text, states);
	if (this->vCursor)
		target.draw(this->cursor);
}

void TextBox::setText(const std::wstring & str)
{
	this->text.setString(str);
}

std::wstring TextBox::getText()
{
	return noUsed ? L"" : this->text.getString();
}

sf::Vector2f TextBox::getPosition()
{
	return this->shape.getPosition();
}

bool TextBox::selected()
{
	return this->isSelected;
}

bool TextBox::empty()
{
	return str.isEmpty();
}
