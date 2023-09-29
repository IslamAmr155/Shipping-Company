#include "../Header Files/PreparationEvent.h"
#include "../Header Files/Company.h"

PreparationEvent::PreparationEvent(Type cargoType, Time evtTime, int ID, int dist, int loadTime, int cost, Company* pCompany): Event(evtTime, ID,  pCompany), loadTime(loadTime)
{
	this->cargotype = cargoType;
	this->dist = dist;
	this->cost = cost;

}

void PreparationEvent::Execute()
{
	Cargo* newCargo = new Cargo(cargotype, ID, dist, evtTime, loadTime, cost);
	pCompany->AppendWaiting(newCargo);
	pCompany->IncrementCargos();
}

PreparationEvent::~PreparationEvent()
{
}