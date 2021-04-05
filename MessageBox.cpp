#include "MessageBox.h"

SfmlMessageBox::SfmlMessageBox(const String& message, const String& title)
{
	if (message.getSize() > 64)
		return;

	this->message = message;
	this->title = title;

	g_message.setFont(ApplicationFonts::getFont(ApplicationFonts::ARIAL));
	g_message.setCharacterSize(14);
	g_message.setFillColor(Color::White);
	g_message.setString(message);

	auto messageBounds = g_message.getGlobalBounds();
	windowSize = Vector2u(messageBounds.width + 20, messageBounds.height + 20);

	if (windowSize.x < 170)
		windowSize.x = 170;
	if (windowSize.y)
		windowSize.y = 100;

	messageBounds = g_message.getGlobalBounds();

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
	button.setPosition(Vector2f(windowSize.x / 2 - buttonSize.x / 2, windowSize.y * 0.75));
	button.addObserver(this);

	sf::FloatRect dsRect = g_message.getLocalBounds();
	g_message.setOrigin(dsRect.left + dsRect.width / 2.f, dsRect.top + dsRect.height / 2.f);
	g_message.setPosition(windowSize.x / 2, windowSize.y / 2.2);
}

bool SfmlMessageBox::show()
{
	using namespace sf;
	if (message.isEmpty())
		return false;

	window.create(VideoMode(windowSize.x, windowSize.y), title, Style::Titlebar);

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
		window.draw(g_message);
		window.draw(button);
		window.display();
	}

	return true;
}

void SfmlMessageBox::handleEvent(const EventMessage& message)
{
	window.close();
}
