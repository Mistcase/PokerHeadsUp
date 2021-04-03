#pragma once
#ifndef _MAIN_MENU_STATE_INCLDUED_
#define _MAIN_MENU_STATE_INCLDUED_

#include "GameState.h"

#include "Button.h"
#include "TextBox.h"

class MainMenuState : public State
{
public:
	static sf::Color BACKGROUND_COLOR;

public:
	MainMenuState(StatesStack* statesStack);
	~MainMenuState();

	void update(float deltaTime, sf::Vector2f mousePos) override;
	void updateSfmlEvent(sf::Event& ev) override;

private:
	void initGui();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Button buttonProrotype;
	Button* btnConnect, btnCreateGame;
	TextBox loginBox;
};

#endif _MAIN_MENU_STATE_INCLDUED_