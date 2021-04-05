#include "Button.h"

Button::Button()
{
	stateContext = shared_ptr<ButtonStateContext>(new ButtonStateContext());
}

Button::Button(const Button & prototype) : Button()
{
	colors[BTN_IDLE] = prototype.colors[BTN_IDLE];
	colors[BTN_HOVER] = prototype.colors[BTN_HOVER];
	colors[BTN_PRESSED] = prototype.colors[BTN_PRESSED];

	buttonShape.setSize(prototype.buttonShape.getSize());
	buttonShape.setFillColor(prototype.buttonShape.getFillColor());

	displayedString.setFont(*prototype.displayedString.getFont());
	displayedString.setCharacterSize(prototype.displayedString.getCharacterSize());
}

void Button::setText(const String & newText)
{
	displayedString.setString(newText);

	sf::FloatRect dsRect = displayedString.getLocalBounds();
	displayedString.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
}

void Button::setButtonColor(const Color & color, ButtonStateId btnState)
{
	colors[btnState] = color;
}

void Button::setTextColor(const Color & color)
{
	displayedString.setFillColor(color);
}

void Button::setFont(ApplicationFonts::FontType fontType)
{
	displayedString.setFont(ApplicationFonts::getFont(fontType));
}

void Button::setCharacterSize(int characterSize)
{
	displayedString.setCharacterSize(characterSize);
}

void Button::setFraming(float boundSize, Color boundColor)
{
	buttonShape.setOutlineThickness(boundSize);
	buttonShape.setOutlineColor(boundColor);
}

void Button::notifyObservers(const EventMessageString& message)
{
	for (auto& obs : observers)
		obs->handleEvent(EventMessage(this));
}

void Button::update(const Vector2f& mousePos)
{
	if (!active)
		return;

	auto stateId = stateContext->update(mousePos, buttonShape.getGlobalBounds().contains(mousePos), sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
	buttonShape.setFillColor(colors[stateId]);

	if (stateId == BTN_RELEASED)
		notifyObservers();
}

Vector2f Button::getSize() const
{
	return buttonShape.getSize();
}

const String & Button::getText() const
{
	return displayedString.getString();
}

void Button::setSize(const Vector2f & size)
{
	buttonShape.setSize(size);
}

void Button::setPosition(const Vector2f& position)
{
    sf::FloatRect dsRect = displayedString.getLocalBounds();
    displayedString.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
    buttonShape.setPosition(position);
    displayedString.setPosition(buttonShape.getPosition() + sf::Vector2f(buttonShape.getSize().x / 2.f, buttonShape.getSize().y / 2.f));
}

void Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!active)
		return;

	target.draw(buttonShape, states);
	target.draw(displayedString, states);
}