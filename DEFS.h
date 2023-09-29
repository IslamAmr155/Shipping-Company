#pragma once
#ifndef Type
enum Type
{
	VIP,
	Special,
	Normal
};

enum InterfaceMode
{
	Interactive,
	Step_By_Step,
	Silent
};

enum TruckStatus {
	Waiting,
	Moving,
	Loading,
	Maintenance
};
#endif