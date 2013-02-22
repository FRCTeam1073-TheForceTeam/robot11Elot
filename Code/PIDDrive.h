#ifndef _PIDDRIVE_H
#define _PIDDRIVE_H

#include "WPILib.h"
#include "Navigation.h"
#include "LNDrive.h"
#include "userincludes.h"
#include "math.h"

class LNDrive;	// Needed for forward declaration

class PIDDrive :public PIDOutput
{
	friend class LNDrive;
	
  public:

	PIDDrive(Navigation *e, LNDrive *t, SmartGyro *gyro, Encoders1073 *l);
	
	virtual ~PIDDrive();
	
	PIDController *GetPID();
	
	void SetOutputRange(float min, float max);
	void SetTolerance(float tolerance);
	void SetSetpoint(float value);
	void SetSpeedOffset(float speed);
	void Enable(bool enable);

	
	void PIDWrite(float output);
	void TurnAngle(float turnDifferentialspeed, float angle);
	void TurnAngleWithSpeed(float turnDifferentialspeed, float angle, float speed);

	void DriveStraight(float speed, float distanceFeet);
	float GetDistanceFeet();
	
	bool IsTurnDone();
	bool IsDistanceDone();
	bool IsAllDone();
	
#ifdef  _MSC_VER
	void SimulationHack();
#endif
  private:
	  void zeroDistance();
	
	  float startDistanceFeet;
	  float targetDistanceFeet;
	  
	  PIDController *anglePID;
	  Navigation *navigation;
	  LNDrive *drive;
	  SmartGyro *smartGyro; 
	  Encoders1073 *encoders;
	  
	  float anglePrecision;
	  float distancePrecision;
	  
	  float maxTurningSpeed;
	  float maxDrivingSpeed;
	  float speedOffset;
	  bool distancePID;
};
	
	
#endif
