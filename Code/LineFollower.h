#ifndef LINEFOLLOWER_H_
#define LINEFOLLOWER_H_

#include "userincludes.h"
#include "LNDrive.h"
#include "Encoders1073.h"
#include "SmartJoystick.h"

class LineFollower;

class LineFollower
{
	public:
		
		LineFollower(LNDrive *d, SmartJoystick *ljoy, SmartJoystick *rjoy, DigitalInput *llsensor,  DigitalInput *mlsensor, DigitalInput *rlsensor, SmartJaguarMotorEncoder *leftMotorJaguar, Navigation *navigation);
		void StartInAutonomous();
		void EndInAutonomous();
		void FollowLine();
		bool IsDone() { return done; }
		void FollowTape();
		void DoTracking();
		void UpdateTargetPole (int xValue) { targetPole = xValue; }
		void SetXOff(float xoff);
		void UpdateTrackingMethod(bool retroTapeParam) { retroTape = retroTapeParam; } 
		bool retroTape;
		
	private:
		LNDrive *drive; // robot drive system
		SmartJoystick *leftJoystick;
		SmartJoystick *rightJoystick;
		DigitalInput *leftLineSensor;
		DigitalInput *middleLineSensor;
		DigitalInput *rightLineSensor;
		SmartJaguarMotorEncoder *leftJag;
		Navigation *_navigation;
		
		float xOff;
		
		bool inAutonomous;
		bool done;
		bool leftIsOn;
		bool rightIsOn;
		bool middleIsOn;
		bool fork;

		
		int targetPole;
};

#endif
