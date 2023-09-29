#include <string>
#include <fstream>
#include "../DEFS.h"
#include "../Header Files/Company.h"
#include "../Header Files/PreparationEvent.h"
#include "../Header Files/CancelEvent.h"
#include "../Header Files/PromoteEvent.h"


Company::Company():Clock(1,0)
{
	numOfCargos = 0;
	numOfMovingCargos = 0;
	interface_ = new UI(this);
	LoadingNT = nullptr;
	LoadingST = nullptr;
	LoadingVT = nullptr;
	AutopromotedCargos = 0;
}

void Company::IncrementCargos()
{
	numOfCargos++;
}

void Company::DecrementCargos()
{
	numOfCargos = (numOfCargos == 0) ? 0 : --numOfCargos;
}

//Adds cargo to its appropriate list according to its type 
void Company::AppendWaiting(Cargo* newCargo)
{
	Type type = newCargo->getType();
	switch (type)
	{
	case Normal:
		WaitingNC.insert(WaitingNC.getLength()+1, newCargo);
		break;
	case Special:
		WaitingSC.enqueue(newCargo);
		break;
	case VIP:
		WaitingVC.enqueue(newCargo, newCargo->getPriority());
		break;
	}
}

//Removes normal cargo if there is a cancellation event and if the cargo is present in the waiting list and not reserved by any loading truck
bool Company::RemoveWaitingNormal(int ID, Cargo* &removedCargo)
{
	if (LoadingNT && LoadingNT->getCapacity() - LoadingNT->getNoOfCargos() == WaitingNC.getLength())
		return false;
	int i = WaitingNC.search(ID, removedCargo);
	return WaitingNC.remove(i);
}

//Function that calculates the lengths of lists to be used in printing to display the total number of trucks or cargos
int Company::getLengthOfLists(int &LT, int &ET, int &MC, int &ICT, int &DC)
{
	LT = (LoadingNT ? 1:0) + (LoadingST ? 1:0) + (LoadingVT ? 1:0);
	ET = WaitingNT.getLength() + WaitingST.getLength() + WaitingVT.getLength();
	MC = numOfMovingCargos;
	ICT = CheckupN.getLength() + CheckupV.getLength() + CheckupS.getLength();
	DC = DeliveredC.getLength();
	return (WaitingNC.getLength() + WaitingSC.getLength() + WaitingVC.getLength());
}

Time Company::GetClock() const
{
	return Clock;
}

void Company::FileLoading(string filename)
{
	//temporary
	int NTspeed, STspeed, VTspeed;

	int NTcap, STcap, VTcap;

	int NTtrips, STtrips, VTtrips;

	ifstream inFile;

	// Opening the file after reading its name from the user in simulate.
	inFile.open(filename);
	// Making user re-input file name until a correct one is entered.
	while(!inFile.is_open())
	{
		interface_->PrintErrorMessage();
		filename = interface_->readFilename();
		inFile.open(filename);
	}
	if (inFile.is_open())
	{
		inFile >> NTcount >> STcount >> VTcount;
		inFile >> TripsBeforeCheckup >> NTtrips >> STtrips >> VTtrips;

		// Create trucks

		for (int i = 0; i < NTcount; i++)
		{
			inFile >> NTspeed >> NTcap;
			Truck* t = new Truck(Normal, NTcap, NTtrips, NTspeed);
			WaitingNT.enqueue(t, t->getPriority());
		}

		for (int i = 0; i < STcount; i++)
		{
			inFile >> STspeed >> STcap;
			Truck* t = new Truck(Special, STcap, STtrips, STspeed);
			WaitingST.enqueue(t, t->getPriority());
			
		}

		for (int i = 0; i < VTcount; i++)
		{
			inFile >> VTspeed >> VTcap;
			Truck* t = new Truck(VIP, VTcap, VTtrips, VTspeed);
			WaitingVT.enqueue(t, t->getPriority());
		}

		inFile >> autoP >> maxW;

		// Create events

		int numOfEvents, day, hours, id, dist, LT, cost;
		char eventType, cargoType, temp;

		inFile >> numOfEvents;
		
		for (int i = 0; i < numOfEvents; i++)
		{
			
			inFile >> eventType;
			if (eventType == 'R')
			{
				inFile >> cargoType;
				inFile >> day >> temp >> hours >> id >> dist >> LT >> cost;
				Time eTime(day, hours);

				Type type;
				if (cargoType == 'N')
					type = Normal;
				else if (cargoType == 'S')
					type = Special;
				else if (cargoType == 'V')
					type = VIP;

				Event* ev = new PreparationEvent(type, eTime, id, dist, LT, cost, this);
				EventList.enqueue(ev);
			}
			else if (eventType == 'X')
			{
				inFile >> day >> temp >> hours >> id;
				Time eTime(day, hours);
				Event* ev = new CancelEvent(eTime, id, this);
				EventList.enqueue(ev);
			}
			else if (eventType == 'P')
			{
				inFile >> day >> temp >> hours >> id >> cost;
				Time eTime(day, hours);
				Event* ev = new PromoteEvent(eTime, id, cost, this);
				EventList.enqueue(ev);
			}
		}
	}
}

