#include "../Header Files/Event.h"

Event::Event(Time evtTime, int id, Company* pCompany): evtTime(evtTime), ID(id), pCompany(pCompany)
{
}

Time Event::getTime() const{
	return evtTime;
}

Event::~Event()
{
}