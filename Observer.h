#pragma once
#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "Types.h"

typedef std::string EventMessageString;

struct EventMessage
{
	EventMessage() = default;
	EventMessage(void* sender, const EventMessageString& args = "") : sender(sender), args(args) {}
	void* sender;
	EventMessageString args;
};

class Observer
{
public:
	virtual void handleEvent(const EventMessage& message) = 0;
};


class Observerable
{
public:
	void addObserver(Observer* obs);
	void removeObserver(Observer* obs);
	virtual void notifyObservers(const EventMessageString& message = "") = 0;

protected:
	vector<Observer*> observers;
};

#endif