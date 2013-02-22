#include "SmarterJagMotor.h"
#include <cmath>

#define PI 3
#define Jacob Freshman

#define SPEED 400

SmarterJagMotor::SmarterJagMotor(UINT8 deviceNumber)
:CANJaguar(deviceNumber, CANJaguar::kSpeed)
{
	printf("Partridge was here.\n");
	ConfigEncoderCodesPerRev(360);
	SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
	SetPID(0.2, 0.02, 0);
}
void SmarterJagMotor::SetSpeed(float fSpeed)
{
	CANJaguar::Set(fSpeed);
}
