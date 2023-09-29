#pragma once
#include "../DEFS.h"
#include "Time.h"
#include "UI.h"

class Truck;

class Cargo
{
private:

	Time ready_time;
	int loading_time;
	Type cargoType;
	int ID;
	int deliveryDistance;
	int cost;
	Time CDT;
	int TID;
	Time waitingTime;

public:

	friend ostream& operator <<(ostream& output, Cargo*& c);

	Cargo(Type cargoType, int ID, int deliveryDistance, Time readyTime, int loadingTime, int cost);

	void setType(Type cargoType);

	void setCDT(Time t);

	Time getCDT();

	Type getType();

	void addExtraMoney(int money);

	float getDeliveryDistance();

	float getCost();

	int getWaitingTime(Time now);

	Time getReadyTime();

	int getLoadingTime();

	int getPriority();

	int getID() const;

	void setTID(int TruckID);

	int getTID() const;

	void calculateWaitingTime(Time moveTime);

	Time getWaitingTime() const;

};



