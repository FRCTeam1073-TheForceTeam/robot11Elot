//////////////////////////////////////////////////////////
// Filename: SmartJaguarMotorEncoder
// Author:   
// Date:     January 23, 2011
//
// Description: This file contains the SmartJaguarMotoeEncode Class to deal with the nuances of the Jag
//////////////////////////////////////////////////////////
#ifndef __SmartJaguarMotor_h__
#define __SmartJaguarMotor_h__

#include "userincludes.h"

class SmartJaguarMotorEncoder : public CANJaguar
{
public: 
	// Need the CAN address of this motor & pulses per ft, 
	SmartJaguarMotorEncoder(UINT8 deviceNumber, UINT16 _PulsesPerFt = 0, bool reverseMotor = false, bool reverseEncoder = false);	
	void Set(float value);
	
	// Reset the encoder to 0
	void ResetEncoder();	
	
	// Return the position, where noise has been removed
	double GetPosition();
	
	// Return the raw position data.  Useful to diagnose bad cables etc.
	double GetPositionUnaltered();
	
	// Use this to display debugging messages
	UINT16 GetPulsePerFt() { return pulsesPerFt; }
	
private: 
	bool isMotorReversed;
	bool isEncoderReversed;	
	
	UINT16 pulsesPerFt;		// scaling factor for the encoder attached to this Jaguar		

};




#endif

