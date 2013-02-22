#include "LineFollower.h"
#include "SmartJoystick.h"
#include "SmartGyro.h"
#include "Robot1073.h"

const float FollowTapeConstant = 17.329335; //14.371;
const float lineLength = 17.329335;

LineFollower::LineFollower(LNDrive *d, SmartJoystick *ljoy, SmartJoystick *rjoy, DigitalInput *llsensor,  DigitalInput *mlsensor, DigitalInput *rlsensor, SmartJaguarMotorEncoder *ljag, Navigation *navigation)
{
	drive = d;
	leftJoystick = ljoy;
	rightJoystick = rjoy;
	leftLineSensor = llsensor;
	middleLineSensor = mlsensor;
	rightLineSensor = rlsensor;
	_navigation = navigation;
	leftJag = ljag;
	done = false; 
	targetPole = 2;
	retroTape = false;
}
void LineFollower::DoTracking()
{
	if(retroTape)
	{
		FollowTape();
	}
	else
	{
		FollowLine();
	}
}
void LineFollower::FollowTape()
{
	const float maxSpeed = 0.4f;
	const float correctionFactor = .015f;
	float y = 54 - _navigation->GetY();
	
	double position = leftJag->GetPosition();
	
	if(position < FollowTapeConstant)
	{	
		drive->Override(maxSpeed + correctionFactor * xOff / y, maxSpeed - correctionFactor * xOff / y);
	}
	else
	{
		done = true;
		drive->Override(0,0);
	}
}
void LineFollower::SetXOff(float xoff)
{
	xOff = xoff;
}
void LineFollower::StartInAutonomous()
{
	inAutonomous = true;
}

void LineFollower::EndInAutonomous()
{
	inAutonomous = false;
}

void LineFollower::FollowLine(void)
{	
	const float maxSpeed = 0.5f;
	double position = leftJag->GetPosition();
	if (!done)
	{
		leftIsOn = leftLineSensor->Get() != 0;
		middleIsOn = middleLineSensor->Get() != 0;
		rightIsOn = rightLineSensor->Get() != 0; 

#if 1	// Simple Line Following IS working, need to debug remaining & more complex logic...
		
		if(position < lineLength  && !done){
			if (middleIsOn && !leftIsOn && !rightIsOn)
				drive->Override(maxSpeed, maxSpeed);
			else if (leftIsOn && middleIsOn && !rightIsOn)
				drive->Override(maxSpeed / 2, maxSpeed);
			else if (rightIsOn && middleIsOn && !leftIsOn)
				drive->Override(maxSpeed, maxSpeed / 2);
			else if (leftIsOn && !middleIsOn && !rightIsOn)
				drive->Override(maxSpeed / 3, maxSpeed);
			else if (leftIsOn && rightIsOn && !middleIsOn)
				drive->Override(maxSpeed / 2, maxSpeed);
			else if (rightIsOn && !middleIsOn && !leftIsOn)
				drive->Override(maxSpeed, maxSpeed / 3);
			else if (leftIsOn && middleIsOn && rightIsOn)
				drive->Override(maxSpeed / 2, maxSpeed / 2);
		}
			else{
				drive->Override(0,0);
				done = true;
		}
		

#else		
		if (!done && position > 9.90 && position < 10.10 && targetPole == 3)
		{
			drive->Override(maxSpeed / 2, maxSpeed);
		}

		if (!done && position > 9.90 && position < 10.10 && targetPole == 4)
		{
			drive->Override(maxSpeed, maxSpeed / 2);
		}
		if (middleIsOn && !leftIsOn && !rightIsOn)
		{
			drive->Override(maxSpeed, maxSpeed);
		}
		else if (leftIsOn && middleIsOn && !rightIsOn)
		{
			drive->Override(maxSpeed / 2, maxSpeed);
		}
		else if (rightIsOn && middleIsOn && !leftIsOn)
		{
			drive->Override(maxSpeed, maxSpeed / 2);
		}
		else if (leftIsOn && !middleIsOn && !rightIsOn)
		{
			drive->Override(maxSpeed / 3, maxSpeed);
		}
		else if (leftIsOn && rightIsOn && !middleIsOn)
		{
			drive->Override(maxSpeed / 2, maxSpeed);
		}
		else if (rightIsOn && !middleIsOn && !leftIsOn)
		{
			drive->Override(maxSpeed, maxSpeed / 3);
		}
		else if (leftIsOn && middleIsOn && rightIsOn)
		{
			drive->Override(maxSpeed / 2, maxSpeed / 2);
				
		} 
		/* else if (!leftIsOn && !middleIsOn && !rightIsOn && position > 6.5 && position < 7.0)
		{
			drive->Override(0, 0);
			printf("Off the line\n");
		} */
		else if (!leftIsOn && !middleIsOn && !rightIsOn && position < 9.90 || position > 10.10)
		{
			drive->Override(maxSpeed / 2, maxSpeed / 2);
		}
		else if ((targetPole == 3 || targetPole == 4) && position > 15.22)	
		{
			done = true;
			drive->Override(0,0);
		}
		else if ((targetPole == 2 || targetPole == 5) && position > lineLength)
		{
			done = true;
			drive->Override(0, 0);
		}
#endif
		
		//printf("l:%d m:%d r:%d position:%f\n", leftIsOn, middleIsOn, rightIsOn, position);
	}
	else
	{
		done = true;
		drive->Override(0, 0);
	}
		
}

	

