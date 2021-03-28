#include "Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, const wchar_t* string, sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor, sf::Font* basicalFont, sf::Color textColor, int characterSize)
{
	this->displayedString.setFont(*basicalFont);
	this->displayedString.setFillColor(textColor);
	this->displayedString.setCharacterSize(characterSize);
	this->displayedString.setString(string);

	sf::FloatRect dsRect = this->displayedString.getLocalBounds();
	this->displayedString.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);


	this->colors[BTN_IDLE] = idleColor;
	this->colors[BTN_HOVER] = hoverColor;
	this->colors[BTN_PRESSED] = pressedColor;

	this->buttonShape.setPosition(position);
	this->buttonShape.setSize(size);
	this->buttonShape.setFillColor(idleColor);

	this->displayedString.setPosition(this->buttonShape.getPosition() + sf::Vector2f(this->buttonShape.getSize().x / 2.f, this->buttonShape.getSize().y / 2.f));

	this->buttonState = ButtonState::BTN_IDLE;

	/*this->extShape = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	this->extShape[0] = this->buttonShape.getPosition();
	this->extShape[1] = this->buttonShape.getPosition() + sf::Vector2f(this->buttonShape.getSize().x, 0);
	this->extShape[2] = this->buttonShape.getPosition() + this->buttonShape.getSize();
	this->extShape[3] = this->buttonShape.getPosition() + sf::Vector2f(0, this->buttonShape.getSize().y);*/

	//for (size_t i = 0; i < 4; i++)
		//this->extShape[i].color = sf::Color::Transparent;
}

Button::~Button()
{
}

void Button::loadFont(std::string fileName)
{
	this->font.loadFromFile(fileName);
	this->displayedString.setFont(this->font);
}

void Button::setFraming(float boundSize, sf::Color boundColor)
{
	this->buttonShape.setOutlineThickness(boundSize);
	this->buttonShape.setOutlineColor(boundColor);
}

void Button::setPointColor(size_t pointIndex, sf::Color newColor)
{
	this->extShape[pointIndex].color = newColor;
}

void Button::update(const sf::Vector2f mousePos)
{
	if (active)
	{
		bool contains = this->buttonShape.getGlobalBounds().contains(mousePos), pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		if (this->buttonState == BTN_PRESSED)
		{
			if (!pressed)
				this->buttonState = contains ? BTN_RELEASED : BTN_IDLE;
		}
		else
		{
			this->buttonState = contains ? (pressed ? BTN_PRESSED : BTN_HOVER) : BTN_IDLE;
		}

		if (this->buttonShape.getFillColor() != this->colors[this->buttonState])
			this->buttonShape.setFillColor(this->colors[this->buttonState]);
	}
}

void Button::setPosition(sf::Vector2f position)
{
    sf::FloatRect dsRect = this->displayedString.getLocalBounds();
    this->displayedString.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
    this->buttonShape.setPosition(position);
    this->displayedString.setPosition(this->buttonShape.getPosition() + sf::Vector2f(this->buttonShape.getSize().x / 2.f, this->buttonShape.getSize().y / 2.f));
}

void Button::setText(const wchar_t* newString)
{
    this->displayedString.setString(newString);
}

void Button::render(sf::RenderTarget * target)
{
	if (active)
	{
		target->draw(this->buttonShape);
		target->draw(this->extShape);
		target->draw(this->displayedString);
	}
}

void Button::reset()
{
	this->buttonState = BTN_IDLE;
}

void Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (active)
	{
		target.draw(this->buttonShape, states);
		target.draw(this->displayedString, states);
	}
}

const std::wstring Button::getString()
{
	std::wstring str = this->displayedString.getString();
	return str;
}

const sf::FloatRect Button::getGlobalBounds()
{
	return this->buttonShape.getGlobalBounds();
}

const bool Button::isReleased() const
{
	return this->buttonState == BTN_RELEASED;
}
