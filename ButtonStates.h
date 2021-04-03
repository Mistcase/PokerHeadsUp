#pragma once
#ifndef _BUTTONS_STATE_H
#define _BUTTONS_STATE_H

#include "Types.h"

enum ButtonStateId : int
{
	BTN_IDLE,
	BTN_HOVER,
	BTN_PRESSED,
	BTN_RELEASED,
	BTN_STATES_COUNT,
	BTN_STATE_INVALID
};

class ButtonStateContext;

class ButtonState
{
public:
	virtual void update(ButtonStateContext* context, bool hoverButton, bool pressed) = 0;
	ButtonStateId getStateId();

protected:
	ButtonStateId stateId = BTN_STATE_INVALID;
};


class IdleButtonState : public ButtonState
{
public:
	IdleButtonState();
	void update(ButtonStateContext* context, bool hoverButton, bool pressed) override;
};


class HoverButtonState : public ButtonState
{
public:
	HoverButtonState();
	void update(ButtonStateContext* context, bool hoverButton, bool pressed) override;
};


class PressedButtonState : public ButtonState
{
public:
	PressedButtonState();
	void update(ButtonStateContext* context, bool hoverButton, bool pressed) override;
};


class ReleasedButtonState : public ButtonState
{
public:
	ReleasedButtonState();
	void update(ButtonStateContext* context, bool hoverButton, bool pressed) override;
};


class ButtonStateContext
{
public:
	ButtonStateContext() : state(new IdleButtonState()) {}
	~ButtonStateContext();

	void setState(ButtonState* state);
	ButtonStateId update(const Vector2f& mousePos, bool hoverButton, bool pressed);

private:
	ButtonState* state;
};

#endif