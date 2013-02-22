///////////////////////////////////////////////////////////////////////////////////////
// Filename: SmartGyro.h                                                            ///
// Author:   Ken Cowan                                                              ///
// Date:     Feb 6, 2011                                                            ///
//                                                                                  ///
// This file contains a Gyro subclass that tries to eliminate noise from bad cables,///
// and rounds the heading to be 0 to 360.                                           ///
//                                                                                  ///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef SmartGyro_H_
#define SmartGyro_H_

#include "userincludes.h"

class SmartGyro;

class SmartGyro : public Gyro
{
protected:

	
public:
	SmartGyro( UINT32 port );   // constructor like Gyro

	// Return the current angle of the gyro, in degrees.  Attempts to keep result steady by
	// ignoring minor bounce in the angle
	virtual float GetAngle();
	
	// Return the raw angle, useful for diagnostics
	float GetAngleUnaltered();
	
	// Return the change since the last call to GetAngle
	float GetNetAngle();
	
	void Reset();
};

#endif
