#include "../Header Files/PromoteEvent.h"

PromoteEvent::PromoteEvent(Time evtTime, int ID, int extraMoney, Company* pCompany) : Event(evtTime, ID, pCompany)
{
	this->extraMoney = extraMoney;
}

void PromoteEvent::Execute()
{
	Cargo* promotedCargo = nullptr;
	if (pCompany->RemoveWaitingNormal(ID, promotedCargo))
	{
		promotedCargo->addExtraMoney(extraMoney);
		promotedCargo->setType(VIP);
		pCompany->AppendWaiting(promotedCargo);
	}
}
