//////////////////////////////////////////////////////////
// Filename: Encoders1073.cpp
// Author:   Ken Cowan / Greg Cowan
// Date:     January 30, 2011
//
// This file contains the implementation for the Encoders1073 class,
// responible for treating the encoders as a pair, and computing 
// distance travelled
//
//////////////////////////////////////////////////////////
#include "Encoders1073.h"


Encoders1073::Encoders1073(SmartGyro *g, SmartJaguarMotorEncoder *left, SmartJaguarMotorEncoder *right)
{
//	printf("Encoders1073 ctor scale %d\n", left->GetPulsePerFt());	

	leftJag = left;
	rightJag = right;
}

void Encoders1073::ResetEncoders()
{		
	leftJag->ResetEncoder();
	rightJag->ResetEncoder();
}
