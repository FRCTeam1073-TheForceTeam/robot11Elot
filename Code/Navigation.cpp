/////////////////////////////////////////////////////////////////////
//File name:     Navigation.cpp                                   ///
//Date created:  Unknown                                          ///
//Last modified: 2/16/11                                          ///
//Comments:      This is a navagation file                        ///
/////////////////////////////////////////////////////////////////////

#include "Navigation.h"
#include "SmartGyro.h"


static const float MinX = 0;
static const float MaxX = 27;
static const float MinY = 0;
static const float MaxY = 54;

Navigation::Navigation(Encoders1073 *enc, SmartGyro *g)
{
	encoders = enc;
	gyro = g;
	hasStarted = false;
	
	timer = new Timer();
	timer->Reset();
	timer->Start();
	
	//temporary (will be set properly in StartPositionRobotToColumn.) 
	
	xVar = 25.0f;
	yVar = 4.0f;
	headingVar = 0.09f;
	heading = -Pi / 2;
	xVelocity = 0;
	yVelocity = 0;
	prevEncoderDist = 0.5f * (float)(encoders->GetDistance().first+encoders->GetDistance().second);
	float angle = gyro->GetAngle();//degrees, clockwise rotation is positive
	prevangleRadians = ToRadians(angle);
	targetPole = 2;

	float krakenBaitToTarget = 4.0f; // 4 feet back from the rack, for the bait
	// left most
	XYKrakenBaits[0].x = 5.75f; // hard-coded x-values for the racks.... field.h for the simulator has logic behind these numbers...
	XYKrakenBaits[0].y = krakenBaitToTarget;
	// left most + 1
	XYKrakenBaits[1].x = 8.25f;
	XYKrakenBaits[1].y = krakenBaitToTarget;
	// left most + 2
	XYKrakenBaits[2].x = 10.75f;
	XYKrakenBaits[2].y = krakenBaitToTarget;
	// right most - 2
	XYKrakenBaits[3].x = (95.0f/6.0f);
	XYKrakenBaits[3].y = krakenBaitToTarget;
	// right most - 1
	XYKrakenBaits[4].x = (110.0f/6.0f);
	XYKrakenBaits[4].y = krakenBaitToTarget;
	// right most
	XYKrakenBaits[5].x = (125.0f/6.0f);
	XYKrakenBaits[5].y = krakenBaitToTarget;
	
	float krakenPegToTarget = 2.5f; // the peg sticks out, beyond (0,0)
									//changed due to kraken consistently overshooting
	// left most
	XYKrakenPeg[0].x = 5.75f; // hard-coded x-values for the racks.... field.h for the simulator has logic behind these numbers...
	XYKrakenPeg[0].y = krakenPegToTarget;
	// left most + 1
	XYKrakenPeg[1].x = 8.25f;
	XYKrakenPeg[1].y = krakenPegToTarget;
	// left most + 2
	XYKrakenPeg[2].x = 10.75f;
	XYKrakenPeg[2].y = krakenPegToTarget;
	// right most - 2
	XYKrakenPeg[3].x = (95.0f/6.0f);
	XYKrakenPeg[3].y = krakenPegToTarget;
	// right most - 1
	XYKrakenPeg[4].x = (110.0f/6.0f);
	XYKrakenPeg[4].y = krakenPegToTarget;
	// right most
	XYKrakenPeg[5].x = (125.0f/6.0f);
	XYKrakenPeg[5].y = krakenPegToTarget;

	robotLength = 43.25f / 12.0f;
	robotWidth = 34.0f / 12.0f;

	for(int i = 0; i < 6; i++){
		XYStartPos[i].x = XYKrakenPeg[i].x;
		XYStartPos[i].y = 18.03f + robotLength / 2.0f;
	}
	x = XYStartPos[targetPole-1].x;
	y = XYStartPos[targetPole-1].y;
}

void Navigation::Start()
{
	hasStarted = true;
}

bool Navigation::GetHasStarted()
{
	return hasStarted;
}

void Navigation::SetX(float xin)
{
	x = xin;
}
void Navigation::SetY(float yin)
{
	y = yin;
}
void Navigation::SetHeading(float headingin)
{
	heading = headingin;
}

