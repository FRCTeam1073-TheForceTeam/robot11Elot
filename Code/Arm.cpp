#include <stdio.h>
#include "WPILib.h"
#include "userincludes.h"
#include "Arm.h"
#include "SmartJoystick.h"
#include "SmartJagMotor.h"
#include "math.h"

//find actual value for maxSpeed
static const float leewayDegrees= 5.0;
static const float Kp = 1.0;
static const float Ki = .25;
static const float maxSpeed = .6f; //set this to one so that it makes sense
static const float manualUpSpeed = .90f;
static const float manualDownSpeed = .65f;
static const float timeToGoNinetyDegrees = 1.5;
static const float ReleaseAngle = 10;

bool isButtonArmTestPressed;
bool isDownPressed;
bool isUpPressed;
bool SetBrake;
float angleOfArm;
float timeToGoToReleaseAngle;

// for magEncoder to set position
//static const float maxVoltCross = 5.0;	// point at which value gets set to zero
//static const float degreesPerVolt = 20.0;

Arm::Arm(SmartJaguarMotorEncoder *ma, SmartJoystick *e, AnalogChannel *magA)
{
	motorA = ma;
	targetPosition = 0.0;
	joystick = e;
	armTimer = new Timer();
	armTimer->Reset();
	armTimer->Start();
	magEncoder = magA;
	controlMode = ManualMode;
	angleOfArm = 0;
	startTime = 0;
	timeToGoToReleaseAngle = 0;
	//zeroAngleVolts = 0;	//volt value at zero degrees
}

void
Arm::GoToReleasePosition() // sets the target position
{
#if 1
	armTimer->Reset();
	armTimer->Start();

	timeToGoToReleaseAngle = 1.00;
#else

	// targetPosition = degrees;
	startTime = (float)armTimer->Get();
	
	if(angleOfArm > ReleaseAngle)
	{
		SetMotors(manualUpSpeed);
	}
	else
	{
		SetMotors(-manualDownSpeed);
	}

	timeToGoToReleaseAngle = (fabs(angleOfArm - ReleaseAngle) / 90) * timeToGoNinetyDegrees;
#endif
	SetMotors(-manualDownSpeed);
	controlMode = ReleasePositionMode;
}

bool
Arm::IsAtReleasePosition()
{
	return controlMode != ReleasePositionMode;
}

float
Arm::GetTargetPositionDegrees() //modifies and returns target position to account for height - base
{
	return (targetPosition);
}

bool
Arm::IsAtTargetPosition() // checks to see if the Arm is at the target position, returns true if it is
{
	/*
	float current = GetCurrentPositionDegrees();
	float dis = (fabs(current - targetPosition));
	
	if(dis < leewayDegrees)
		return true;
	else
		return false;
	
	printf("Distance: %f\n",dis);
	*/
	return false;
}

float
Arm::GetCurrentPositionDegrees() //returns the current Arm position
{
	/*
	float angleVolts;
	
	// Fun Math - need to change to account for design modifications
	float curPosition = magEncoder->GetVoltage();
	
	if (degreesPerVolt > 0)
	{
		float currentOffset = curPosition - zeroAngleVolts;
		
		if(currentOffset >0)
		{
			angleVolts = currentOffset;
		}
		else
		{
			angleVolts = curPosition + maxVoltCross - zeroAngleVolts;
		}
		
		float angle = angleVolts * degreesPerVolt;
		return angle;
	}
	
	return curPosition;
	*/

	return angleOfArm;
}

void
Arm::CheckJoystick()
{
		static bool wasButtonArmTestPressed = false;	
		
		bool isButtonArmTestPressed = joystick->GetRawButton(ArmNextPositionTestButton);
		
		
		if(isButtonArmTestPressed && !wasButtonArmTestPressed)
		{
			// if 90, go to 0, if 0, go to 90
		}
	
		wasButtonArmTestPressed = isButtonArmTestPressed;
}

void Arm::CheckManualControl()
{
	static State status = Still;

	bool isDownPressed = joystick->GetRawButton(ArmManualDown);
	bool isUpPressed = joystick->GetRawButton(ArmManualUp);
	static bool wasDownPressed = false;
	static bool wasUpPressed = false;

	if(isDownPressed && !IsDownLimitSwitchActive())
	{
		#if SIMULATOR
//			printf("Arm: Moving Down\n");
		#endif

		SetMotors(-manualDownSpeed);
		controlMode = ManualMode;

		if(!wasDownPressed)
		{
			startTime = (float)armTimer->Get();
			status = GoingDown;
		}
	}
	
	else if(isUpPressed && !IsUpLimitSwitchActive())
	{
		#if SIMULATOR
//			printf("Arm: Up\n");
		#endif

		SetMotors(manualUpSpeed);
		controlMode = ManualMode;

		if(!wasUpPressed)
		{
			startTime = (float)armTimer->Get();
			status = GoingUp;
		}
	}
	else
	{
		if(controlMode == ManualMode)
		{
			SetMotors(0);
		}
		
		float dTime = (float)armTimer->Get() - startTime;

		if (status == GoingUp)
		{
			angleOfArm -= dTime * 90 / timeToGoNinetyDegrees;  
		}
		else if (status == GoingDown)
		{
			angleOfArm += dTime * 90 / timeToGoNinetyDegrees;
		}
	}
	
	wasDownPressed = isDownPressed;
	wasUpPressed = isUpPressed;
}

void 
Arm::SetMotors(float value)
{
	motorA->Set(value);
}

void
Arm::PeriodicService()
{
	if (motorA == NULL) return;
	
	CheckJoystick();
	CheckManualControl();

	
	switch(controlMode)
	{
//	printf("Arm ControlMode = %d\n", controlMode);
	
	case HomingMode:
		SetMotors(-maxSpeed/2); //both move down at half speed, can change to accomodate for others
	
		if(IsDownLimitSwitchActive())
		{
			angleOfArm = 90;
			motorA->Set(0);
			//zeroAngleVolts = magEncoder->GetVoltage();
			controlMode = ManualMode;
		}
		break;
	
	case ReleasePositionMode:
		{
			if(armTimer->Get() >= timeToGoToReleaseAngle)
			{
				SetMotors(0);
				angleOfArm = ReleaseAngle;
				controlMode = ManualMode;
			}

			break;
		} 
	
	case ManualMode:
		break;
	
	case ToUpLimitSwitchMode:
	{
		if(IsUpLimitSwitchActive())
		{
			SetMotors(0);
			angleOfArm = 0;
			controlMode = ManualMode;
		}

		break;
	}

	case ToDownLimitSwitchMode:
	{
		if(IsDownLimitSwitchActive())
		{
			SetMotors(0);
			angleOfArm = 90;
			controlMode = ManualMode;
		}

		break;
	}

	default:
		break;
	}
}

bool Arm::IsUpLimitSwitchActive()
{
	return !motorA->GetReverseLimitOK();
}

bool Arm::IsDownLimitSwitchActive()
{
	return !motorA->GetForwardLimitOK();	
}

void Arm::GoToDownPosition()
{
	if(!motorA->GetForwardLimitOK())
	{
		SetMotors(-maxSpeed);
	}
}

void Arm::GoToUpPosition()
{
	if(!motorA->GetReverseLimitOK())
	{
		SetMotors(maxSpeed);
	}
}
