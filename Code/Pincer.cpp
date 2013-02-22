#include "Pincer.h"

#define PINCER_DEBUG 1
#define MANUAL_MODE 0


static const float closingSpeed = 0.65f;
static const float openingSpeed = -0.70f;

// Let's calculate the window size based on History duration required
// For a 1/4 second and a 0.05 duretion, this should be an array of 6 or 7 (float uncertainty)
static const float HistoryDurationSeconds = 0.250f;  // 1/4 second 
// Need to do this calculation by hand for now as VC++ has some problems...
// static const int PincerWindowSize = ((int)(HistoryDurationSeconds/WaitTime)) + 1;
static const int PincerWindowSize = 6;  // based on 0.050 wait time...
static const float StallDetectionLimit = 0.3f;

static const float PresetTargetPosition = 0.30f;		// Need Calibration
static const float AllowedPositionError = 0.10f;		// Need to determine


Pincer::Pincer(SmartJaguarMotorEncoder *mb, Relay *rR, AnalogChannel *mag, SmartJoystick *opJoy)
{
	motorEncoder = mb;
	rollerRelay = rR;
	magEncoder = mag;
	operatorJoystick = opJoy;
	SetMotorOutput(0);
	RollOff();
}


bool PincerDebug = false;

bool Pincer::IsStallDetected(bool doWindowReset)
{
	static float encoderValues[PincerWindowSize];
	static int index = 0;

	if(doWindowReset)
		index = 0;
		

	int useIndex = index % PincerWindowSize;
	++index;	// Bump Index for next time...
		
	float currentVoltage = magEncoder->GetVoltage();
	float oldVoltage = encoderValues[useIndex];
	encoderValues[useIndex] = currentVoltage;
		
	// Cannot have opinion on stall UNTIL we have at lest a full window of data
	if(index < PincerWindowSize)		// If we don't have enough data
		return false;			// Say we are NOT stalled...

	float travel = fabs(currentVoltage - oldVoltage);
	
	//if(PincerDebug)
	//	printf("PincerErrror = %f\n", error);
	
	if (travel < StallDetectionLimit)
	{
		return false;
	}
		
	return true;
}

void Pincer::Open()
{	
	controlMode = OpeningMode;
#if SIMULATOR
//		printf("Pincer: Opening\n");
#endif

#ifdef PINCER_DEBUG
//				printf("Open Requested\n");
#endif				

}

void Pincer::Close()
{
	controlMode = ClosingMode;

#if SIMULATOR
//		printf("Pincer: Closing\n");
#endif


#ifdef PINCER_DEBUG
//				printf("Close Requested\n");
#endif			
}

void Pincer::RollIn()
{
	rollerRelay->Set(Relay::kReverse);
#if SIMULATOR
//		printf("Roller: Rolling In\n");
#endif

	
}

void Pincer::RollOut()
{
	rollerRelay->Set(Relay::kForward);
#if SIMULATOR
//	printf("Roller: Rolling Out\n");
#endif
	
}

void Pincer::RollOff()
{
	rollerRelay->Set(Relay::kOff);

}


// This should be the only function reading the Top/Open Limit position switch
bool Pincer::IsOpenLimitSwitchActive()
{
	return !motorEncoder->GetReverseLimitOK();
}
// This should be the only function reading the Bottom/Closed Limit position switch
bool Pincer::IsClosedLimitSwitchActive()
{
	return !motorEncoder->GetForwardLimitOK();
}

// All Motor Output requests should filter through this single function.
void
Pincer::SetMotorOutput(float value)
{
	static float lastValue = -99.0;		
	
	if(value != lastValue)
		(void)IsStallDetected(true);	// Request that Stall detect reset window on any change in motor value...

	motorEncoder->Set(value);
	lastValue = value;					// Simply set last to current...
}