float Navigation::GetX()
{
	return x;
}
float Navigation::GetY()
{
	return y;
}

float Navigation::GetHeading()
{
	return heading;
}

float Navigation::GetXVar()
{
	return xVar;
}

float Navigation::GetYVar()
{
	return yVar;
}

float Navigation::GetHeadingVar()
{
	return headingVar;
}

float Navigation::GetXVelocity()
{
	return xVelocity;
}

float Navigation::GetYVelocity()
{
	return yVelocity;
}

#define GYROVAR 0.05f  //TODO: measure
#define ENCODERVAR (0.05f) //test
void Navigation::PeriodicService()
{
	static float lastTime = 0;
	float currentTime;
	
	currentTime = (float)timer->Get();
	float dt = currentTime - lastTime;
	
	float angle = gyro->GetAngle();					//degrees, clockwise rotation is positive
	float angleRadians = ToRadians(angle);
	float anglediff = angleRadians - prevangleRadians;
	
	xVelocity = anglediff * (37.0f / 12.0f) / dt;

	heading = heading - anglediff;
	prevangleRadians = angleRadians;

	//std::pair<double, double> distPair = encoders->GetDistance();
	//printf("lrdist: %f %f\n",distPair.first,distPair.second);

	float dist = 0.5f * (float)(encoders->GetDistance().first+encoders->GetDistance().second);
	float distdiff = dist - prevEncoderDist;
	prevEncoderDist=dist;

	//printf ("heading = %f angle = %f angleRadians = %f dist = %f distdiff = %f deltatime(ms) =%f\n", heading, angle, angleRadians, dist, distdiff, dt);

	float dx = distdiff * cos(heading);
	float dy = distdiff * sin(heading);
	
	x = x+dx;
	y = y+dy;
	
	if(x > MaxX)
	{
		x = MaxX;
	}
	else if(x < MinX)
	{
		x = MinX;
	}
	
	if(y > MaxY)
	{
		y = MaxY;
	}
	else if(y < MinY)
	{
		y = MinY;
	}

	headingVar += dt * GYROVAR;
	float xyHeadingVar = distdiff * distdiff;
	if (headingVar < (Pi/2.0f)*(Pi/2.0f)) {  // have some heading information
	    float mul=sin(sqrt(headingVar));  // the closer headingVariance is to zero, the less
                                                   // uncertainty in x, y position after moving
	    xyHeadingVar *= mul*mul;
	}

	xVar += ENCODERVAR * dx * dx + xyHeadingVar;  // simple for now
	yVar += ENCODERVAR * dy * dy + xyHeadingVar;  // simple for now
	//printf("dx=%f dy=%f xVar=%f yVar=%f headingVar=%f\n",dx,dy,xVar,yVar,headingVar);
	
	yVelocity = distdiff / dt;
	lastTime = currentTime;

}

float Navigation::WrapRadianAngle(float angle) 
{
	int i = (int)(floor(angle * (0.5f/Pi) + 0.5f));
	angle -= i*2.0f*Pi;
	return (angle);
}

void Navigation::Combine(float newDatum, float newVar, float *oldDatum, float *oldVar) {
	float oldWeight = newVar / (*oldVar + newVar);
	float newWeight = *oldVar / (*oldVar + newVar);

	*oldDatum = oldWeight * *oldDatum + newWeight * newDatum;
	*oldVar = oldWeight * oldWeight * *oldVar + newWeight * newWeight * newVar;

}

void Navigation::AngleCombine(float newDatum, float newVar, float *oldDatum, float *oldVar) {

	float diff = WrapRadianAngle(newDatum - *oldDatum);
	newDatum = *oldDatum + diff;

	float oldWeight = newVar / (*oldVar + newVar);
	float newWeight = *oldVar / (*oldVar + newVar);

	*oldDatum = WrapRadianAngle(oldWeight * *oldDatum + newWeight * newDatum);
	*oldVar = oldWeight * oldWeight * *oldVar + newWeight * newWeight * newVar;
}

