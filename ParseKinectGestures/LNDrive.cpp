#include "LNDrive.h"

LNDrive::LNDrive(SmartJaguarMotorEncoder *lmj, SmartJaguarMotorEncoder *rmj, SmartJoystick *lj, SmartJoystick *rj, Navigation *n, Encoders1073 *enc, SmartGyro *g)
{
	leftJoystick = lj;
	rightJoystick = rj;
	leftMotorJaguar = lmj;
	rightMotorJaguar = rmj;
	navigation = n;
	angleDrive = new PIDDrive(n, this, g, enc);
	
	left = 0;
	right = 0;
	
	overridden = false;
	turningToAngle = false;
	isTankDrive = true;
	drivingToLoc = false;
	
	targetPole = 3;
}

void LNDrive::PeriodicService()
{
	CheckDriveMode();


#if 0
	
	if(leftJoystick->GetRawButton(DriveToPegButton))
	{
		DriveToPeg(navigation->GetHeadingToBait(targetPole), navigation->GetDistanceToBait(targetPole));
	}
	else
	{
		//StopDriveToPeg();
	}
	
	if (leftJoystick->GetRawButton(TurnToRackButton))
	{
		StartTurnToAngle(AngleStraightHome);
	}
	else
	{
		//StopTurnToAngle();
	}
	
	if (overridden)
	{
		//Left blank deliberately, someone else is in control
	}
	else if (drivingToLoc)
	{
		static bool SwitchedToDriveStraight = false;
		
		if(turningToAngle)
		{
			if(IsAllDone())
			{
				StopTurnToAngle();
			}
		}
		else if (!SwitchedToDriveStraight)
		{
			angleDrive->DriveStraight(.5, desiredDistance);
			angleDrive->Enable(true);
		}
		else if(StatusDriveToPeg())
		{
			StopDriveToPeg();
		}
	}
	else if (turningToAngle)
	{
		if(IsAllDone())
		{
			StopTurnToAngle();
		}
	}
	else
	{
		TankDrive();
	}
#else
	if(!(rightJoystick->GetRawButton(KrakenButton) || rightJoystick->GetRawButton(AutoReleaseButton) || leftJoystick->GetRawButton(DriveToPegButton)))
	{
		TankDrive();
	}
#endif

}

void LNDrive::TankDrive()
{
#if 1
	left = leftJoystick->GetY();
	right = rightJoystick->GetY();
#else
	left = right = leftJoystick->GetZ();
#endif

	if(fabs(left) < .1)
	{
		left = 0;
	}
	
	if(fabs(right) < .1)
	{
		right = 0;
	}
	
	float diff = fabs(left - right);
	
	if(left >= .1 && right >= .1 && diff <= .05)
	{
		left = right = (left > right) ? left : right;
	}
	else if(left <= -.1 && right <= -.1 && diff <= .05)
	{
		left = right = (left < right) ? left : right;
	}
	
	if (leftJoystick->GetRawButton(LeftTurboButton) && left != 0)
	{
		left /= fabs(left);
	}
	if(rightJoystick->GetRawButton(RightTurboButton) && !rightJoystick->GetRawButton(MinibotSafetyButton) && right != 0)
	{
		right /= fabs(right);
	}
	
	// DriverMessages has a special debug mode.  If the test button is held down, don't drive
	bool isTestMode = rightJoystick->GetRawButton(DriverMessagesTestButton) && !rightJoystick->GetRawButton(MinibotSafetyButton);
	if (!isTestMode)
	{
		Scale();
		SetMotors();
	}
}

void LNDrive::ArcadeDrive()
{
	float x = leftJoystick->GetX();
	float y = leftJoystick->GetY();
	
	left = y + x;
	right = y - x;
	
	Scale();
	SetMotors();
}

void LNDrive::Scale()
{
	if(left > 1)
	{
		left = 1;
		right /= left;
	}
	else if (left < -1)
	{
		left = -1;
		right /= -left;
	}
	
	if(right > 1)
	{
		right = 1;
		left /= right;
	}
	else if (right < -1)
	{
		right = -1;
		left /= -right;
	}
}

void LNDrive::SetMotors()
{
	// Set the left & right motor outputs, reversing if necessary...
	leftMotorJaguar->Set(left);			
	rightMotorJaguar->Set(right);
}

void LNDrive::StartTurnToAngle(float angle)
{
	turningToAngle = true;
	
	angleDrive->SetOutputRange(-0.5, 0.5);
	angleDrive->SetTolerance(1.0);
	angleDrive->SetSetpoint(angle);
	angleDrive->TurnAngle(.5, angle);
	angleDrive->Enable(true);
}

void LNDrive::StartDriveToDistance(float speed, float feet)
{
	angleDrive->DriveStraight(speed, feet);
	angleDrive->SetOutputRange(-0.5, 0.5);
	angleDrive->SetTolerance(1.0);
	angleDrive->Enable(true);
}

