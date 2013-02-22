/*MEETING NOTES: 2/5/2011 SOFTWARE, MECH, ELEC
	>Turn Analog IO for Accelerometers to Dig IO.
	>don't but extra strain on motors... basixally dont use to support anything
	>
	*/
#ifndef USERINCLUDES_H_
#define USERINCLUDES_H_

#include "WPILib.h"
#include "Math.h"
#include <list>
#include <map>
#include <string>
#include <time.h>

enum {PWM_PORT_1 = 1,
	  PWM_PORT_2,
	  PWM_PORT_3,
	  PWM_PORT_4,
	  PWM_PORT_5,
	  PWM_PORT_6,
	  PWM_PORT_7,
	  PWM_PORT_8,
	  PWM_PORT_9 };

enum {DIGITAL_PORT_1 = 1,
	  DIGITAL_PORT_2,
	  DIGITAL_PORT_3,
	  DIGITAL_PORT_4,
	  DIGITAL_PORT_5,
	  DIGITAL_PORT_6,
	  DIGITAL_PORT_7,
	  DIGITAL_PORT_8,
	  DIGITAL_PORT_9,
	  DIGITAL_PORT_10,
	  DIGITAL_PORT_11,
	  DIGITAL_PORT_12,
	  DIGITAL_PORT_13,
	  DIGITAL_PORT_14};

enum {ANALOG_PORT_1 = 1,
	  ANALOG_PORT_2,
	  ANALOG_PORT_3,
	  ANALOG_PORT_4,
	  ANALOG_PORT_5,
	  ANALOG_PORT_6,
	  ANALOG_PORT_7,
	  ANALOG_PORT_8};

enum {RELAY_PORT_1 = 1,
	  RELAY_PORT_2,
	  RELAY_PORT_3,
	  RELAY_PORT_4,
	  RELAY_PORT_5,
	  RELAY_PORT_6};

// Global Variables Defined
extern bool IsSpareChassis;  // Used to differentiate MainRobot from SpareChassis, Primarily available JAGS..
#define SIMULATOR 1

// CAN Address Assignments

	// Total of 6 Jaguars used/defined as follows,,,

	// Two Jaguars for Left & Right Drive motors
	const unsigned int CAN_LeftMotorAddress = 6;
	// JAGIO --> Left Encoder connected to Left Motor Jaguar..
	const unsigned int CAN_RightMotorAddress = 1;
	// JAGIO --> Right Encoder connected to Right Motor Jaguar..

	// One Jaguars driving 1 bigger CIM motor
	const unsigned int CAN_ElevatorUpDownAMotorAddress = 2; 
	// JAGIO --> Elevator 'Down'limit Switch connected to Elevator A
	// JAGIOIO --> Elevator Optical Encoder connected to JaguarUpDownA Encoder Input
	
	// More Jaguars for Pincer
	const unsigned int CAN_PincerMotorAddress = 4;
	
		
	//Another Jaguar for Arm rotate.
	const unsigned int CAN_ElevatorArmMotorAddress = 5;
	// JAGIO --> Down limit switch for Elevator Arm to jaguar limit switch
	// ElevatorArm Mag encoder defined below in Analog input seaction
	

// PWMs defined for two servos (two PWMs used total)
	
	static const int PWM_RetroIlluminator = PWM_PORT_4;
	static const int PWM_ChassisCamera = PWM_PORT_5;
	static const int PWM_ElevatorBrakeServo = PWM_PORT_6; 	 // Servo used to stop the elevator movement
	static const int PWM_MinibotFirstStageServo = PWM_PORT_9; 	 
	static const int PWM_MinibotSecondStageServo = PWM_PORT_8; 	 


