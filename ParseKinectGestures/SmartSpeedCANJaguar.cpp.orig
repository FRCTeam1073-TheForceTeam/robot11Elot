#include "smartspeedcanjaguar.h"



SmartSpeedCANJaguar::SmartSpeedCANJaguar(UINT8 deviceNum, int encPulsesPerRev, bool isEncReversed)
:CANJaguar(deviceNum),
 isEncoderReversed(isEncReversed),
 encoderPulsesPerRevolution(encPulsesPerRev),
 pid_P(0), pid_I(0), pid_D(0),
 lastSetSpeed(-1.0)
{
}



void
SmartSpeedCANJaguar::SetSpeedRevolutionsPerMinute(float speed)
{
	if(lastSetSpeed < 0){		//Have we ever set a specific speed ?
		Initialize();			// Initialize if not
	}
	
	CANJaguar::Set(speed);
	lastSetSpeed = speed;
}

void 
SmartSpeedCANJaguar::SetPID(double p, double i, double d)
{
	if(isEncoderReversed){
		p *= -1;
		i *= -1;	
	}

	// Store the PID paramaters incase we need to reset the jaguar...
	pid_P = p;
	pid_I = i;
	pid_D = d;

}

void 
SmartSpeedCANJaguar::Initialize()
{
	ChangeControlMode(kSpeed);								// Set for speed mode
	SetSpeedReference(CANJaguar::kSpeedRef_Encoder);		// Define the encoder type
	ConfigEncoderCodesPerRev(encoderPulsesPerRevolution);	// Define the lines per revolution
	CANJaguar::SetPID(pid_P, pid_I, pid_D);					// Set our passed in PID constants
	EnableControl();										// Must enable the control paramaters									
	CANJaguar::Set(lastSetSpeed);							// Restore the last target speed...
}




void
SmartSpeedCANJaguar::PeriodicService()
{
	static int counter = 0;
	
	if(lastSetSpeed < 0)		// Do nothing until speed has been set
		return;
	
	if(++counter %20 == 0){		// Let's just check every second or so rather thaan at 20 Hz, same some traffic...
		if(GetControlMode() != CANJaguar::kSpeed){
			printf("Speed Jaguar Reset, Reinitializing\n");
			Initialize();
		}
	}
	
}
