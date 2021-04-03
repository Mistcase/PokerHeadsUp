#pragma once
#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "Types.h"

typedef std::string ObsMessageString;

struct ObsMessage
{
	ObsMessage() = default;
	ObsMessage(void* sender, const ObsMessageString& args = "") : sender(sender), args(args) {}
	void* sender;
	ObsMessageString args;
};

class Observer
{
public:
	virtual void handleEvent(const ObsMessage& message) = 0;
};


class Observerable
{
public:
	void addObserver(Observer* obs);
	void removeObserver(Observer* obs);
	virtual void notifyObservers(const ObsMessageString& message = "") = 0;

protected:
	vector<Observer*> observers;
};

#endif