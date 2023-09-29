#pragma once
#include "Time.h"
#include "Company.h"


class Company;

class Event
{
protected:

	Company* pCompany;
	Time evtTime;
	int ID;
public:
	Event(Time, int, Company*);
	virtual void Execute() = 0;
	Time getTime() const;
	virtual ~Event();
};