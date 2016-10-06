#include "pch.h"
#include "Counter.h"


Counter::Counter() : time_elapsed{}, time_interval{}
{
}

Counter::Counter(int time_interval) : time_elapsed{}, time_interval{time_interval}
{
}

Counter::~Counter()
{
}

void Counter::ChangeInterval(float interval)
{
	time_interval = interval;
}

void Counter::Update(float elapsed)
{
	if (!Passed())
		time_elapsed += elapsed;
}

void Counter::Reset()
{
	time_elapsed = 0.f;
}

bool Counter::Passed() const
{
	if (time_elapsed > time_interval)
		return true;
	else
		return false;
}
