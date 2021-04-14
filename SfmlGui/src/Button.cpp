#include "Button.h"

sfml_gui::Button::Button()
{
	stateContext = std::shared_ptr<ButtonStateContext>(new ButtonStateContext());
}

sfml_gui::Button::Button(const Button & prototype) : Button()
{
	colors[BTN_IDLE] = prototype.colors[BTN_IDLE];
	colors[BTN_HOVER] = prototype.colors[BTN_HOVER];
	colors[BTN_PRESSED] = prototype.colors[BTN_PRESSED];

	buttonShape.setSize(prototype.buttonShape.getSize());
	buttonShape.setFillColor(prototype.buttonShape.getFillColor());

	displayedString.setFont(*prototype.displayedString.getFont());
	displayedString.setCharacterSize(prototype.displayedString.getCharacterSize());
}

void sfml_gui::Button::setText(const String & newText)
{
	displayedString.setString(newText);

	sf::FloatRect dsRect = displayedString.getLocalBounds();
	displayedString.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
}

void sfml_gui::Button::setButtonColor(const Color & color, ButtonStateId btnState)
{
	colors[btnState] = color;
}

void sfml_gui::Button::setTextColor(const Color & color)
{
	displayedString.setFillColor(color);
}

void sfml_gui::Button::setFont(ApplicationFonts::FontType fontType)
{
	displayedString.setFont(ApplicationFonts::getFont(fontType));
}

void sfml_gui::Button::setCharacterSize(int characterSize)
{
	displayedString.setCharacterSize(characterSize);
}

void sfml_gui::Button::setFraming(float boundSize, Color boundColor)
{
	buttonShape.setOutlineThickness(boundSize);
	buttonShape.setOutlineColor(boundColor);
}

void sfml_gui::Button::notifyObservers(const EventMessageString& message)
{
	for (auto& obs : observers)
		obs->handleEvent(EventMessage(this));
}

void sfml_gui::Button::update(const Vector2f& mousePos)
{
	if (!active)
		return;

	auto stateId = stateContext->update(mousePos, buttonShape.getGlobalBounds().contains(mousePos), sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
	buttonShape.setFillColor(colors[stateId]);

	if (stateId == BTN_RELEASED)
		notifyObservers();
}

const sf::String & sfml_gui::Button::getText() const
{
	return displayedString.getString();
}

void sfml_gui::Button::setSize(const Vector2f & size)
{
	buttonShape.setSize(size);
}

void sfml_gui::Button::setPosition(const Vector2f& position)
{
    sf::FloatRect dsRect = displayedString.getLocalBounds();
    displayedString.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
    buttonShape.setPosition(position);
    displayedString.setPosition(buttonShape.getPosition() + sf::Vector2f(buttonShape.getSize().x / 2.f, buttonShape.getSize().y / 2.f));
}

void sfml_gui::Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!active)
		return;

	target.draw(buttonShape, states);
	target.draw(displayedString, states);
}