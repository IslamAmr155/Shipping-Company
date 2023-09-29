#pragma once
#include "Event.h"
#include "Time.h"

class PreparationEvent: public Event
{
	Type cargotype;
	int dist;
	int loadTime;
	int cost;

public:

	PreparationEvent(Type, Time, int, int, int, int, Company*);
	virtual void Execute();
	virtual ~PreparationEvent();
};