#ifndef SMARTJOYSTICK_H_
#define SMARTJOYSTICK_H_

#include "userincludes.h"

// Stop the madness proper joystick perspective such that forward stick is positive 
// rather than negative...
// Let's declare our own "Joystick like" class called "SmartJoystick"
class SmartJoystick : public Joystick
{
public: 
	SmartJoystick(UINT32 port)	// Need a constructor like Joystick's
	:Joystick(port)				// and pass in USB port number..
	{	}						// Nothing else to do..
	
	virtual float GetY()		// let's override Joystick basclass functionality  
	{									// Our SmartJoystick simply inverts the Joystick to a more
	   return -1 * Joystick::GetY();  	// logical perspective such that forward is positive
	}									// and pulling back is negative...
	
	float GetYUnaltered()		// for diagnostic purposes, the unadulterated stick value
	{									
	   return Joystick::GetY();  	
	}
	
	float GetXUnaltered()	// for symmetry
	{
		return Joystick::GetX();
	}

	bool ButtonClickDown(UINT32 button);
	bool ButtonClickUp(UINT32 button);
	
protected:
	typedef map<UINT32, bool> StateMap; 
	StateMap buttonStates;
	
};
#endif
