#include "PokerButton.h"

PokerButton::PokerButton()
{
	// shape.setFillColor(SHAPE_COLOR);
	// shape.setRadius(SHAPE_RADIUS);
}

bool PokerButton::loadFromFile(const String& filePath)
{
	if (!texture.loadFromFile(filePath))
		return false;
	shape.setTexture(&texture);
	shape.setSize(Vector2f(40, 34));

	return true;
}

void PokerButton::setTableSlot(table_slots::Value tableSlot)
{
	shape.setPosition(BUTTON_POSITIONS[tableSlot]);
}

void PokerButton::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(shape, states);
}