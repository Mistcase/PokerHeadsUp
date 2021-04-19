#pragma once
#ifndef _MESSAGE_BOX_INCLUDED_
#define _MESSAGE_BOX_INCLUDED_

#include "Settings.h"
#include "Types.h"
#include "Button.h"
#include "Fonts.h"

#include <Observer.h>

#include <iostream>

class SfmlMessageBox : public Observer
{
private:
	static const int MAX_STRINGS_COUNT = 3;
	static const int MAX_STRING_SIZE = 64;
	static const int MAX_MESSAGE_SIZE = MAX_STRINGS_COUNT * MAX_STRING_SIZE;

public:
	SfmlMessageBox(const String& message, const String& title = "Message");

	void show();

	void handleEvent(const EventMessage& message) override;

private:
	RenderWindow window;
	Vector2u windowSize;
	String title, messageStrings[MAX_STRINGS_COUNT];
	sf::Text g_messageStrings[MAX_STRINGS_COUNT];
	size_t usedStrings = 1;

	Button button;
};

#endif