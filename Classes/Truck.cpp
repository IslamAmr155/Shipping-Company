#include "../Header Files/Truck.h"
#include <cmath>

int Truck::currtruckID = 1;

Truck::Truck(Type truckType, int capacity, int maintenanceTime, int speed)
{
	status = Waiting;
	this->truckType = truckType;
	this->capacity = capacity;
	this->maintenanceTime = maintenanceTime;
	this->deliveryJourneys = 0;
	this->speed = speed;
	sumOfUnloadTimes = 0;
	deliveryInterval = 0;
	totalActiveTime = 0;
	truckID = currtruckID;
	currtruckID++;
	prevLoad = 0;
}

int Truck::getID() const
{
	return truckID;
}

Type Truck::getType() const
{
	return truckType;
}

Type Truck::getCargoType() const
{
	return cargoType;
}

void Truck::setStatus(TruckStatus status)
{
	this->status = status;
}

void Truck::setmaxW(bool flag)
{
	maxW = flag;
}

bool Truck::getmaxW() const
{
	return maxW;
}

TruckStatus Truck::getStatus() const
{
	return status;
}

void Truck::incrementJourneys()
{
	deliveryJourneys++;
}

int Truck::getDeliveryJourneys() const
{
	return deliveryJourneys;
}

int Truck::getCapacity() const
{
	return capacity;
}

int Truck::getNoOfCargos() const
{
	return MovingC.getLength();
}

bool Truck::isFull() const
{
	return getNoOfCargos() == capacity;
}
// Trucks are sorted according to their different speeds and capacities.
// Speed increases priority while capacity decreases it.
// Multiply by negative one since lower number means higher priority
int Truck::getPriority() const
{
	return (capacity * -2 + speed * 3) * -1;
}

//returns the time of the cargo that has waited the longest in the truck
int Truck::getMaxWaitingCargo(Time Clock) 
{
	if (!MaxWaitingCargo.isValid())
		return 0;
	return Clock.toInt() - MaxWaitingCargo.toInt();
}

//returns the time at which the first cargo currently in the truck arrives
Time Truck::getFirstArrival()
{
	Cargo* cargo;
	if (MovingC.peek(cargo)) 
	{
		float dist = cargo->getDeliveryDistance();
		int load = cargo->getLoadingTime();
		return Time(MoveTime.toInt() + ceil(dist / speed) + prevLoad + load); //prevLoad is the unload times of all cargos before the current first cargo 
	}
	return finalTime;
}

void Truck::setdeliveryInterval()
{
	deliveryInterval = ceil(2 *(float)distanceOfFurthest / speed + sumOfUnloadTimes); //ceil to allow printing to be done in next hour instead of current one
}

void Truck::setMoveTime(const Time& time)
{
	MoveTime = time;
}

Time Truck::getMoveTime() const
{
	return MoveTime;
}

float Truck::getTotalActiveTime()
{
	return totalActiveTime;
}

float Truck::getTruckUtilizationTime(int simulationTime)
{
	//assuming all cargos are delivered
	if (!deliveryJourneys)
		return 0;
	if (!totalActiveTime) //to prevent division by 0
		totalActiveTime = 1;
	return (((float)totaldeliveredCargos / (capacity * deliveryJourneys)) * (totalActiveTime / simulationTime)) * 100;
}

bool Truck::load(Cargo* &item, Time clock)
{
	if (finalTime.isValid())
		return false;
	cargoType = item->getType();
	totalActiveTime += item->getLoadingTime(); //loading time is included as part of a truck's active time
	status = Loading;
	calculatefinaltime(clock, item->getLoadingTime()); //finds the time when the truck finishes loading
	MovingC.enqueue(item,item->getDeliveryDistance());
	if(distanceOfFurthest < item->getDeliveryDistance()) //finds the cargo with the furthest distance to calculate the journey time accordingly
		distanceOfFurthest = item->getDeliveryDistance();
	sumOfUnloadTimes += item->getLoadingTime();
	return true;
}

void Truck::unload(Cargo* &item)
{
	MovingC.dequeue(item);
	prevLoad += item->getLoadingTime();
	totalActiveTime += item->getLoadingTime();
	//reset data members that are journey-dependent for the next journey 
	if (MovingC.isEmpty()) 
	{ 
		distanceOfFurthest = 0;
		sumOfUnloadTimes = 0;
	}
}

//returns the time when a truck finishes loading/unloading, or maintenance, or its journey
int Truck::calculatefinaltime(Time Clock, int loadTime)
{
	switch (status) {
	case Moving:
		finalTime = Time(Clock.toInt() + deliveryInterval);
		break;
	case Loading:
		finalTime = Time(Clock.toInt() + loadTime);
		break;
	case Maintenance:
		finalTime = Time(Clock.toInt() + maintenanceTime);
		break;
	}
	return finalTime.toInt();
}

void Truck::resetFinalTime() //sets it with arbitrary values
{
	finalTime = Time();
}

void Truck::PrintMovingCargo() const
{
	MovingC.Print();
}

void Truck::resetLoad()
{
	prevLoad = 0;
}

Time Truck::getfinalTime() const
{
	return finalTime;
}

//checks if a truck has finished loading a cargo to allow it to start loading more cargos or move the truck on its journey 
void Truck::checkLoading(Time Clock) 
{
	if (finalTime.isTime(Clock))
	{
		finalTime = Time();
	}
}

void Truck::addJourneyTime()
{
	totalActiveTime += ((float)distanceOfFurthest / speed);
}

//adds the cargos delivered by a truck in its current journey to the total number of cargos delivered in all journeys
void Truck::incrementTotalDeliveredCargos() 
{
	totaldeliveredCargos += MovingC.getLength();
}

Truck::~Truck()
{
}
