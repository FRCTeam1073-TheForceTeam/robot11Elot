#ifndef __SmarterJagMotor_h__
#define __SmarterJagMotor_h__

#include "SmarterJagMotor.h"
#include "SmartJagMotor.h"
#include <cmath>

class SmarterJagMotor : public CANJaguar
{
public:
	SmarterJagMotor(UINT8 deviceNumber);
	void PeriodicService();
	void SetSpeed(float fSpeed);
	
private:
	
};

#endif
