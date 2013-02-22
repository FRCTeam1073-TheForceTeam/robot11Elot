#include <stdio.h>
#include "WPILib.h"
#include "userincludes.h"
#include "Elevator.h"
#include "SmartJoystick.h"
#include "SmartJagMotor.h"
#include "math.h"

// Stall Detection Constant
static const float ElevatorHistoryDurationSeconds = 0.50f;  // 1 second second 
// Need to do this calculation by hand for now as VC++ has some problems...
// ElevatorWindowSize = ((int)(ElevatorHistoryDurationSeconds/WaitTime)) + 1;
static const int ElevatorWindowSize =11;  // Size is 11 based on 0.050 loop time...
static const float ElevatorStallDetectionLimit = 0.1f;
static const float StallTimeOutSeconds = 5.0f;  // Seconds
static const float BrakeOnTime = 1.0f; //seconds



//find actual values for brake on, brake off, and maxspeed.
static const float leewayfeet = 0.1f; //error range for elevator height
static const float Kp = 1.5f;
static const float Ki = 0.5f;
static const float maxSpeed = 0.7f; //set this to one so that it makes sense
static const float maxUpSpeed = 1.0f;
static const float maxDownSpeed = 0.7f;
static const float servoBrakeOn = 0.0f;
static const float servoBrakeOff = 1.0f;
static const float startingPoint = 0.54167f; //accuracy is key.
static float heights[] = { 0.0f, 0.830583f, 3.994416f, 5.144083f, 8.695916f, 9.422583f }; //as accurate as possible // purpledragon the first value is 0.0 because
static const float manualSpeed = maxSpeed;  // max speed in manual mode is same as auto max speed // the lowest peg is lower than the lowest possible elevator height
static const float VoltageRampRate = 25;	// volts/second


Elevator::Elevator(SmartJaguarMotorEncoder *ma, Servo *s1, SmartJoystick *e)
{
	motorA = ma;
	motorA->SetVoltageRampRate(VoltageRampRate);
	servo = s1;
	targetPosition = 0.0f;
	joystick = e;
	controlMode = HomingMode;
	brakeState = Idle;
	timeoutTimer.Reset();
	timeoutTimer.Start();
	currentMotorValue = 0.0f;
	elevatorIndex = 5;
	
}


bool Elevator::IsStallDetected(float encoderNow, bool doWindowReset)
{
	/*static float encoderValues[ElevatorWindowSize];
	static int index = 0;

	if(doWindowReset){
		index = 0;
		//printf("ResetCalled\n");
	}
	float currentFeet = encoderNow;
	int useIndex = index % ElevatorWindowSize;
	float oldFeet = encoderValues[useIndex];
	
//	printf("old = %f, new = %f\n",oldFeet, currentFeet );
	float travel = fabs(currentFeet - oldFeet);
//	printf("Elevator travel = %f, index = %d,use = %d \n", travel, index, useIndex);


	++index;	// Bump Index for next time...
		
	encoderValues[useIndex] = currentFeet;
		
	// Cannot have opinion on stall UNTIL we have at lest a full window of data
	if(index < ElevatorWindowSize)		// If we don't have enough data
		return false;			// Say we are NOT stalled...
	
	if (travel > ElevatorStallDetectionLimit)
	{
		return false;
	}	
	return true;
	*/
	return false;
}


void
Elevator::GoToPositionFeet(float ft) // sets the target position
{
	if(IsTimeOutMode())
		return;
	targetPosition = ft;
	SetControlMode(LocatingMode);
}

void
Elevator::DropDownHalfAPeg()
{
	const float DropDownDistance = 0.75f;
	
	if(IsTimeOutMode())
		return;
	
	if(elevatorIndex > 2)
	{
		GoToPositionFeet(heights[elevatorIndex - 1] - DropDownDistance);
		//GoToPositionFeet((heights[elevatorIndex - 1] - heights[elevatorIndex - 3]) / 2 + heights[elevatorIndex - 3]);
	}
}

