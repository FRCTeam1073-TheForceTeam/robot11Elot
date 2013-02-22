#include "Shooter.h"

#define SHOOTER_DEBUG 1
#define MANUAL_MODE 0
#define NORMAL_SHOOT_POWER 1.0
#define TOP_SHOOT_POWER 0.8
#define BOTTOM_SHOOT_POWER 1.0

static const float closingSpeed = 0.65f;
static const float openingSpeed = -0.70f;

static const float HistoryDurationSeconds = 0.250f;
static const int ShooterWindowSize = 6;
static const float StallDetectionLimit = 0.3f;

static const float PresetTargetPosition = 0.30f;
static const float AllowedPositionError = 0.10f;

Shooter::Shooter(SmartSpeedCANJaguar *mb,SmartSpeedCANJaguar *mt, SmartJoystick *opJoy)
{
	bottomMotorEncoder = mb;
	topMotorEncoder = mt;
	operatorJoystick = opJoy;
	fTMin = 5000;
	fBMin = 5000;
}

// All Motor Output requests should filter through these two functions.
void Shooter::SetTopMotorOutput(float value)
{
	static float lastValue = -99.0;		

	topMotorEncoder->Set(value);
	lastValue = value;					// Simply set last to current...
}

void Shooter::SetBottomMotorOutput(float value)
{
	static float lastValue = -99.0;		
	
	bottomMotorEncoder->Set(value);
	lastValue = value;					// Simply set last to current...
}

void Shooter::PeriodicService()
{
	if (operatorJoystick->GetRawButton(backspinShootButton))
	{
		SetBottomMotorOutput(BOTTOM_SHOOT_POWER);
		SetTopMotorOutput(TOP_SHOOT_POWER);
	}
	
	if (operatorJoystick->GetRawButton(normalShootButton))
	{
		SetBottomMotorOutput(NORMAL_SHOOT_POWER);
		SetTopMotorOutput(NORMAL_SHOOT_POWER);
	}
	//Calculating minimums
	if (topMotorEncoder->Get()<fTMin){
		fTMin = topMotorEncoder->Get();
	}
	if (bottomMotorEncoder->Get()<fBMin){
		fBMin = bottomMotorEncoder->Get();
	}
}
float Shooter::GetTopSpeed(){
	return topMotorEncoder->Get();
}
float Shooter::GetBottomSpeed(){
	return bottomMotorEncoder->Get();
}
float Shooter::GetTopMinimumSpeed(){
	return fTMin;
}
float Shooter::GetBottomMinimumSpeed(){
	return fBMin;
}
void Shooter::ResetMinimum(){
	fTMin = 5000;
	fBMin = 5000;
}
