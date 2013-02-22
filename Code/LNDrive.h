#ifndef LNDRIVE_H_
#define LNDRIVE_H_

#include "userincludes.h"
#include "SmartJoystick.h"
#include "SmartJagMotor.h"
#include "Navigation.h"
#include "PIDDrive.h"

class LNDrive;
class Encoders1073;
class PIDDrive;

class LNDrive
{
	friend class PIDDrive;
	
	public:
		
		LNDrive(SmartJaguarMotorEncoder *lmj, SmartJaguarMotorEncoder *rmj, SmartJoystick *lj, SmartJoystick *rj, Navigation *n, Encoders1073 *encoders, SmartGyro *g);
		void PeriodicService();
		
		void Override(float leftMotor, float rightMotor);
		void StopOverride();
		
		void StartTurnToAngle(float angle);
		void StartDriveToDistance(float distance, float angle = 0);
		bool IsAllDone();
		bool IsTurnDone();
		//bool UpdateTurnToAngle();
		void StopTurnToAngle();
		
		void DriveToPeg(float heading, float dist);
		bool StatusDriveToPeg();
		//void UpdateDriveToPeg();
		void StopDriveToPeg();
		void DisablePID();		
		void UpdateTargetPole(int xValue) { targetPole = xValue; }

#ifdef  _MSC_VER
	void SimulationHack();
#endif

		
	private:

		void TankDrive();
		void ArcadeDrive();
		void Scale();
		void SetMotors();
		void UpdateTurnToAngle();
		void CheckDriveMode();
		void SetLeft(float l)  { left = l;}
		void SetRight(float r) { right = r;}
		
		SmartJoystick *leftJoystick;
		SmartJoystick *rightJoystick;
		SmartJaguarMotorEncoder *leftMotorJaguar;
		SmartJaguarMotorEncoder *rightMotorJaguar;
		Navigation *navigation;
		Encoders1073 *encoders;
		
		PIDDrive *angleDrive;
		
		float left;
		float right;
		//float desiredAngle;
		//float currentAngle;
		float desiredDistance;
		//float currentDistance;
		
		bool overridden;
		bool turningToAngle;
		bool isTankDrive;
		bool drivingToLoc;
		
		int targetPole;
};

#endif
