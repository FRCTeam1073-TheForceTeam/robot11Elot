//////////////////////////////////////////////////////////
// Filename: SmartGyro.cpp
// Author:   Ken Cowan
// Date:     Feb 6, 2011
//
// This file contains ...
//
//////////////////////////////////////////////////////////
#include "SmartGyro.h"

SmartGyro::SmartGyro(UINT32 port) : Gyro(port)
{
	SetSensitivity(0.006f);
}

float SmartGyro::GetAngle()
{
	float raw_angle = this->Gyro::GetAngle();
	return raw_angle;
	
}

float SmartGyro::GetAngleUnaltered()
{
	float raw_angle = this->Gyro::GetAngle();	
	return raw_angle;
}

void SmartGyro::Reset()
{
	printf("Reset gyro, wait 1 second\n");
	
	Gyro::Reset();
	Wait(1.0);
	float initial_angle = GetAngleUnaltered();
	printf("Gyro initial angle is %3.5f\n", initial_angle);
}
