#ifndef KRAKENDRIVE_H_
#define KRAKENDRIVE_H_
//////////////////////////////////////////////////////////
// Filename: KrakenDrive.h
// Author:   Chaaaales and maybe Bitwise ||
// Date:     January 23, 2011
//
// Description: RELEASE THE KRAKEN. YES. THIS IS THE KRAKEN.
////////////////////////////////////////////////////////////

#include "WPILib.h"
#include "Elevator.h"
#include "Pincer.h"
#include "Navigation.h"
#include "LNDrive.h"
#include "SmartJoystick.h"
#include "Arm.h"
#include <stdio.h>
#include "Robot1073.h"

class KrakenDrive
{
	
public:
	KrakenDrive(Robot1073 *pRobot);
	enum TheKrakenDrive { TurnToBait, DriveToBait, TurnToPeg, RaiseElevator, DriveToPeg, WaitForPeg, 
		AutoRelease, DropTheArm ,OpenPincer, DropOffTube, BackUp, DropElevator, TurnAround, DriveBack, TheLastWait,
		/*TurnAndDrive, TurnDriveDrop, ReleasePincer, DriveBack,*/ 
		LineFollow1, LineFollow2, StopEverything, IdleMode };
	void PeriodicService();
	TheKrakenDrive GetMode() { return theKrakenDrive; }
	void StopAll();
	
	void UpdateTargetPole (int xValue) { targetPole = xValue; }
	void UpdateTargetFoot (int yValue) { targetFoot = yValue; }

	void SetAutoReleaseButton(bool button) { isAutoReleaseButtonHeld = button; }
	
private:
	TheKrakenDrive theKrakenDrive;
	Robot1073 *robot;

	bool isAutoReleaseButtonHeld;
    int targetPole;
    int targetFoot;
};

#endif
