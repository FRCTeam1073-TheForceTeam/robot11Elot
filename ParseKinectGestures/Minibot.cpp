#include "Minibot.h"
#include "Arm.h"
#include "Pincer.h"

static const float FirstServoAngle = 0; //Dummy value, fill in later.
static const float SecondServoAngle = 78; //Dummy value, fill in later.

static const float FirstDeployServoAngle = 100.0
; //Dummy value, fill in later.
static const float SecondDeployServoAngle = 0; //Dummy value, fill in later.

Minibot::Minibot(Servo *one, Servo *two, SmartJoystick *rJoy, SmartJoystick *opJoy, Arm *arm, Pincer *pincer)
{
	firstStageServo = one;
	secondStageServo = two;
	rightJoystick = rJoy;
	operatorJoystick = opJoy;
	stageOneDeployed = false;
	stageTwoDeployed = false;
	_arm = arm;
	_pincer = pincer;
	
	InitializeServos();
}

void Minibot::PeriodicService()
{
	if (operatorJoystick->GetRawButton(MinibotFirstStageDeployButton))								
	{
		DeployFirstStage();
	}
	
	if (stageOneDeployed && operatorJoystick->GetRawButton(MinibotSecondStageDeployButton))
	{
		DeploySecondStage();
		_arm->GoToUpPosition();
		_pincer->Open();
	}
}

void Minibot::DeployFirstStage()
{
	stageOneDeployed = true;
	firstStageServo->SetAngle(FirstDeployServoAngle);
#if SIMULATOR
		printf("MiniBot: Deploy First Stage\n");
#endif

}

void Minibot::DeploySecondStage()
{
	stageTwoDeployed = true;
	secondStageServo->SetAngle(SecondDeployServoAngle);
#if SIMULATOR
		printf("MiniBot: Deploy Second Stage Out\n");
#endif

}

void Minibot::InitializeServos()
{
	if(!stageOneDeployed)
	{
		firstStageServo->SetAngle(FirstServoAngle);
	}
	if(!stageTwoDeployed)
	{
		secondStageServo->SetAngle(SecondServoAngle);
	}
}
