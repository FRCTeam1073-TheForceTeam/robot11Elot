#ifndef ROBOT1073_H_
#define ROBOT1073_H_

#include "wpilib.h"
#include "userincludes.h"
#include "LNDrive.h"
#include "Navigation.h"
#include "Minibot.h"
#include "DashboardSender.h"
#include "DashboardReceiver.h"
#include "LineFollower.h"
#include "Encoders1073.h"
#include "SmartJoystick.h"
#include "Pincer.h"
#include "arm.h"
#include "elevator.h"
#include "SmartJagMotor.h"
#include "SmartGyro.h" 
#include "KrakenDrive.h"
#include "MatchTimer.h"
//////////////////////////////////////////////////////////
// Filename: Robot1073.h
// Author:    
// Date:     January 23, 2011
//
// Description: This file contains the main defintion for the Team1073 Robot.
//
//////////////////////////////////////////////////////////
#include "DriverMessages.h"
#include "cameramanager.h"
class Robot1073;
class Robot1073: public SimpleRobot
{
	
	
	public:
		Robot1073();
		
		void Autonomous();
		void OperatorControl();
		void ZombieZonePeriodicService();
		void ProcessDashboardData(char *receivedRequest);


		int GetTargetPole () {return targetPole; }
		int GetTargetFoot () {return targetFoot; }

		void SetTargetPoleAndFoot(int polePos, int footPos);
		
		Victor *retroIlluminator;

		
				
	private:
		// The DashboardSender & DriverMessage classes are read-only functions that need to access a variety of
		// our 'private' data so we'll make them 'friends' to make it much more convenient to access & use..
		friend class DashboardSender;  // Friend #1 to send our telemetry to the dashboard
		friend class DriverMessages;   // Friend # 2 to send our data to the driver station//
		friend class PhantomRobot;        // Allow simulator access
		friend class KrakenDrive; 	   // Needs a ton of classes

		
		DigitalInput *isSpareChassisJumper; // Jumper to determine whether SW is executing on MainBot or Spare Chasis  
		AxisCamera *camera;          
		DriverStation *driverStation;
		
		// Pointers for each of 6 Jaguar Motors
		SmartJaguarMotorEncoder *leftMotorJaguar;
		SmartJaguarMotorEncoder *rightMotorJaguar;
		SmartJaguarMotorEncoder *pincerJaguar;
		SmartJaguarMotorEncoder *armJaguar;
		SmartJaguarMotorEncoder *elevatorJaguarMotorA;
		
		SmartJoystick *leftJoystick;
		SmartJoystick *rightJoystick;
		SmartJoystick *operatorJoystick;
		Encoders1073 *encoders;
		
		Relay *rollerRelay;
		AnalogChannel *magEncoder;
		AnalogChannel *magPincerEncoder;
		
		SmartGyro *gyro;
		Servo *elevatorBrakeServo;
		Servo *cameraServo;
		Servo *minibotFirstStageServo;
		Servo *minibotSecondStageServo;
		Navigation *navigation;
		KrakenDrive *krak;
		LNDrive *drive;
		Minibot *minibot;
		DashboardSender *dashboardSender;
		DashboardReceiver *dashboardReceiver;
		DriverMessages *driverMessages;
		CameraManager *cameraManager;
		DigitalInput *leftLineSensor;
		DigitalInput *middleLineSensor;
		DigitalInput *rightLineSensor;
		LineFollower *lineFollower;
		Pincer *pincer;
		Arm *arm;
		Elevator *elevator;
		KrakenDrive *kraken;
		DigitalOutput *scopeLoop;
		Timer *systemTimer;
		MatchTimer *matchTimer;
		int targetPole;
		int targetFoot;

		void DoPeriodicServiceFunctions();
		void InitializeTheZombieZone(Robot1073 *ptr);
		void InitializeDashboardReceiverThread(Robot1073 *, DashboardReceiver *);
};


#endif

