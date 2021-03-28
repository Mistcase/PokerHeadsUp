#pragma once
#ifndef _BUTTON_INCLUDED_
#define _BUTTON_INCLUDED_

#include <SFML/Graphics.hpp>

enum ButtonState : int
{
	BTN_IDLE,
	BTN_HOVER,
	BTN_PRESSED,
	BTN_RELEASED,
};

class Button : public sf::Drawable
{
public:
	//Initialization
	Button() = default;
	Button(sf::Vector2f position, sf::Vector2f size, const wchar_t* string, sf::Color idleColor, sf::Color hoverColor, sf::Color pressedColor, sf::Font* basicalFont, sf::Color textColor = sf::Color::White, int characterSize = 12);
	~Button();

	void loadFont(std::string fileName);

	//Functions
	void setFraming(float boundSize, sf::Color boundColor);
	void setPointColor(size_t pointIndex, sf::Color newColor);
    void setPosition(sf::Vector2f position);
    void setText(const wchar_t* newString);
    
	void update(const sf::Vector2f mousePos);
	void render(sf::RenderTarget* target);

	void reset();

	//Access
	const std::wstring getString();
	const sf::FloatRect getGlobalBounds();
	const bool isReleased() const;

	//Open data
	bool active = true;
private:
	//Functions
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//Data
	ButtonState buttonState;

	sf::RectangleShape buttonShape;
	sf::Color colors[3];
	sf::Text displayedString;
	sf::Font font;

	sf::VertexArray extShape;
};

#endif
