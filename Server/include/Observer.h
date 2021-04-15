#pragma once
#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <string>
#include <vector>

typedef std::string EventMessageString;

struct EventMessage
{
	EventMessage() = default;
	EventMessage(void* sender, const EventMessageString& params = "") : sender(sender), params(params) {}
	void* sender;
	EventMessageString params;
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
	std::vector<Observer*> observers;
};

#endif