//Digital I/O Ports (5 total)
	static const int DIO_LeftLightSensorPort = 	 DIGITAL_PORT_2;
	static const int DIO_MiddleLightSensorPort = DIGITAL_PORT_3;
	static const int DIO_RightLightSensorPort =  DIGITAL_PORT_1;
	static const int DIO_ScopeLoopTest =  DIGITAL_PORT_14;
	static const int DIO_SpareChasisJumper =  DIGITAL_PORT_10;
	
	// Relay inputs for 4 spikes 
	static const int RELAY_PincerRoller = 	  RELAY_PORT_5;		// Spike 5 -> For/Rev/Off

	//various Analog Port defines (5 total)
	static const int ANALOG_GyroPort = ANALOG_PORT_1; //Gyro can ONLY be placed on Port 1
	static const int ANALOG_PincerMagneticEncoder = ANALOG_PORT_2;
	static const int ANALOG_ElevatorArmMagneticEncoder = ANALOG_PORT_3;
	
	// Let's define  orientation  motors & encoders...
	// Motor orientation for BOTH CAN and PWM Jaguars
	static const bool IsLeftMotorReversed = true;
	static const bool IsRightMotorReversed = false;
	static const bool IsArmMotorReversed = true;
	static const bool IsPincerMotorReversed = false;

	static const bool IsLeftEncoderReversed = false;
	static const bool IsRightEncoderReversed = true;
	
//USB Ports
	
	static const int USB_LeftJoystickPort = 1;
	static const int USB_RightJoystickPort = 2;
	static const int USB_OperatorJoystickPort = 3;
	
//Joystick Buttons
	
	//Left Joystick
	
	static const int LeftTurboButton = 1;
	static const int TurnToRackButton = 2;
	static const int DriveToPegButton = 3;
	static const int CycleDriverStationDiagsButton = 6;
	static const int SwitchDriveModeButton = 10;
	
	//Right Joystick

	static const int RightTurboButton = 1;   // Yes, two turbo buttons. Press both, the robot goes only at full speed
	static const int MinibotSafetyButton = 2;
	static const int KrakenButton = 3;
	static const int AutoReleaseButton = 6;  //gone
	static const int DriverMessagesTestButton = 9;
	static const int ChassisCameraDownButton = 10;
	static const int ChassisCameraUpButton = 11;
	
	//Operator Joystick
	
	//static const int CameraSwitchButton = 2;
	static const int GoToPositionButton = 1;
	static const int ElevatorToGroundButton = 2;
	//static const int GripRollerOutButton = 3;
	static const int backspinShootButton = 3; //changed for shooter prototyping
	static const int ArmDownButton = 4;
	//static const int GripRollerInButton = 5;
	static const int normalShootButton = 5; //changed for shooter prototyping
	static const int ArmUpButton = 6;
	static const int MinibotFirstStageDeployButton = 7;
	static const int MinibotSecondStageDeployButton = 8;
	static const int ElevatorManualDown = 9;
	static const int ElevatorManualUp = 10;
	static const int GripReleaseButton = 11;
	static const int GripGrabButton = 12;

	static const int ElevatorNextPositionTestButton = 13;	//temp to try manual mode
	static const int ArmNextPositionTestButton = 17;	//Kate, these buttons DNE
	static const int ArmManualDown = ArmDownButton;	//temp to try manual mode
	static const int ArmManualUp = ArmUpButton;	//temp to try manual mode
	
	
//Other Useful(?) Constants
	static const float WaitTime = .05f;
	static const float Pi = 3.14159f;
	static const float WheelCircumferenceFeet = (6.25f/12.0f * Pi);  // 6.25 inches
	static const float AngleStraightHome = 3.0f * Pi / 2.0f;
	static const float AngleStraightAway = Pi / 2.0f;
	static const UINT16 ElevatorPulsesPerFoot = 283;
	static const float nonsensicalAddition = 420;
	static const UINT16 DriveWheelPulsesPerFoot = 600; // found through experimentation
	static const float GettingDownSpeed = 0.25f;//awww yeahhh
	

#define sizeof_array(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0])) 
	

#endif
