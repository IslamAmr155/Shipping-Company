#pragma once
class Time
{
	int Day;
	int Hour;

public:

	Time(int, int);

	Time();

	Time(int n);

	int getDay();

	int getHour();

	void incrementTime();

	bool isTime(Time) const;

	int toInt() const;

	bool operator == (const Time& time) const;

	bool isValid();
};