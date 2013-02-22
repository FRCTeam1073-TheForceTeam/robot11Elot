#ifndef ELEVATOR_H_
#define ELEVATOR_H_
//////////////////////////////////////////////////////////
// Filename: Elevator.h
// Author:   Chaaaales
// Date:     January 23, 2011
//
// Description: This contains the elevator manager! Yay!1oneeleven11!!!
////////////////////////////////////////////////////////////

#include "WPILib.h"
#include "SmartJagMotor.h"
class SmartJoystick;
class SmartGyro;

#include <stdio.h>


class Elevator
{
	friend class KrakenDrive;
	
public:
	Elevator(SmartJaguarMotorEncoder *ma, Servo *s1, SmartJoystick *e);
	
	enum ControlMode { HomingMode, LocatingMode, ManualMode, BrakeOnMode, TimeOutMode };
	enum BrakeState { On, Off, Idle };
	
	void GoToPositionFeet(float ft);
	void GoToPositionIndex(int index);  // Go to positions 1 through 6
	void GoToPositionIndex();  // Go to elevatorIndex
	float GetCurrentPositionFeet();
	bool IsAtTargetPosition();
	float GetTargetPositionFeet();
	void CheckJoystick();
	void PeriodicService();
	void CheckManualControl();
	bool GetLimitSwitch()
	{ return !motorA->GetReverseLimitOK(); }
	
	// **KC** Removed GetBrake due to compiler warning, and I don't think it's being called.
	// Visual C++ says warning C4800: 'Servo *' : forcing value to bool 'true' or 'false' (performance warning)	
	/*bool GetBrake()
	{ return servo;} */
	
	float GetMotors()
	{
		return currentMotorValue;
	}
	
	void DropDownHalfAPeg();
	
	bool IsTimeOutMode() { return(controlMode == TimeOutMode); }
	void UpdateTargetPoleAndFoot( int pole, int foot) 
	{ 
		// Converts pole (1-6) with foot (1-3) to 1-6 index
		elevatorIndex = 6;
		
		if(pole == 1 || pole == 3 || pole == 4 || pole == 6)
		{
			if(foot == 1) elevatorIndex = 1;
			if(foot == 2) elevatorIndex = 3;
			if(foot == 3) elevatorIndex = 5;
		}

		else if(pole == 2 || pole == 5)
		{
			if(foot == 1) elevatorIndex = 2;
			if(foot == 2) elevatorIndex = 4;
			if(foot == 3) elevatorIndex = 6;
		}
		else
		{
			printf("Invalid pole\n");
		}
		
		printf("elevatorIndex: %d, x=%d, y=%d\n", elevatorIndex, pole, foot);
	}
	
	void GoToFeederPosition(bool  activate);

	
private:
	bool IsStallDetected(float curFeet, bool doWindowReset = false);
	bool IsDownLimitSwitchActive();
	ControlMode controlMode;
	BrakeState brakeState;
	void SetControlMode(ControlMode newMode);
	float currentMotorValue;
	void SetBrake(bool brake);
	void SetMotors(float value);
    float targetPosition;
    SmartJaguarMotorEncoder *motorA;
    SmartJaguarMotorEncoder *motorB;
    Servo *servo;
    SmartJoystick *joystick;
    Timer timeoutTimer;
	Timer brakeTimer;
	

	int elevatorIndex;
};

#endif