void Pincer::PeriodicService()
{
	
	//Let's check and update every time
	bool isStalled = IsStallDetected();
	
	
	CheckButtons();
	
	switch(controlMode){
		
		case ClosingMode:
			PincerDebug = true;
			position = Moving;
			SetMotorOutput(closingSpeed);
			//printf("Closing, Stalled = %d\n", isStalled);
			if(IsClosedLimitSwitchActive() || isStalled){
				position = isStalled ? StalledClosed : FullyClosed; 
				SetMotorOutput(0);
				controlMode = OffMode;
#ifdef PINCER_DEBUG
	//			printf("Pincer: Was closing, now off(lim = %d, stalled = %d)\n", IsClosedLimitSwitchActive(), isStalled);
#endif				
			}
			
			break;
				
		case OpeningMode:
			PincerDebug = true;
			position = Moving;
			//printf("Opening, Stalled = %d\n", isStalled);

			SetMotorOutput(openingSpeed);
			if(IsOpenLimitSwitchActive() || isStalled){
				position = isStalled ? StalledOpen : FullyOpen; 
				SetMotorOutput(0);
				controlMode = OffMode;
#ifdef PINCER_DEBUG
//				printf("Pincer: Was opening, now off(lim = %d, stalled = %d)\n", IsOpenLimitSwitchActive(), isStalled);
#endif				
				
			}
			break;

		case SeekingPresetMode:
			position = Moving;
			if(IsOpenLimitSwitchActive() || IsClosedLimitSwitchActive() || isStalled){
				position = PositionError;
				SetMotorOutput(0);
			}
			else{
				
				// This PI stuff is getting a bit complex and should probably be moved to a member function..
				position = Moving;		//Assume Moving
		
				float positionNow = magEncoder->GetVoltage();
				float err = (PresetTargetPosition - positionNow);
			
				if(fabs(err) < AllowedPositionError){
					SetMotorOutput(0);
					controlMode = OffMode;
					position = PresetPosition;
				}
				
				else{
					static const float Kp = 1.0;
					static const float Ki = .25;
					static float i;
					float lim = 0.0;
					i += err;
					lim = 2.0f;
					if(i > lim){i = lim;}
					if(i < -lim){i = -lim;}
					float output = err*Kp + i * Ki;
					SetMotorOutput(output);
				}
			}
			break;
			
		case ManualMode:
			// Do nothing as buttons are directly controlling...
			break;
			
		case OffMode:
		default:	
			PincerDebug = true;
			SetMotorOutput(0);
	}
}

void Pincer::CheckButtons()
{
// Check Buttons and do what is requested..		
#if !MANUAL_MODE
		if(operatorJoystick->ButtonClickDown(GripGrabButton))
			Close();
	
		else if (operatorJoystick->ButtonClickDown(GripReleaseButton))
			Open();
#else
		if(operatorJoystick->GetRawButton(GripGrabButton)){
			SetMotorOutput(closingSpeed);
			controlMode = ManualMode;
			//printf("ManualClose %f\n", closingSpeed);
			
		}
		else if (operatorJoystick->GetRawButton(GripReleaseButton)){
			SetMotorOutput(openingSpeed);
			controlMode = ManualMode;
			//printf("ManualOpen %f\n", openingSpeed);
		}
		else if (controlMode == ManualMode){
			controlMode = OffMode;
			SetMotorOutput(0);
		}

#endif
		
		static bool wasRollerInButton = false;
		static bool wasRollerOutButton = false;

		bool isRollerInButton = operatorJoystick->GetRawButton(GripRollerInButton);
		bool isRollerOutButton = operatorJoystick->GetRawButton(GripRollerOutButton);


		if(isRollerInButton){
			RollIn();
		}
		else if (isRollerOutButton){
			RollOut();
		}
		else{
			if(wasRollerInButton || wasRollerOutButton){
				RollOff();
#if SIMULATOR
//				printf("Roller: Roller OFF\n");
#endif
			}	

		}
		wasRollerInButton = isRollerInButton;
		wasRollerOutButton = isRollerOutButton;
}
