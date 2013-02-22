#ifndef __Shooter_h__
#define __Shooter_h__

#include "userincludes.h"
#include "SmartSpeedCANJaguar.h"
#include "SmartJoystick.h"
#include <stdio.h>

class Shooter
{
public:
	Shooter(SmartSpeedCANJaguar *mb,SmartSpeedCANJaguar *mt, SmartJoystick *opJoy);
	void PeriodicService();
	void SetBottomMotorOutput(float value);
	void SetTopMotorOutput(float value);
	float GetTopSpeed();
	float GetBottomSpeed();
	float GetTopMinimumSpeed();
	float GetBottomMinimumSpeed();
	void ResetMinimum();
	
private:
	SmartSpeedCANJaguar *topMotorEncoder;
	SmartSpeedCANJaguar *bottomMotorEncoder;
	SmartJoystick *operatorJoystick;
	
	float fTMin;
	float fBMin;
};

#endif
