#ifndef MINIBOT_H_
#define MINIBOT_H_

#include "userincludes.h"
#include "SmartJoystick.h"
#include "Arm.h"
#include "Pincer.h"

class Minibot;

class Minibot
{
	public:
		
		Minibot(Servo *one, Servo *two, SmartJoystick *rjoy, SmartJoystick *opJoy, Arm *arm, Pincer *pincer);
		void PeriodicService();
		
	private:
		void DeployFirstStage();
		void DeploySecondStage();
		void InitializeServos();
		
		bool stageOneDeployed;
		bool stageTwoDeployed;
		
		SmartJoystick *rightJoystick;
		SmartJoystick *operatorJoystick;
		Servo *firstStageServo;
		Servo *secondStageServo;
		Arm *_arm;
		Pincer *_pincer;
		
};

#endif