bool LNDrive::IsTurnDone()
{
	return angleDrive->IsTurnDone();
}

bool LNDrive::IsAllDone()
{
	return angleDrive->IsAllDone();
}

void LNDrive::StopTurnToAngle()
{
	turningToAngle = false;
	angleDrive->Enable(false);
}
/*
void LNDrive::UpdateTurnToAngle()
{	
	currentAngle = navigation->GetHeading();
	
	float angleDif = desiredAngle - currentAngle;
	
	if(fabs(angleDif) < 5)
	{
		left = 0;
		right = 0;
	}

	else if (angleDif > 0 && angleDif > Pi ||
			 angleDif < 0 && -angleDif < Pi)
	{
		left = angleDif / Pi;
		right = -left;
	}
	else
	{
		right = angleDif / Pi;
		left = -right;
	}
	
	if (fabs(angleDif) < Pi / 6 && navigation->GetXVelocity() > .5)
	{
		left *= -1;
		right *= -1;
	}
	
	
	Scale();
	SetMotors();
}
*/
void LNDrive::Override(float leftMotor, float rightMotor)
{
	overridden = true;
	
	left = leftMotor;
	right = rightMotor;
	
	Scale();
	SetMotors();
}

void LNDrive::StopOverride()
{
	overridden = false;
}

void LNDrive::CheckDriveMode()
{
	
}

void LNDrive::DriveToPeg(float heading, float dist)
{	
	desiredDistance = dist;
	StartTurnToAngle(heading);
	drivingToLoc = true;
}

bool LNDrive::StatusDriveToPeg()
{
	return angleDrive->IsDistanceDone();
}
/*
void LNDrive::UpdateDriveToPeg()
{
#if 0
		currentAngle = navigation->WrapRadianAngle(navigation->GetHeading());

	if(!StatusTurnToAngle())
	{
		UpdateTurnToAngle();
	}
	else if (!StatusDriveToPeg())
	{
		float howMuchFarther = desiredDistance - currentDistance;
		float yVelocity = navigation->GetYVelocity();
		
		if(howMuchFarther <= .5 && yVelocity > 1)
		{
			left = -yVelocity;
			right = -yVelocity;
		}
		else if (howMuchFarther <= .5)
		{
			left = .25;
			right = .25;
		}
		else
		{
			left = 1;
			right = 1;
		}
		
		Scale();
		SetMotors();
	}
	else
	{
		StopDriveToPeg();
	}
	

	static float xDist; 
	static float yDist; 
	static float hypotenuseDist;
	
	currentXLoc = navigation->GetX();
	currentYLoc = navigation->GetY();
	
	xDist = desiredXLoc - currentXLoc;
	yDist = desiredYLoc - currentYLoc;
	
	float yVel = navigation->GetYVelocity();
		
	hypotenuseDist = sqrt(pow(xDist, 2) + pow(yDist, 2));
	desiredAngle = navigation->WrapRadianAngle(atan(yDist / xDist));
	currentAngle = navigation->WrapRadianAngle(navigation->GetHeading());
	
	if (fabs(desiredAngle - currentAngle) < Pi / 6 || fabs(desiredAngle - currentAngle) > 11 * Pi / 6)
	{
		UpdateTurnToAngle();
	}
	else if (hypotenuseDist >= .5)
	{
		if(hypotenuseDist >= 5)
		{
			left = 1;
			right = 1;
		}
		else if (hypotenuseDist > 1 && yVel < 1)
		{
			left = .25;
			right = .25;
		}
		else if (yVel < .5)
		{
			left = 0;
			right = 0;
		}
		else
		{
			left = -yVel;
			right = -yVel;
		}
		
		Scale();
		SetMotors();
	}
	else
	{
		if (yVel < .1)
		{
			if (drivingToPeg && fabs(currentAngle - Pi / 2) < Pi / 12)
			{
				desiredAngle = Pi / 2;
				UpdateTurnToAngle();
			}
			else
			{
				left = 0;
				right = 0;
				Scale();
				SetMotors();
				
				StopDriveToLocation();
			}
		}
		else
		{
			left = -yVel;
			right = -yVel;
			
			Scale();
			SetMotors();
		}
	}
#endif
}
*/

void LNDrive::StopDriveToPeg()
{
	drivingToLoc = false;
	angleDrive->Enable(false);
}

void LNDrive::DisablePID()
{ 
	angleDrive->Enable(false); 
}

void LNDrive::StartDanceSpin()
{
	Override( GettingDownSpeed, -GettingDownSpeed);
}

void LNDrive::StopDanceSpin()
{
	StopOverride();
}

#ifdef  _MSC_VER
void 
LNDrive::SimulationHack()
{ 	angleDrive->SimulationHack(); 
}
#endif
