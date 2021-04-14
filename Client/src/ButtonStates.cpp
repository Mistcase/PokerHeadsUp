#include "ButtonStates.h"

ButtonStateContext::~ButtonStateContext()
{
	if (state != nullptr)
		delete state;
}

void ButtonStateContext::setState(ButtonState * state)
{
	delete this->state;
	this->state = state;
}

ButtonStateId ButtonState::getStateId()
{
	return stateId;
}

ButtonStateId ButtonStateContext::update(const Vector2f & mousePos, bool hoverButton, bool pressed)
{
	state->update(this, hoverButton, pressed);
	return state->getStateId();
}

IdleButtonState::IdleButtonState()
{
	stateId = BTN_IDLE;
}

void IdleButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
{
	if (hoverButton)
	{
		context->setState(new HoverButtonState());
	}
}

HoverButtonState::HoverButtonState()
{
	stateId = BTN_HOVER;
}

void HoverButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
{
	if (hoverButton)
	{
		if (pressed)
		{
			context->setState(new PressedButtonState());
		}
	}
	else
	{
		context->setState(new IdleButtonState());
	}
}

PressedButtonState::PressedButtonState()
{
	stateId = BTN_PRESSED;
}

void PressedButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
{
	if (!pressed)
	{
		if (hoverButton)
		{
			context->setState(new ReleasedButtonState());
		}
		else
		{
			context->setState(new IdleButtonState());
		}
	}
}

ReleasedButtonState::ReleasedButtonState()
{
	stateId = BTN_RELEASED;
}

void ReleasedButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
{
	context->setState(new IdleButtonState());
}
