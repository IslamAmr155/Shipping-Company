#pragma once
#include "Event.h"

class PromoteEvent : public Event
{
	int extraMoney;

public:

	PromoteEvent(Time, int ,int , Company*);
	void Execute();
};