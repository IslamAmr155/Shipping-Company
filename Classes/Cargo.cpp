#include "../DEFS.h"
#include "../Header Files/Cargo.h"
#include <iostream>
#include <string>
using namespace std;


Cargo::Cargo(Type cargoType,int ID, int deliveryDistance, Time readyTime, int loadingTime,  int cost)
{
	this->ID = ID;
	this->cargoType = cargoType;
	this->deliveryDistance = deliveryDistance;
	this->cost = cost;
	this->ready_time = readyTime;
	this->loading_time = loadingTime;
}

void Cargo::setType(Type cargoType)
{
	this->cargoType = cargoType;
}

void Cargo::setCDT(Time t)
{
	CDT = t;
}

void Cargo::addExtraMoney(int money)
{
	cost += money;
}

Time Cargo::getCDT()
{
	return CDT;
}

Type Cargo::getType()
{
	return cargoType;
}

float Cargo::getDeliveryDistance()
{
	return deliveryDistance;
}

float Cargo::getCost()
{
	return cost;
}

Time Cargo::getReadyTime()
{
	return ready_time;
}

int Cargo::getLoadingTime()
{
	return loading_time;
}

// Multiply by -1 so that the higher the magnitude of the number, the lower it becomes
// Lower number means higher priority
//Cost has highest weight, the delivery distance then loading time
// The earlier the cargo comes (lower ready time), the higher its priority thus multiply by a negative number.

int Cargo::getPriority()
{
	if (cargoType == VIP)
	{
		return (cost * 4 + deliveryDistance * 3 - ready_time.toInt() * 2 + loading_time) * -1;
	}

	return  0;
}



int Cargo::getID() const
{
	return ID;
}

int Cargo::getWaitingTime(Time now)
{
	return now.toInt() - ready_time.toInt();
}

void Cargo::setTID(int TruckID)
{
	TID = TruckID;
}

int Cargo::getTID() const
{
	return TID;
}

void Cargo::calculateWaitingTime(Time moveTime)
{
	waitingTime = Time(moveTime.toInt() - ready_time.toInt());
}

Time Cargo::getWaitingTime() const
{
	return waitingTime;
}
