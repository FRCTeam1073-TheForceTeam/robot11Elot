
#ifndef __Pincer_h__
#define __Pincer_h__

#include "userincludes.h"
#include "SmartJagMotor.h"
#include "SmartJoystick.h"
#include <stdio.h>

class Pincer
{
public:
	Pincer(SmartJaguarMotorEncoder *mb, Relay *rR, AnalogChannel *mag, SmartJoystick *opJoy);
	void PeriodicService();
	
	enum PincerPositions { Moving, FullyOpen, FullyClosed, StalledOpen, StalledClosed, PresetPosition, PositionError };
	PincerPositions GetPosition();
	
	void Close();
	void Open();
	void GotoPreset();

	void RollIn();
	void RollOut();
	void RollOff();

	
	bool IsOpenLimitSwitchActive();
	bool IsClosedLimitSwitchActive();

	bool IsPincerOpen() { return (position == StalledOpen || position == FullyOpen); }
	// When the pincer closes on a tube, it's not fully closed, but the Close operati
	// stops because the motor stalls.  It's "IsPincerCloseDone", since it reflects the close 
	// operation being finished.
	bool IsPincerClosed() { return (position == StalledClosed || position == FullyClosed); }

	enum ControlMode { ClosingMode, OpeningMode, SeekingPresetMode, OffMode, ManualMode };
	ControlMode controlMode;
private:
	void CheckButtons();
	bool IsStallDetected(bool doReset = false);
	PincerPositions position;	 
	void SetMotorOutput(float val);

	AnalogChannel *magEncoder;
    SmartJaguarMotorEncoder *motorEncoder;
    SmartJoystick *operatorJoystick;
    Relay *rollerRelay;
};

#endif