void Company::Simulate()
{
	//Read file name from user and loads the file, then asks user what type of interface does he/she wants
	string filename =  interface_->readFilename();
	FileLoading(filename);
	interface_->readInterfaceMode();
	interface_->wait(); //To remove the last enter key from the buffer to avoid printing the first hour on its own
	
	Event* eve;
	
	if (interface_->getUImode() == Silent)
	{
		interface_->StartSilent();
	}

	//This loop is the main loop of the project and it will end only when there are no events left, all cargos are in the delivered list and the there are no trucks moving or in maintenance
	while(!EventList.isEmpty() || numOfCargos != DeliveredC.getLength() || !CheckupN.isEmpty() || !CheckupS.isEmpty() || !CheckupV.isEmpty() || !MovingT.isEmpty())
	{
		EventList.peek(eve);
		while (eve->getTime() == Clock && !EventList.isEmpty())
		{
			eve->Execute();
			EventList.dequeue(eve);
			delete eve;
			EventList.peek(eve);
		}

		//Check if any Normal Cargo reached autoP and promotes it
		Cargo* C;
		for(int i = 1; i < WaitingNC.getLength() + 1; i++)
		{
			WaitingNC.getEntry(i, C);
			if (C->getWaitingTime(Clock) == autoP*24)
			{
				AutopromotedCargos++;
				Event* autoPAction = new PromoteEvent(Clock, C->getID(), 0, this);
				autoPAction->Execute();
				delete autoPAction;
				i--;
			}
		}

		//Check if any trucks in maintenance are done to return them to waiting
		CheckCheckupTrucks();

		//Checks if there is cargo that should be delivered from any moving truck and returns trucks to their corresponding waiting lists or to maintenance if it reached the number of journeys that was specified in the input file
		DeliverCargos();

		//Assigns cargo to its suitable truck
		Assign();

		//If chosen interface mode is not silent it prints the lists on the console
		if (interface_->getUImode() != Silent)
		{
			interface_->PrintHour(LoadingNT,LoadingST,LoadingVT);
		}
		Clock.incrementTime();
	}
	setTotalSimulationTime(); // Calculates total simulation time to be used in calculations of average active time and average utilization of trucks for the output file

	GenerateOutputFile(filename);
	interface_->End();
}

//Checks if trucks in maintenance should return to their appropriate waiting lists if they finished their maintenance 
void Company::CheckCheckupTrucks()
{
	Truck* truck;
	while(CheckupV.peek(truck) && truck->getfinalTime().isTime(Clock))
	{
		truck->resetFinalTime();
		CheckupV.dequeue(truck);
		truck->setStatus(Waiting);
		WaitingVT.enqueue(truck, truck->getPriority());
	}

	while(CheckupN.peek(truck) && truck->getfinalTime().isTime(Clock))
	{
		truck->resetFinalTime();
		CheckupN.dequeue(truck);
		truck->setStatus(Waiting);
		WaitingNT.enqueue(truck, truck->getPriority());
	}

	while(CheckupS.peek(truck) && truck->getfinalTime().isTime(Clock))
	{
		truck->resetFinalTime();
		CheckupS.dequeue(truck);
		truck->setStatus(Waiting);
		WaitingST.enqueue(truck, truck->getPriority());
	}
}
/*
 We need trucks to assign cargos in special cases not always; if no assignment is to happen, no need to call trucks from their waiting lists 
 1. If the company is off hours, then no assignment should happen, therefore no need to replace the trucks
 Otherwise:
 2. a.)If the required cargo type is normal; normal trucks or else vip trucks are required
 2. b.)If the required cargo type is special; special trucks are required
 2. c.)If the required cargo type is VIP; vip trucks or else normal trucks or else special trucks are required
 */
