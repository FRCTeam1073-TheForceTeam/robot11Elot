//////////////////////////////////////////////////////////
// Filename: SmartJoystick.cpp
// Author:   Ken Cowan
// Date:     Feb 6, 2011
//
// This file contains ...
//
//////////////////////////////////////////////////////////
#include "SmartJoystick.h"


bool SmartJoystick::ButtonClickDown(UINT32 button)
{
	StateMap::iterator it = buttonStates.find(button);
	bool current_state = GetRawButton(button);
	
	// If this is the first time the button is being queried, add it
	// to our state map using the current state
	if (it == buttonStates.end())
	{
		buttonStates[button] = current_state;
		return false;
	}
	
	// Get the previous state of the button 
	bool old_state = (*it).second;
	
	// Update the state
	it->second = current_state;
	
	// If the button was up, but now is down, we have a ClickDown event
	if ((old_state == false) && (current_state == true))
		return true;
	
	return false;
}

bool SmartJoystick::ButtonClickUp(UINT32 button)
{
	StateMap::iterator it = buttonStates.find(button);
	bool current_state = GetRawButton(button);
	
	// If this is the first time the button is being queried, add it
	// to our state map using the current state
	if (it == buttonStates.end())
	{
		buttonStates[button] = current_state;
		return false;
	}
	
	// Get the previous state of the button 
	bool old_state = (*it).second;
	
	// Update the state
	it->second = current_state;
	
	// If the button was down, but now is up, we have a ClickUp event
	if ((old_state == true) && (current_state == false))
		return true;
	
	return false;

}


