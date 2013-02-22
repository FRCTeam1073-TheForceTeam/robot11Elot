// some random comment i should put here
#include "wpilib.h"

#ifndef MATCHTIMER_H_
#define MATCHTIMER_H_

#include "userincludes.h"

static const float MaxAutonomousTime = 15.0;
static const float MaxOperatorTime = 120.0;

class MatchTimer
{
public:
	MatchTimer();
	void StartOperator();
	void EndOperator();
	void StartAutonomous();
	void EndAutonomous();
	float GetElapsedTime();
	float GetTimeRemaining();
	
private:
	Timer *timer;
	bool autonomous;

	
};

#endif