float
Elevator::GetTargetPositionFeet() //modifies and returns target position to account for height - base
{
	return (targetPosition - startingPoint);
}
bool
Elevator::IsAtTargetPosition() // checks to see if the elevator is at the target position, returns true if it is
{
	float current = GetCurrentPositionFeet();
	float dis = (fabs(current - targetPosition));
	//printf("Distance: %f\n",dis);
	if(dis < leewayfeet)
	{
		return true;
	}
	else
		return false;
}
void
Elevator::SetBrake(bool brake) // sets the brake on if true and off otherwise, self explanatory
{
//	printf("brakeState=%d brake=%d brakeTimer=%f\n",brakeState,brake,brakeTimer.Get());
#if 0
	switch(brakeState){
		case Idle:
			if(!brake) {
				servo->Set(servoBrakeOff);
				brakeState = Off;
			}
			break;
		case On:
			if(brake){
				if(brakeTimer.Get() > BrakeOnTime){
					brakeState = Idle;
					servo->SetOffline();
				}
			}
			else{
				servo->Set(servoBrakeOff);
				brakeState = Off;
			}
			break;
		case Off:
			if(brake){
				servo->Set(servoBrakeOn);
				brakeState = On;
				brakeTimer.Reset();
				brakeTimer.Start();
			}
			break;
		default:
			break;
	}
#else
	if(brake)
		servo->Set(servoBrakeOn);
	else
		servo->Set(servoBrakeOff);
#endif
}
float
Elevator::GetCurrentPositionFeet() //returns the current elevator position
{
	float curposition = (float)motorA->GetPosition();
	
	return curposition;
}
void
Elevator::GoToPositionIndex(int index)
{

	if(IsTimeOutMode())
		return;
//	printf("Elev go to %f ft\n",heights[elevatorIndex-1]);
	GoToPositionFeet(heights[index-1]);
}

void
Elevator::GoToPositionIndex()
{
	if(IsTimeOutMode())
		return;

//	printf("Elev go to %f ft\n",heights[elevatorIndex-1]);
	GoToPositionFeet(heights[elevatorIndex-1]);
}

void
Elevator::SetControlMode(ControlMode newMode)
{
	
	if(newMode != controlMode)
		IsStallDetected(0.0f, true);  //Reset Window on any mode change...
	
	controlMode = newMode;
	
	
}

void
Elevator::CheckJoystick()
{
		static bool wasButtonElevatorTestPressed = false;	
		//static unsigned int testIndex = 0;
		
		//bool isButtonElevatorTestPressed = joystick->GetRawButton(ElevatorNextPositionTestButton);
		bool isButtonElevatorTestPressed = joystick->GetRawButton(GoToPositionButton);
		
		if(isButtonElevatorTestPressed && !wasButtonElevatorTestPressed)
		{
			//if(testIndex >= sizeof_array(heights))
			//	testIndex = 0;
			//GoToPositionFeet(heights[testIndex++]);
			GoToPositionIndex(elevatorIndex);
		}
		wasButtonElevatorTestPressed = isButtonElevatorTestPressed;
}
void Elevator::CheckManualControl()
{
	bool isDownPressed = joystick->GetRawButton(ElevatorManualDown);
	bool isUpPressed = joystick->GetRawButton(ElevatorManualUp);
	bool isGroundPressed = joystick->GetRawButton(ElevatorToGroundButton);
	static bool wasGroundPressed = false;

	
	
	if(isGroundPressed && !wasGroundPressed)
	{
		if(controlMode == HomingMode)
		{
			SetControlMode(ManualMode);
		}
		else
		{
			SetControlMode(HomingMode);
		}
	}
	
	wasGroundPressed = isGroundPressed;
	
	if(isDownPressed)
	{
		SetBrake(false);
		SetMotors(-manualSpeed);
		SetControlMode(ManualMode);
	}
	else if(isUpPressed)
	{
		SetBrake(false);
		SetMotors((manualSpeed*2));
		SetControlMode(ManualMode);
	}
	else{
		if(controlMode == ManualMode){
			SetControlMode(BrakeOnMode);
			SetMotors(0);
		}
	}

}

