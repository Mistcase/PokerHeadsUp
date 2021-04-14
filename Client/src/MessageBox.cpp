#include "MessageBox.h"

SfmlMessageBox::SfmlMessageBox(const String& message, const String& title)
{
	if (message.getSize() > MAX_MESSAGE_SIZE)
		return;

	for (size_t i = 0; i < message.getSize(); i++)
	{
		messageStrings[usedStrings - 1] += message[i];
		if (i / MAX_STRING_SIZE > (usedStrings - 1) && message[i] == ' ')
			usedStrings++;
	}
	this->title = title;

	float messageHeight = 0;
	for (size_t i = 0; i < usedStrings; i++)
	{
		g_messageStrings[i].setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
		g_messageStrings[i].setCharacterSize(14);
		g_messageStrings[i].setFillColor(Color::White);
		g_messageStrings[i].setString(messageStrings[i]);
		messageHeight += g_messageStrings[i].getGlobalBounds().height;
	}

	auto messageBounds = g_messageStrings[0].getGlobalBounds();
	messageBounds.height = messageHeight;

	windowSize = Vector2u(static_cast<unsigned int>(messageBounds.width + 20.f),
						  static_cast<unsigned int>(messageBounds.height + 20.f));

	if (windowSize.x < 170)
		windowSize.x = 170;
	if (windowSize.y)
		windowSize.y = 100;

	 

	Vector2f buttonSize = Vector2f(windowSize.x * 0.65f, windowSize.y * 0.2f);
	button.setSize(buttonSize);
	button.setButtonColor(Color(128, 128, 128), BTN_IDLE);
	button.setButtonColor(Color(165, 165, 165), BTN_HOVER);
	button.setButtonColor(Color(20, 20, 20, 200), BTN_PRESSED);
	button.setFraming(1, Color::White);
	button.setCharacterSize(14);
	button.setFont(ApplicationFonts::ARIAL);
	button.setTextColor(Color::White);
	button.setText("OK");
	button.setPosition(Vector2f(windowSize.x / 2.f - buttonSize.x / 2, windowSize.y * 0.75f));
	button.addObserver(this);

	for (size_t i = 0; i < usedStrings; i++)
	{
		sf::FloatRect dsRect = g_messageStrings[i].getLocalBounds();
		g_messageStrings[i].setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
		g_messageStrings[i].setPosition(windowSize.x / 2.f, windowSize.y / 2.2f - messageHeight);
	}
}

void SfmlMessageBox::show()
{
	using namespace sf;
	if (messageStrings[0].isEmpty())
		return;

	window.create(VideoMode(windowSize.x, windowSize.y), title, Style::Close);

	while (window.isOpen())
	{
		Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				window.close();
		}

		button.update(static_cast<sf::Vector2f>(window.mapPixelToCoords(sf::Mouse::getPosition(window))));

		window.clear(Color(40, 40, 40));
		for (size_t i = 0; i < usedStrings; i++)
			window.draw(g_messageStrings[i]);
		window.draw(button);
		window.display();
	}
}

void SfmlMessageBox::handleEvent(const EventMessage& message)
{
	window.close();
}
