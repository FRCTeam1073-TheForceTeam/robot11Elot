#include "KrakenDrive.h"
#include "LNDrive.h"
#include <stdio.h>
#include "WPILib.h"
#include "userincludes.h"
#include "math.h"
#include "Elevator.h"
#include "Pincer.h"
#include "Navigation.h"
#include "SmartJoystick.h"
#include "Arm.h"



KrakenDrive::KrakenDrive(Robot1073 *p)
:robot(p)
{
	theKrakenDrive = TurnToBait;
	//theKrakenDrive = TurnToBait;
	targetPole = 2;
	targetFoot = 3;
	isAutoReleaseButtonHeld = false;
}

void KrakenDrive::StopAll()
{
	robot->drive->DisablePID();
}

void KrakenDrive::PeriodicService()
{
	const bool ChooseToLineFollow = true;

	bool isKrakenButtonHeld = robot->rightJoystick->GetRawButton(KrakenButton);
	bool isDriveToPegButtonHeld = robot->leftJoystick->GetRawButton(DriveToPegButton);
	//bool isTurnToRackButtonHeld = robot->leftJoystick->GetRawButton(TurnToRackButton);
	//bool isAutoReleaseButtonHeld = robot->rightJoystick->GetRawButton(AutoReleaseButton);

	static bool wasKrakenButtonHeld = false;
	static bool wasDriveToPegButtonHeld = false;
	//static bool wasTurnToRackButtonHeld = false;
	static bool wasAutoReleaseButtonHeld = false;
	static bool SetStartAutonomous = false;
	
	float backDistance = 0.0f;
	float nextDistance = 0.0f;
	float nextAngle = 0.0;

	if(ChooseToLineFollow && !SetStartAutonomous && robot->IsAutonomous())
	{
		printf("Kraken: Decided to line follow.\n");
		theKrakenDrive = LineFollow1;
		SetStartAutonomous = true;
	}
	else if ((isKrakenButtonHeld && !wasKrakenButtonHeld) || (isDriveToPegButtonHeld && !wasDriveToPegButtonHeld))
	{
		theKrakenDrive = TurnToBait;
		printf("The Kraken lives!\n");
	}
	/*
	else if (isTurnToRackButtonHeld && !wasTurnToRackButtonHeld)
	{
		theKrakenDrive = TurnToPeg;
	}
	*/
	else if (isAutoReleaseButtonHeld && !wasAutoReleaseButtonHeld)
	{
		theKrakenDrive = AutoRelease;
	}
	else if (!(isKrakenButtonHeld || isDriveToPegButtonHeld || robot->IsAutonomous() || theKrakenDrive == IdleMode))
	{
		theKrakenDrive = StopEverything;
	}

	switch(theKrakenDrive)
	{
	case IdleMode:
		break;

	case TurnToBait:
		nextAngle = robot->navigation->GetHeadingToBait(targetPole);
		
		printf("Kraken: Turning to Bait (%fdeg)\nCurrentAngle: %fdeg", nextAngle, robot->gyro->GetAngle());
		//printf("Desired Angle: %f\n", nextAngle);
		//printf("CurrentAngle: %f\n", robot->gyro->GetAngle());
		
		robot->drive->StartTurnToAngle(nextAngle);
		theKrakenDrive = DriveToBait;
		break;

	case DriveToBait:
		if(robot->drive->IsTurnDone())
		{
			nextDistance = robot->navigation->GetDistanceToBait(targetPole);
			printf("Kraken: Driving to bait (%fft)\n", nextDistance);
			theKrakenDrive = TurnToPeg;
			robot->drive->StartDriveToDistance(0.5, nextDistance);
		}
		break;
		
	case TurnToPeg:
		if(robot->drive->IsAllDone())
		{
			nextAngle = robot->navigation->GetHeadingToPeg(targetPole);
			printf("Kraken: Turning to Peg (%f deg)\n", nextAngle);

			robot->drive->StartTurnToAngle(nextAngle);
			
			//if (isTurnToRackButtonHeld)
			//{
			//	theKrakenDrive = TheLastWait;
			//}
			//else
			//{
				theKrakenDrive = RaiseElevator;
			//}
		}
		break;

	case RaiseElevator:
		if(robot->drive->IsTurnDone())
		{
			printf("Kraken: Moving Elevator to foot %d.\n", targetFoot);
			robot->elevator->UpdateTargetPoleAndFoot(targetPole, targetFoot); 
			robot->elevator->GoToPositionIndex();
			theKrakenDrive = DriveToPeg;
		}
		break;

	case DriveToPeg:
		if(robot->elevator->IsAtTargetPosition())
		{
			nextDistance = robot->navigation->GetDistanceToPeg(targetPole);
			printf("Kraken: Driving to peg (%fft)\n", nextDistance);
			robot->drive->StartDriveToDistance(0.5,nextDistance);
			theKrakenDrive = WaitForPeg;
		}
		break;
	
	
	case WaitForPeg:
		if(robot->drive->IsAllDone())
		{
			printf("Kraken: Waiting for Peg\n");
			
			if(!isDriveToPegButtonHeld)
			{
				theKrakenDrive = AutoRelease;
			}
			else
			{
				theKrakenDrive = StopEverything;
			}
		}									
		break;

	// JFH This function is probably the AUTO Release sequence...
	case AutoRelease:
			printf("Kraken: AutoRelease\n");
			robot->arm->GoToReleasePosition();
			theKrakenDrive = OpenPincer;	// JFH  Jon Could. possibly should, consolidate
   									        // arm drop, pincer open, and roller on movements...
		break;
	
	case OpenPincer:
		if(robot->arm->IsAtReleasePosition())
		{
			printf("Kraken: Opening Pincer\n");
			robot->pincer->Open();
			theKrakenDrive = DropOffTube;
		}
		break;

	case DropOffTube:
		if(robot->pincer->IsPincerOpen())
		{
			printf("Kraken: Roller Out & Elevator Down\n");
			robot->pincer->RollOut();
			robot->elevator->DropDownHalfAPeg();
			theKrakenDrive = BackUp;
		}
		break;

	case BackUp:  // JFH Backup ?  Should probably be absolute move ?
		if(robot->elevator->IsAtTargetPosition())
		{
			backDistance = -5; // robot->navigation->GetDistanceToBait(targetPole);
			printf("Kraken: Backing up %f\n", backDistance);
			robot->drive->StartDriveToDistance(0.5, backDistance);
			theKrakenDrive = DropElevator;
		}
		break;

	case DropElevator:  // JFH Backup ?  Should probably be absolute move ?
		if(robot->drive->IsAllDone())
		{
			robot->elevator->GoToPositionFeet(0);
			theKrakenDrive = TurnAround;
		}
		break;


	case TurnAround:
		if(robot->elevator->IsAtTargetPosition())
		{
			printf("Turning Around\n");
			robot->pincer->Close();
			robot->pincer->RollOff();
			robot->arm->GoToUpPosition();
			robot->drive->StartTurnToAngle(robot->navigation->GetHeadingToPeg(targetPole) + 180.0);
			theKrakenDrive = DriveBack;
		}
		break;

	case DriveBack:
		if(robot->drive->IsTurnDone())
		{
			robot->drive->StartDriveToDistance(0.5, 3.0);
			robot->elevator->SetControlMode(Elevator::HomingMode);
			theKrakenDrive = TheLastWait;
		}
		break;

	case TheLastWait:
		if(robot->drive->IsAllDone())
		{
			theKrakenDrive = StopEverything;
		}
		break;

	case StopEverything:
		robot->drive->DisablePID();
		
		theKrakenDrive = IdleMode;
		break;
	
	case LineFollow1:
		printf("Kraken: Following Line\n");
		robot->lineFollower->DoTracking();
		robot->elevator->UpdateTargetPoleAndFoot(targetPole, targetFoot); 
		robot->elevator->GoToPositionIndex();
		
		if(robot->lineFollower->IsDone())
		{
			printf("Kraken: Done Following\n");
			theKrakenDrive = AutoRelease;
		}
		break;

	case LineFollow2:
		robot->lineFollower->DoTracking();
		if(robot->lineFollower->IsDone()){
			theKrakenDrive = AutoRelease;
		}

		break;


	#if 0
		case TurnAndDrive:
			robot->drive->StartTurnToAngle(robot->navigation->GetHeadingToBait(baitnumber));
			if(robot->navigation->GetHeading() == robot->navigation->GetHeadingToBait(baitnumber))
			{
			theKrakenDrive = TurnDriveDrop;
			break;
			}
		case TurnDriveDrop:
			robot->drive->DriveToPeg(robot->navigation->GetHeadingToPeg(baitnumber), robot->navigation->GetDistanceToPeg(baitnumber));
			if(robot->elevator->IsAtTargetPosition())
			{
			theKrakenDrive = ReleasePincer;
			break;
			}
		case ReleasePincer:
			robot->arm->GoToReleasePosition();  // JFH Odd ?
			robot->pincer->Open();
			if(robot->pincer->IsPincerOpen() && robot->arm->IsAtTargetPosition())
			{	
				robot->pincer->RollOff();
				robot->elevator->GoToPositionFeet(8.489583f);  // JFH ODD ?
			}
			if(robot->elevator->GetCurrentPositionFeet() > 8.3f && robot->elevator->GetCurrentPositionFeet() < 8.6f)  // JFH Odd ?
			{
				//robot->arm->GoToDownPosition();
				theKrakenDrive = DriveBack;
				break;
			}
		case DriveBack:
			robot->pincer->Close();
			robot->elevator->GoToPositionFeet(0);
			if(robot->pincer->IsPincerClosed())
			{
			break;
			}
	#endif
	
	}

	wasKrakenButtonHeld = isKrakenButtonHeld;
	wasDriveToPegButtonHeld = isDriveToPegButtonHeld;
	wasAutoReleaseButtonHeld = isAutoReleaseButtonHeld;
}