bool 
Elevator::IsDownLimitSwitchActive()
{
	return !motorA->GetReverseLimitOK();
}


void Elevator::SetMotors(float value)
{
	// check if value is between -0.4 and +0.8 (or whatever we change the constants to)
	if (value > maxUpSpeed)
		value = maxUpSpeed;
	if (value < -maxDownSpeed)
		value = -maxDownSpeed;
	
	currentMotorValue = value;
	motorA->Set(value);
	
	//printf("Enc=%f, Motor=%f, limit=%d, Target=%f\n", GetCurrentPositionFeet(), value, motorA->GetReverseLimitOK(), targetPosition);

}



const float feederStationFeet = 3.55f;  // Ben to fix
void
Elevator::GoToFeederPosition(bool activate)
{
	if(activate)
	{
	targetPosition = feederStationFeet;
	controlMode = LocatingMode;
	}
	else
		return;
}


static float i = 0;
void
Elevator::PeriodicService()
{
	// For now, add a test for NULL for our motors. Until they are connected to to the bot,
	// we can't do anything
	float lim = 0.0;

	if ((motorA == NULL)) return;
	
	if(controlMode != TimeOutMode){
		CheckJoystick();
		CheckManualControl();
	}
	
	bool isLimit = IsDownLimitSwitchActive();
	float positionNow = GetCurrentPositionFeet();
	bool isStalled = IsStallDetected(positionNow);
	bool isDrivingIntoLimit = currentMotorValue < 0 && isLimit;
	
#if 1
	//printf("Pos %f, stalled = %d\n", positionNow, isStalled);
	if(isStalled && (controlMode != BrakeOnMode)  && (controlMode != TimeOutMode) && !isDrivingIntoLimit){
		SetControlMode(TimeOutMode);
		timeoutTimer.Reset();
		printf("Stall Detected and Elevator Stopped\n");
	}
#endif
	static float prevpos = positionNow;
	static float motorval = 0;
	static float kp = 1.0;
	static float desiredv = -1.0;
//	printf("Hi. Elevator control mode is: %d\n", controlMode);
	switch(controlMode){
	
	case HomingMode:
		{
			float v = positionNow - prevpos;
			motorval = kp * (desiredv-v);
			SetBrake(false);
#if 1			
			SetMotors(motorval); //both move down at half speed, can change to accomodate for others
			//printf("mot:%f, v:%f\n", motorval, v);
#else
			SetMotors(-maxDownSpeed); //both move down at half speed, can change to accomodate for others
#endif			
			
			
			prevpos = positionNow;			
		}
			
		if(isLimit)
		{
			//printf("Elevator homing mode, reverse limit hit\n");
			SetMotors(0);
			motorA->ResetEncoder();
			targetPosition = 0;
			SetBrake(true);
			SetControlMode(BrakeOnMode);
		}
		break;
	case LocatingMode:
		{
		
		float err = (targetPosition - positionNow);
		i += err;
		lim = 2.0f;
		if(i > lim){i = lim;}
		if(i < -lim){i = -lim;}
		float output = err*Kp + i * Ki;
		if(IsAtTargetPosition()) //brakes if at the target position
		{
			//printf("Elevator locating mode, setting brake on, motor calc was %f, now zero\n", output);
			output = 0;
			SetBrake(true);
			SetControlMode(BrakeOnMode);  // if we hit our target, put the brake on and stop adjusting
		}
		else
			SetBrake(false);
		SetMotors(output);
		} break;
	case ManualMode:		
		break;
	
	case BrakeOnMode:
		SetBrake(true);
		SetMotors(0.0);
		break;
		
	case TimeOutMode:
		//rintf("In TimeOut\n");
		SetBrake(true);
		SetMotors(0.0);
		if(timeoutTimer.Get() > StallTimeOutSeconds){
			SetControlMode(BrakeOnMode);
			printf("Timeout done\n");
		}

		
	default:
		break;
	}
}
