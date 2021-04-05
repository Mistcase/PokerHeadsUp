#pragma once
#ifndef _MESSAGE_BOX_INCLUDED_
#define _MESSAGE_BOX_INCLUDED_

#include "Settings.h"
#include "Types.h"
#include "Button.h"
#include "Fonts.h"

#include "Observer.h"

#include <iostream>

class SfmlMessageBox : public Observer
{
public:
	SfmlMessageBox(const String& message, const String& title = "Message");

	bool show();

	void handleEvent(const EventMessage& message) override;

private:
	RenderWindow window;
	Vector2u windowSize;
	String title, message;
	sf::Text g_message;

	Button button;
};

#endif