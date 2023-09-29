#pragma once
#include "../ADTs/PriorityQueue.h"
#include "../ADTs/LinkedQueue.h"
#include "Cargo.h"

class Cargo;

class Truck
{
private:
	PriorityQueue<Cargo*> MovingC;
	Type truckType;
	Type cargoType;
	int capacity;
	int maintenanceTime;
	int deliveryJourneys;
	int speed;
	float deliveryInterval;
	float totalActiveTime;
	static int currtruckID;
	int truckID;
	Time MoveTime;
	int distanceOfFurthest; //To be used in functions total active time and delivery interval  
	int sumOfUnloadTimes;
	TruckStatus status;
	Time MaxWaitingCargo;
	Time finalTime;
	int totaldeliveredCargos;
	int prevLoad;
	bool maxW;
public:
	friend ostream& operator <<(ostream& output, Truck*& c);
	Truck(Type truckType, int capacity, int maintenanceTime, int speed);
	int getID() const;
	Type getType() const;
	void setStatus(TruckStatus);
	void setmaxW(bool flag);
	bool getmaxW() const;
	TruckStatus getStatus() const;
	int getCapacity() const;
	int getDeliveryJourneys() const;
	void incrementJourneys();
	int getNoOfCargos() const;
	Type getCargoType() const;
	bool isFull() const;
	void PrintMovingCargo() const;
	void setMoveTime(const Time& time);
	Time getMoveTime() const;
	void setdeliveryInterval();
	float getTotalActiveTime();
	float getTruckUtilizationTime(int simulationTime);
	bool load(Cargo*& item, Time clock);
	void unload(Cargo*& item);
	int calculatefinaltime(Time Clock, int loadTime = 0);
	int getPriority() const;
	int getMaxWaitingCargo(Time Clock);
	Time getFirstArrival();
	Time getfinalTime() const;
	void resetFinalTime();
	void resetLoad();
	void checkLoading(Time Clock);
	void addJourneyTime();
	void incrementTotalDeliveredCargos();
	~Truck();
};