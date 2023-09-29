#include "../Header Files/Time.h"

Time::Time(int day, int hour): Day(day), Hour(hour)
{
}

Time::Time(){
	Day = -1;
	Hour = -1;
}

int Time::getDay()
{
	return Day;
}

int Time::getHour()
{
	return Hour;
}

Time::Time(int n)
{
	Day = n / 24;
	Hour = n % 24;
}

void Time::incrementTime()
{
	if (++Hour >= 24)
	{
		Hour = 0;
		Day++;
	}
}

bool Time::isTime(Time t) const
{
	if (Day == t.Day && Hour == t.Hour)
		return true;
	return false;
}

int Time::toInt() const
{
	int num{ 0 };
	num = Day * 24 + this->Hour;
	return num;
}

bool Time::operator == (const Time& time) const
{
	if (Day == time.Day && Hour == time.Hour)
		return true;
	return false;
}

bool Time::isValid()
{
	if (Day < 0 && Hour < 0)
		return false;
	return true;
}
