#pragma once
#include "UI.h"
#include "../ADTs/LinkedNormal.h"
#include "../ADTs/LinkedQueue.h"
#include "Event.h"
#include "Truck.h"

class Event;
class UI;
class Truck;
class Cargo;

class Company
{
	int numOfCargos; 
	int numOfMovingCargos;
	int NTcount;
	int STcount;
	int VTcount;
	int autoP;
	int maxW;
	int TripsBeforeCheckup;
	int AutopromotedCargos;
	int TotalSimulationTime;
	Time Clock;
	LinkedQueue<Event*> EventList;

	LinkedNormal WaitingNC;
	LinkedQueue<Cargo*> WaitingSC;
	PriorityQueue<Cargo*> WaitingVC;

	LinkedQueue<Cargo*> DeliveredC;

	PriorityQueue<Truck*> WaitingNT;
	PriorityQueue<Truck*> WaitingST;
	PriorityQueue<Truck*> WaitingVT;

	PriorityQueue<Truck*> MovingT;

	Truck* LoadingNT;
	Truck* LoadingST;
	Truck* LoadingVT;

	LinkedQueue<Truck*> CheckupV;
	LinkedQueue<Truck*> CheckupN;
	LinkedQueue<Truck*> CheckupS;

	UI* interface_;

	public:

	Company();

	void AppendWaiting(Cargo*);
	bool RemoveWaitingNormal(int, Cargo*&);
	void PrintWaitingNC();
	void PrintWaitingSC();
	void PrintWaitingVC();
	void PrintDeliveredC();
	void PrintWaitingNT();
	void PrintWaitingST();
	void PrintWaitingVT();
	void PrintMovingT();
	void PrintCheckupNT();
	void PrintCheckupST();
	void PrintCheckupVT();
	void IncrementCargos();
	void DecrementCargos();
	int getLengthOfLists(int &LT, int &ET, int &MC, int &ICT, int &DC);
	Time GetClock() const;
	void FileLoading(const string filename);
	void Simulate();
	void Assign();
	void AssignMaxW();
	bool AssignNormal(Cargo* cargo, Truck* &currNT);
	bool AssignSpecial(Cargo* cargo, Truck* &currST);
	bool AssignVIP(Cargo* cargo, Truck* &currVT);
	void DeliverCargos();
	void ChecktoMove(Truck*& truck, bool isMaxW);
	void CheckCheckupTrucks();
	void ReplaceTruck(Truck*& truck, Type type, bool isMaxW);
	float calcavgTimes(float &avgUtilizationTime);
	void setTotalSimulationTime();
	void GenerateOutputFile(string filename);
	~Company();
};