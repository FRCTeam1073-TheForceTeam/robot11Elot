#ifndef SMARTSPEEDCANJAGUAR_H_
#define SMARTSPEEDCANJAGUAR_H_

#include "wpilib.h"

class SmartSpeedCANJaguar : public CANJaguar
{
public:
	SmartSpeedCANJaguar(UINT8 deviceNumber, int encPulsesPerRev, bool isEncReversed);
	void SetSpeedRevolutionsPerMinute(float speed);
	
	void PeriodicService();
	void SetPID(double p, double i, double d);
private:
	void Initialize();
	bool isEncoderReversed;
	int encoderPulsesPerRevolution;
	double pid_P, pid_I, pid_D;		
	double lastSetSpeed;
};


#endif
