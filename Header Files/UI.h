#pragma once
#include <iostream>
#include "../DEFS.h"

using namespace std;

class Company;
class Time;
class Truck;
class Cargo;

class UI
{
	InterfaceMode UImode;
	Company* company;
public:
	UI(Company*);
	void readInterfaceMode();
	InterfaceMode getUImode();
	string readFilename();
	void PrintErrorMessage();
	void printTime(Time t);
	void PrintBreakLine();
	void PrintHour(Truck* N, Truck* S, Truck* V);
	void wait();
	static void PrintTruck(Truck* t);
	static void PrintCargo(Cargo* c);
	void StartSilent();
	void End();
};