void Company::ReplaceTruck(Truck*& truck, Type type, bool isMaxW)
{
	if (5 > Clock.getHour() || Clock.getHour() >= 23) //if clock is beyond the off-hours and there is a moving truck then do not replace the truck
		return;
	if (truck)
		return;
	Truck* tempNT = nullptr, * tempST = nullptr , * tempVT = nullptr;
	WaitingNT.peek(tempNT);
	WaitingST.peek(tempST);
	WaitingVT.peek(tempVT);

	//replaces truck according to the assignment criteria of cargo type sent as a parameter
	switch (type)
	{
	case Normal:
	{
		if(tempNT && (tempNT->getCapacity() <= WaitingNC.getLength() || isMaxW))
		{
			WaitingNT.dequeue(truck);
			return;
		}
		if (tempVT && (tempVT->getCapacity() <= WaitingNC.getLength() || isMaxW))
		{
			WaitingVT.dequeue(truck);
			return;
		}
		break;
	}
	case VIP:
	{
		if (tempVT && tempVT->getCapacity() <= WaitingVC.getLength() )
		{
			WaitingVT.dequeue(truck);
			return;
		}
		if (tempNT && tempNT->getCapacity() <= WaitingVC.getLength())
		{
			WaitingNT.dequeue(truck);
			return;
		}
		if (tempST && tempST->getCapacity() <= WaitingVC.getLength())
		{
			WaitingST.dequeue(truck);
			return;
		}
		break;
	}
	case Special:
	{
		if (tempST && (tempST->getCapacity() <= WaitingSC.getLength() || isMaxW))
		{
			WaitingST.dequeue(truck);
			return;
		}
		break;
	}
	}
}

/*This function calculates the total active time and the average
utilization time of all the trucks so that it can be stored and viewed in the output file*/
float Company::calcavgTimes(float& avgUtilizationTimes)
{
	float totalActiveTime = 0;
	float totalUtilizationTime = 0;
	Truck* temp;
	while (WaitingNT.dequeue(temp)) {
		totalActiveTime += temp->getTotalActiveTime();
		totalUtilizationTime += temp->getTruckUtilizationTime(TotalSimulationTime);
		delete temp;
	}
	while (WaitingST.dequeue(temp)) {
		totalActiveTime += temp->getTotalActiveTime();
		totalUtilizationTime += temp->getTruckUtilizationTime(TotalSimulationTime);
		delete temp;
	}
	while (WaitingVT.dequeue(temp)) {
		totalActiveTime += temp->getTotalActiveTime();
		totalUtilizationTime += temp->getTruckUtilizationTime(TotalSimulationTime);
		delete temp;
	}
	float percentage = (totalActiveTime / (NTcount + STcount + VTcount) / TotalSimulationTime) * 100;
	avgUtilizationTimes = (totalUtilizationTime / (NTcount + STcount + VTcount));
	return percentage;
}


/*This function calculates the total simulation time to be used in the 
total active time and average utilization time calculation. The reason 
why we subtract 25 from the clock is because the simulation starts from 
Day:1 , Hour:0 so we need to subtract 24 for accurate calculations. 
The extra hour subtracted is because an extra hour is added at the end of
the simulate function before exiting the while loop when the simulation ends, 
so we need to remove this extra hour for calculating accurate results.*/
void Company::setTotalSimulationTime()
{
	TotalSimulationTime = Clock.toInt() - 25;
}

