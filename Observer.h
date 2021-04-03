#pragma once
#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "Types.h"

class Observer
{
public:
	virtual void handleEvent(void* subject) = 0;
};


class Observerable
{
public:
	void addObserver(Observer* obs);
	void removeObserver(Observer* obs);
	virtual void notifyObservers() = 0;

protected:
	vector<Observer*> observers;
};

#endif