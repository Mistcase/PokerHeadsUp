#include "Observer.h"

void Observerable::addObserver(Observer * obs)
{
	observers.emplace_back(obs);
}

void Observerable::removeObserver(Observer * obs)
{
	auto iterator = std::find(observers.begin(), observers.end(), obs);
	if (iterator != observers.end())
		observers.erase(iterator);
}
