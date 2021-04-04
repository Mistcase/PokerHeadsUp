#include "PokerButton.h"

PokerButton::PokerButton()
{
	shape.setFillColor(SHAPE_COLOR);
	shape.setRadius(SHAPE_RADIUS);
}

void PokerButton::setTableSlot(table_slots::Value tableSlot)
{
	shape.setPosition(BUTTON_POSITIONS[tableSlot]);
}

void PokerButton::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(shape, states);
}