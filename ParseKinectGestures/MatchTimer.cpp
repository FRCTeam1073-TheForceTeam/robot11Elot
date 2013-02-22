#include "MatchTimer.h"

bool autonomous;

MatchTimer::MatchTimer(void)
{
	timer = new Timer();
	timer->Reset();
}

void MatchTimer::StartAutonomous(void)
{
	autonomous = true;
	timer->Reset();
	timer->Start();
}

/* void MatchTimer::EndAutonomous(void)
{
	if(autonomous && timer->Get() == 15.0)
	{
		timer->Reset();
	}
	autonomous = false;
} */

void MatchTimer::StartOperator(void)
{
	autonomous = false;
	timer->Reset();
	timer->Start();
}

/* void MatchTimer::EndOperator(void)
{
	if(!autonomous && timer->Get() == 120.0)
	{
		timer->Reset();
	}
} */

float MatchTimer::GetElapsedTime(void)
{
	return (float)timer->Get();
}

float MatchTimer::GetTimeRemaining(void)
{
	if (autonomous)
		return MaxAutonomousTime - (float)timer->Get();
	else
		return MaxOperatorTime - (float)timer->Get();
}
