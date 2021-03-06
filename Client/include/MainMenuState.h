#pragma once
#ifndef _MAIN_MENU_STATE_INCLDUED_
#define _MAIN_MENU_STATE_INCLDUED_

#include "GameState.h"
#include "Observer.h"

#include <Button.h>
#include <TextBox.h>

class MainMenuState : public State, public Observer
{
public:
	static sf::Color BACKGROUND_COLOR;

public:
	MainMenuState(StatesStack* statesStack);
	~MainMenuState() override;

	void update(float deltaTime, const Vector2f& mousePos) override;
	void updateSfmlEvent(sf::Event& ev) override;

	void handleEvent(const EventMessage& message) override;

private:
	void initGui();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Button buttonProrotype;
	Button btnConnect;
	TextBox loginBox;
};

#endif