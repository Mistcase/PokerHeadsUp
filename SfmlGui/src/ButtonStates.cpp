#include "ButtonStates.h"

sfml_gui::ButtonStateContext::~ButtonStateContext()
{
	if (state != nullptr)
		delete state;
}

void sfml_gui::ButtonStateContext::setState(ButtonState * state)
{
	delete this->state;
	this->state = state;
}

sfml_gui::ButtonStateId sfml_gui::ButtonState::getStateId()
{
	return stateId;
}

sfml_gui::ButtonStateId sfml_gui::ButtonStateContext::update(const sf::Vector2f & mousePos, bool hoverButton, bool pressed)
{
	state->update(this, hoverButton, pressed);
	return state->getStateId();
}

sfml_gui::IdleButtonState::IdleButtonState()
{
	stateId = BTN_IDLE;
}

void sfml_gui::IdleButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
{
	if (hoverButton)
	{
		context->setState(new HoverButtonState());
	}
}

sfml_gui::HoverButtonState::HoverButtonState()
{
	stateId = BTN_HOVER;
}

void sfml_gui::HoverButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
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

sfml_gui::PressedButtonState::PressedButtonState()
{
	stateId = BTN_PRESSED;
}

void sfml_gui::PressedButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
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

sfml_gui::ReleasedButtonState::ReleasedButtonState()
{
	stateId = BTN_RELEASED;
}

void sfml_gui::ReleasedButtonState::update(ButtonStateContext * context, bool hoverButton, bool pressed)
{
	context->setState(new IdleButtonState());
}
