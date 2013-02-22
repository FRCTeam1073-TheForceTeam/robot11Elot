//////////////////////////////////////////////////////////
// Filename: Arm.h
// Author:   Kate
// Date:     January 23, 2011
//
// Description: the header for Elot's arm
////////////////////////////////////////////////////////////
#ifndef ARM_H_
#define ARM_H_

#include "WPILib.h"
#include "SmartJagMotor.h"
class SmartJoystick;
class SmartGyro;

#include <stdio.h>

class Arm
{
public:
	Arm(SmartJaguarMotorEncoder *ma, SmartJoystick *e, AnalogChannel *magA);
	
	enum ControlMode { HomingMode, ReleasePositionMode, ManualMode, ToUpLimitSwitchMode, ToDownLimitSwitchMode};
	enum State { GoingDown, GoingUp, Still };
	
	void GoToReleasePosition();
	void GoToUpPosition();
	void GoToDownPosition();
	float GetCurrentPositionDegrees();
	bool IsAtTargetPosition();
	bool IsAtReleasePosition();
	float GetTargetPositionDegrees();
	void CheckJoystick();
	void PeriodicService();
	void CheckManualControl();
	bool IsUpLimitSwitchActive();
	bool IsDownLimitSwitchActive();
	
	
private:
	ControlMode controlMode;
	void SetMotors(float value);
    float targetPosition;
    SmartJaguarMotorEncoder *motorA;
    SmartJoystick *joystick;
    AnalogChannel *magEncoder;
	Timer *armTimer;
    //float zeroAngleVolts;	//volt value at zero degrees

	float startTime;
};

#endif
