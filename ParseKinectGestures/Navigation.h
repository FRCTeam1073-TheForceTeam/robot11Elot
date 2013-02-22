#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include "userincludes.h"
#include "encoders1073.h"
#include "POINTF.h"
#include "math.h"

class Navigation;
class Encoders1073;
class SmartGyro;

class Navigation
{
	POINTF XYKrakenBaits[6];
	POINTF XYKrakenPeg[6];
	POINTF XYStartPos[6];
	float robotLength;
	float robotWidth;
	bool hasStarted;

	public:
		Navigation(Encoders1073 *enc, SmartGyro *g);
		void Start();
		bool GetHasStarted();
		float GetX();
		float GetY();
		float GetXVelocity();
		float GetYVelocity();
		float GetXVar();
		float GetYVar();
		float GetHeadingVar();
		float GetSX();
		float GetSY();
		float GetHeading();
		float GetDistanceToBait(int bait);
		float GetHeadingToBait(int bait);
		float GetDistanceToPeg(int peg);
		float GetHeadingToPeg(int peg);
		void PeriodicService();
		float ToRadians(float degrees);
		float ToDegrees(float radians);
		void SetX(float xin);
		void SetY(float yin);
		void SetHeading(float headingin);
		void UpdateNavData(float xExp, float yExp, float headingExp, float xVarExp, float yVarExp, float headingVarExp);
		float WrapRadianAngle(float angle);
		void VisionDataReceived(char *str);
		void StartPositionRobotToColumn(int pole);
		void SetStartPosition();

	private:
		
		//PositionArmHeight(); (part of object handling).
		/// initiates the robot positioning sequence (and carries through)
		
		/// return: boolean, positioned (t) or not (f). 
		bool StatusPositionRobotToColumn();
		/// stop the robot positioning (when held button is let up??)
		void StopPositionRobotToColumn();
		/// x, y, std dev x, std dev y, heading. 
		/// in a vertical orientation of the field, the left corner 
		/// when facing the targets (as intial robot position) is 0,0.
		/// down/right are both positive. units are in feet.
		

		void Combine(float newDatum, float newVar, float *oldDatum, float *oldVar);
		void AngleCombine(float newDatum, float newVar, float *oldDatum, float *oldVar);


		Encoders1073 *encoders;
		SmartGyro *gyro;
		Timer *timer;
		
		float x; //feet
		float y; //feet
		float xVelocity;
		float yVelocity;
		float heading; //radians
		float prevEncoderDist;
		float prevangleRadians;
		float xVar; //feet^2
		float yVar; //feet^2
		float headingVar; //radians^2
		int targetPole; 
		
};

#endif