void Navigation::UpdateNavData(float xExp, float yExp, float headingExp, float xVarExp, float yVarExp, float headingVarExp) {
	//gets data from camera; updates robot's position, factoring in validity of camera data
	
	Combine(xExp, xVarExp, &x, &xVar);
	Combine(yExp, yVarExp, &y, &yVar);
	AngleCombine(headingExp, headingVarExp, &heading, &headingVar);
}

float Navigation::ToRadians(float degrees)
{
	return degrees *Pi / 180.0f;
}

float Navigation::ToDegrees(float radians)
{
	return radians * 180.0f / Pi;
}

void Navigation::StartPositionRobotToColumn(int pole)
{
	targetPole = pole;
}

bool Navigation::StatusPositionRobotToColumn()
{
	return false;
}

void Navigation::StopPositionRobotToColumn()
{
	
}

void Navigation::SetStartPosition()
{
	x = XYStartPos[targetPole - 1].x;
	y = XYStartPos[targetPole - 1].y;
	heading = -Pi / 2.0f;
	xVar = 1.0f / 144.0f;
	yVar = 16.0f / 144.0f;
	headingVar = (2.0f / 180.0f * Pi) * (2.0f / 180.0f * Pi);
}

float Navigation::GetDistanceToBait(int bait){ // pass in 1 - 6 for the targest, left to right
	POINTF XYBait;

	POINTF XYRobot;
	XYRobot.x = GetX();
	XYRobot.y = GetY();

	XYBait = XYKrakenBaits[(bait - 1)];

	float distance = sqrt((XYRobot.x - XYBait.x)*(XYRobot.x - XYBait.x) + (XYRobot.y - XYBait.y)*(XYRobot.y - XYBait.y));

	return distance;
}

float Navigation::GetHeadingToBait(int bait){
	POINTF XYBait;

	POINTF XYRobot;
	XYRobot.x = GetX();
	XYRobot.y = GetY();

	XYBait = XYKrakenBaits[(bait - 1)];
	
	/*
	float heading = ToDegrees(atan2((XYRobot.x - XYBait.x), (XYRobot.y - XYBait.y)));

	//heading = (float)((int)(heading * 1000) % 360000) / 1000.0;

	float currentAngle = gyro->GetAngle();

	int revolutions = (currentAngle / 360);
	heading += revolutions * 360;
	
	
	if(heading > 0 && currentAngle - heading > 180)
	{
		heading += 360;
	}
	else if (heading < 0 && heading - currentAngle > 180)
	{
		heading -= 360;
	}

	return heading;
	*/
	
	float currentAngle = gyro->GetAngle();
	float desiredHeading = atan2((XYRobot.x - XYBait.x), (XYRobot.y - XYBait.y));
	float currentHeading = GetHeading();
	
	return currentAngle - ToDegrees(WrapRadianAngle(currentHeading - desiredHeading));
	
	//return heading;
}

float Navigation::GetDistanceToPeg(int peg){ // pass in 1 - 6 for the targest, left to right
	POINTF XYPeg;

	POINTF XYRobot;
	XYRobot.x = GetX();
	XYRobot.y = GetY();

	XYPeg = XYKrakenPeg[(peg - 1)];

	float distance = sqrt((XYRobot.x - XYPeg.x)*(XYRobot.x - XYPeg.x) + (XYRobot.y - XYPeg.y)*(XYRobot.y - XYPeg.y));

	return distance;
}

float Navigation::GetHeadingToPeg(int peg){
	POINTF XYPeg;

	POINTF XYRobot;
	XYRobot.x = GetX();
	XYRobot.y = GetY();

	XYPeg = XYKrakenPeg[(peg - 1)];

	float heading = ToDegrees(atan2((XYRobot.x - XYPeg.x), (XYRobot.y - XYPeg.y)));

	//heading = (float)((int)(heading * 1000) % 360000) / 1000.0;

	float currentAngle = gyro->GetAngle();

	int revolutions = (currentAngle / 360);	
	heading += revolutions * 360;
	
	if(heading > 0 && currentAngle - heading > 180)
	{
		heading += 360;
	}
	else if (heading < 0 && heading - currentAngle > 180)
	{
		heading -= 360;
	}
	
	return heading;
}

void Navigation::VisionDataReceived(char *v)
{
	printf("Whatever...\n");
}


