#pragma once
#include "Event.h"

class CancelEvent : public Event
{

public:

	CancelEvent(Time, int, Company*);
	void Execute();
};