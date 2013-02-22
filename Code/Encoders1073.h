//////////////////////////////////////////////////////////
// Filename: Encoders1073.h
// Author:   Ken Cowan / Greg Cowan
// Date:     January 30, 2011
//
// Description: This file contains class defintion for the Encoders1073 class,
// responible for treating the encoders as a pair, and computing 
// distance travelled
//
//////////////////////////////////////////////////////////


#include "userincludes.h"
#include "SmartJagMotor.h"
#include "SmartGyro.h"

#ifndef ENCODERS1073_H_
#define ENCODERS1073_H_

class Encoders1073
{
protected:
	SmartJaguarMotorEncoder *leftJag;
	SmartJaguarMotorEncoder *rightJag;
	
public:
	Encoders1073(SmartGyro *g, SmartJaguarMotorEncoder *leftJag, SmartJaguarMotorEncoder *rightJag);
	void ResetEncoders();
	void InitEncoders();
	void PeriodicService();
	std::pair<double, double> GetDistance() { return pair<double, double>( leftJag->GetPosition(), rightJag->GetPosition() );}
	std::pair<double, double> GetDistanceUnaltered() { return pair<double, double>( leftJag->GetPositionUnaltered(), rightJag->GetPositionUnaltered()); }

private:
			
};

#endif
