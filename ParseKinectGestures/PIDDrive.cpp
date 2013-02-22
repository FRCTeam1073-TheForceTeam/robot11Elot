#include <stdio.h>
#include "PIDDrive.h"

PIDDrive::PIDDrive(Navigation *n, LNDrive *t, SmartGyro *g, Encoders1073 *l)
: navigation(n), drive(t), smartGyro(g), encoders(l)
{
	anglePID = new PIDController(0.05f, 0.00f, 0.2f, smartGyro, this);
	distancePID = false;
	speedOffset = 0.0f;

	startDistanceFeet =0.0f;
	targetDistanceFeet=0.0f;

	anglePrecision = 5.0f;
	distancePrecision = 0.5f;
	  
	maxTurningSpeed = 1.0f;
	maxDrivingSpeed = 1.0f;


}

PIDDrive::~PIDDrive()
{
	
}

PIDController *
PIDDrive::GetPID()
{
	return anglePID;  /* anglePID; */
}
void 
PIDDrive::PIDWrite(float output)
{
	static float prevErr = 0.0f;
	static float iErr = 0.0f;
	const float kpDrive = 1.8f;
	const float kiDrive = 0.0f;
	const float kdDrive = 9.0f;
	float speed;
	float err =0;


	if (distancePID) {
		float curDistance = GetDistanceFeet();
		err = targetDistanceFeet - curDistance;
//		printf("err: %f", err);
		iErr +=err;
		
		if(fabs(iErr) > 1)
		{
			iErr /= fabs(iErr);
		}
	
		speed = kpDrive * err;
		speed += iErr* kiDrive;
		speed += kdDrive * (err - prevErr);
		
		if(speed > maxDrivingSpeed)
		{
			speed = maxDrivingSpeed;
		}
		else if(speed < -maxDrivingSpeed)
		{
			speed = -maxDrivingSpeed;
		}
		
//		printf("Speed: %f", speed);
		prevErr = err;
	}
	else
	{
		speed = speedOffset;
	}
	
	//printf("s=%4.2f o=%4.2f, dist = %ddisterr = %f\n",speed,output, distancePID, err);

	float left = speed + output;
	float right = speed - output;
	drive->SetLeft(left);
	drive->SetRight(right);
	
	drive->Scale();
	drive->SetMotors();
}

bool
PIDDrive::IsTurnDone()
{
	static int countsAtPosition = 0;

	float err = fabs(anglePID->GetError());
	//printf("err = %f, anglePrecision = %f\n", err, anglePrecision);
	if(err < anglePrecision){
		if(++countsAtPosition > 7){
			return true;
		}
	}
	else
		countsAtPosition = 0;

	return false;
	
}


bool
PIDDrive::IsDistanceDone()
{
	float curDistance = GetDistanceFeet();
	float err = fabs(targetDistanceFeet - curDistance);

	
	if(err < distancePrecision)
		return true;
	return false;
	
}

bool
PIDDrive::IsAllDone()
{
	return (IsTurnDone() && IsDistanceDone());
	
}


void PIDDrive::TurnAngle(float maxTurnSpeed, float angle)
{
	maxTurningSpeed = maxTurnSpeed;
	
	anglePID->SetOutputRange(-maxTurnSpeed, maxTurnSpeed);
	anglePID->SetTolerance(2.0f);
	anglePID->SetSetpoint(angle);
	anglePID->Enable();
	targetDistanceFeet = 0.0f;
	distancePID = false;
}
void PIDDrive::TurnAngleWithSpeed(float maxTurnSpeed, float angle, float speed)
{
	maxTurningSpeed = maxTurnSpeed;
	
	anglePID->SetOutputRange(-maxTurnSpeed, maxTurnSpeed);
	anglePID->SetTolerance(2.0f);
	anglePID->SetSetpoint(angle);
	anglePID->Enable();
	speedOffset = speed;
	distancePID = false;
}


void 
PIDDrive::DriveStraight(float speed, float distanceFeet)
{
	//We want to drive straight
	TurnAngle(0.5f, smartGyro->GetAngle());  // Set for 0.00 degree angle PID
	maxDrivingSpeed = speed;
	zeroDistance();
	targetDistanceFeet = distanceFeet;
	distancePrecision = 0.1f;
	distancePID = true;
}

void
PIDDrive::SetOutputRange(float min, float max)
{
	anglePID->SetOutputRange(min, max);
}

void
PIDDrive::SetTolerance(float tolerance)
{
	anglePrecision = tolerance;
	anglePID->SetTolerance(tolerance);
}

void
PIDDrive::SetSetpoint(float angle)
{
	anglePID->SetSetpoint(angle);
}

void
PIDDrive::Enable(bool enable)
{
	enable ? anglePID->Enable() : anglePID->Disable();
	//distancePID = enable;
}

void
PIDDrive::zeroDistance()
{
	startDistanceFeet = (float)(encoders->GetDistance().first + encoders->GetDistance().second) / 2.0f;
}

float
PIDDrive::GetDistanceFeet()
{
	float currentDistance = (float)(encoders->GetDistance().first + encoders->GetDistance().second) / 2.0f;
	
	return (currentDistance - startDistanceFeet);
}

#ifdef  _MSC_VER
void 
PIDDrive::SimulationHack()
{ 
	anglePID->Calculate(); 
}
#endif
