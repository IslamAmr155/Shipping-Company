#include "../Header Files/CancelEvent.h"

CancelEvent::CancelEvent(Time evtTime, int ID,Company* pCompany):Event(evtTime, ID, pCompany)
{
}


void CancelEvent::Execute()
{
	Cargo* cancelledCargo = nullptr;
	if (pCompany->RemoveWaitingNormal(ID, cancelledCargo))
	{
		delete cancelledCargo;
		pCompany->DecrementCargos();
	}
}