void Company::Assign()
{
	/*The function starts by getting a truck, if needed, according to the cargo type.
	Then, check if the truck finished loading a cargo, then make it available for future assignments.
	Then, check if truck needs to move as it reached its full capacity or because a cargo reached its max wait time*/

	ReplaceTruck(LoadingVT, VIP, false);
	if (LoadingVT)
	{
		LoadingVT->checkLoading(Clock);
		ChecktoMove(LoadingVT, LoadingVT->getmaxW());
		ReplaceTruck(LoadingVT, VIP, false);
	}

	ReplaceTruck(LoadingNT, Normal, false);
	if (LoadingNT)
	{
		LoadingNT->checkLoading(Clock);
		ChecktoMove(LoadingNT, LoadingNT->getmaxW());
		ReplaceTruck(LoadingNT, Normal, false);
	}

	ReplaceTruck(LoadingST, Special, false);
	if (LoadingST)
	{
		LoadingST->checkLoading(Clock);
		ChecktoMove(LoadingST, LoadingST->getmaxW());
		ReplaceTruck(LoadingST, Special, false);
	}
	//Check for maxW cargos before normal ones
	AssignMaxW();

	//Normal Assignment
	Cargo* ctemp;
	if (WaitingVC.peek(ctemp) && AssignVIP(ctemp, LoadingVT))
		WaitingVC.dequeue(ctemp);

	if (WaitingSC.peek(ctemp) && AssignSpecial(ctemp, LoadingST))
		WaitingSC.dequeue(ctemp);

	if (WaitingNC.getEntry(1, ctemp) && AssignNormal(ctemp, LoadingNT))
		WaitingNC.remove(1);
}

void Company::AssignMaxW()
{
	Cargo* ctemp;
	LinkedQueue<Cargo*> temp;
	int waitingtime;

	//Normal cargos that may reach maxW are in the front of the normal waiting list
	//If there is a normal cargo that reached its max wait time then try and assign it immediately to an appropriate truck
	while (WaitingNC.getEntry(1, ctemp)) 
	{
		waitingtime = ctemp->getWaitingTime(Clock);
		if (waitingtime >= maxW)
		{
			ReplaceTruck(LoadingNT, Normal, true);
			if (AssignNormal(ctemp, LoadingNT))
			{
				WaitingNC.remove(1);
				LoadingNT->setmaxW(true);
			}
			else break;
		}
		else
			break;
	}

	//Special cargos that may reach maxW are in the front of the special waiting queue
	//If there is a special cargo that reached its max wait time then try and assign it immediately to an appropriate truck
	while (WaitingSC.peek(ctemp)) {
		waitingtime = ctemp->getWaitingTime(Clock);
		if (waitingtime >= maxW)
		{
			ReplaceTruck(LoadingST, Special, true);
			if (AssignSpecial(ctemp, LoadingST))
			{
				WaitingSC.dequeue(ctemp);
				LoadingST->setmaxW(true);
			}
			else break;
		}
		else
			break;
	}
}

bool Company::AssignVIP(Cargo* cargo, Truck*& LoadingVT)
{
	if (LoadingVT && LoadingVT->load(cargo, Clock))
		return true;
	return false;
}

bool Company::AssignNormal(Cargo* cargo,Truck*& LoadingNT)
{
	
	if (LoadingNT && LoadingNT->load(cargo, Clock))
		return true;
	return false;
}

bool Company::AssignSpecial(Cargo* cargo, Truck*& LoadingST)
{
	if (LoadingST && LoadingST->load(cargo, Clock))
		return true;
	return false;
}

void Company::DeliverCargos()
{
	Cargo* cargo;
	Truck* tempT;

	//Check if any cargos should be delivered now for all moving trucks or a truck has returned to the company
	while (MovingT.peek(tempT) && tempT->getFirstArrival().isTime(Clock))
	{
		MovingT.dequeue(tempT);
		if (tempT->getNoOfCargos()) //if the truck has cargos which means that this is the time for delivering a cargo
		{
			cargo = nullptr;
			tempT->unload(cargo);
			cargo->setCDT(Clock);
			cargo->setTID(tempT->getID());
			cargo->calculateWaitingTime(tempT->getMoveTime());
			DeliveredC.enqueue(cargo);
			numOfMovingCargos--;

			MovingT.enqueue(tempT, tempT->getFirstArrival().toInt());
			continue;
		}
		
		//if the previous if condition for a truck was false, this means that the truck has returned to the company
		tempT->resetFinalTime();
		tempT->incrementJourneys();
		if (tempT->getDeliveryJourneys()%TripsBeforeCheckup == 0) //if the truck has reached the number of journeys for maintenance
		{
			tempT->setStatus(Maintenance);
			tempT->calculatefinaltime(Clock);
			switch(tempT->getType())
			{
			case Normal: 
				CheckupN.enqueue(tempT);
				break;
			case Special:
				CheckupS.enqueue(tempT);
				break;
			case VIP:
				CheckupV.enqueue(tempT);
				break;
			}
			continue;
		}
		// if there is no maintenance needed,return the truck to the waiting list according to its type
		switch (tempT->getType())
		{
		case Normal:
			WaitingNT.enqueue(tempT, tempT->getPriority());
			break;
		case Special:
			WaitingST.enqueue(tempT, tempT->getPriority());
			break;
		case VIP:
			WaitingVT.enqueue(tempT, tempT->getPriority());
			break;
		}
	}
}

