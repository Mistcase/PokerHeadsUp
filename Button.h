#pragma once
#ifndef _BUTTON_INCLUDED_
#define _BUTTON_INCLUDED_

#include "Types.h"
#include "Fonts.h"
#include "ButtonStates.h"
#include "Observer.h"

class Button : public Drawable, public Observerable
{
public:
	//Initialization
	Button() = default;
	Button(const Button& prototype);

	//Functional
	void setSize(const Vector2f& size);
	void setPosition(const Vector2f& position);
	void setText(const String& newString);
	void setButtonColor(const Color& color, ButtonStateId btnState);
	void setTextColor(const Color& color);
	void setFont(ApplicationFonts::FontType fontType);
	void setCharacterSize(int characterSize);
	void setFraming(float boundSize, sf::Color boundColor);

	//Observers
	void notifyObservers(const EventMessageString& message = "") override;

	//Loop functions
	void update(const Vector2f& mousePos);

	//Access
	const String& getText() const;

	//Open data
	bool active = true;
private:
	//Functions
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//Data
	std::shared_ptr<ButtonStateContext> stateContext;

	sf::RectangleShape buttonShape;
	sf::Color colors[3];
	sf::Text displayedString;
};

#endif
