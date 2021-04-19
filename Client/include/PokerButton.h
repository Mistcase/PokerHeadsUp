#pragma once
#ifndef _POKER_BUTTON_H_
#define _POKER_BUTTON_H_

#include <Types.h>
#include "Settings.h"
#include "TableSlots.h"



class PokerButton : public Drawable
{
public:
	PokerButton();
	bool loadFromFile(const String& filePath);
	void setTableSlot(table_slots::Value tableSlot);

protected:
	void draw(RenderTarget& target, RenderStates states) const override;

private:
	//sf::CircleShape shape;
	Texture texture;
	RectangleShape shape;

private:
	const float SHAPE_RADIUS = 20.f;
	const Vector2f BUTTON_POSITIONS[table_slots::TABLE_SLOTS_COUNT] =
	{
		Vector2f(APPLICATION_WINDOW_SIZE.x / 2 - SHAPE_RADIUS / 2, 180),
		Vector2f(APPLICATION_WINDOW_SIZE.x / 2 - SHAPE_RADIUS / 2, 390)
	};
};

#endif