/*Check if a truck has to move as it reached its full capacity or one of its
cargo reached its max wait time.*/
void Company::ChecktoMove(Truck*& truck, bool isMaxW)
{
	if (!truck->getfinalTime().isValid() && (truck->isFull() || isMaxW))
	{
		truck->setmaxW(false);
		truck->resetLoad();
		truck->resetFinalTime();
		truck->setStatus(Moving);
		truck->addJourneyTime();
		truck->setdeliveryInterval();
		truck->calculatefinaltime(Clock);
		truck->incrementTotalDeliveredCargos();
		truck->setMoveTime(Clock);
		MovingT.enqueue(truck, truck->getFirstArrival().toInt());
		numOfMovingCargos += truck->getNoOfCargos();
		truck = nullptr;
	}
}

void Company::GenerateOutputFile(string filename)
{
	Cargo* ptr;
	int sumWaitPeriod = 0, NDcount = 0, VDcount = 0, SDcount = 0;
	float avgUtilizationTime;
	// Creating output file, opening it.
	ofstream outputFile{"Output " + filename};
	outputFile << "CDT\tID\tPT\tWT\tTID" << endl;

	// writing data of cargos into the file
	while(DeliveredC.dequeue(ptr))
	{
		outputFile << ptr->getCDT().getDay() << ":" << ptr->getCDT().getHour() << "\t";
		outputFile << ptr->getID() << "\t";
		outputFile << ptr->getReadyTime().getDay() << ":" << ptr->getReadyTime().getHour() <<  "\t";
		outputFile << ptr->getWaitingTime().getDay() << ":" << ptr->getWaitingTime().getHour() << "\t";
		outputFile << ptr->getTID() << "\t" << endl;
		sumWaitPeriod += ptr->getWaitingTime().toInt();
		switch (ptr->getType())
		{
		case Normal:
			NDcount++;
			break;
		case Special:
			SDcount++;
			break;
		case VIP:
			VDcount++;
			break;
		}
		delete ptr;
	}
	outputFile << "------------------------------------" << endl << "------------------------------------" << endl;

	int averageWaitPeriod = numOfCargos ? sumWaitPeriod/numOfCargos : 0;

	outputFile << "Cargos: " << numOfCargos << "\t[N: " << NDcount << ", S: " << SDcount << ", V: " << VDcount << "]" << endl;

	Time avgWaitTime(averageWaitPeriod);

	outputFile << "Cargo Avg Wait = " << avgWaitTime.getDay() << ":" << avgWaitTime.getHour() << endl;

	float percent = 0;
	if (NDcount + AutopromotedCargos)
		percent = (float)AutopromotedCargos / (NDcount + AutopromotedCargos) * 100;

	outputFile << "Auto-promoted Cargos: " << percent << "%" << endl;

	int Tcount = NTcount + STcount + VTcount;

	outputFile << "Trucks: " << Tcount << "\t[N: " << NTcount << ", S: " << STcount << ", V: " << VTcount << "]" << endl;

	outputFile << "Avg Active Time = " << calcavgTimes(avgUtilizationTime) << "%" << endl;

	outputFile << "Avg utilization = " << avgUtilizationTime << "%" << endl;

	outputFile.close();	
}

void Company::PrintWaitingNC()
{
	WaitingNC.Print();
}

void Company::PrintWaitingSC()
{
	WaitingSC.Print();
}

void Company::PrintWaitingVC()
{
	WaitingVC.Print();
}

void Company::PrintDeliveredC()
{
	DeliveredC.Print();
}


void Company::PrintWaitingNT()
{
	WaitingNT.Print();
}

void Company::PrintWaitingST()
{
	WaitingST.Print();
}

void Company::PrintWaitingVT()
{
	WaitingVT.Print();
}

void Company::PrintMovingT()
{
	MovingT.Print();
}

void Company::PrintCheckupNT()
{
	CheckupN.Print();
}

void Company::PrintCheckupST()
{
	CheckupS.Print();
}

void Company::PrintCheckupVT()
{
	CheckupV.Print();
}
Company::~Company()
{
	delete interface_;
}

