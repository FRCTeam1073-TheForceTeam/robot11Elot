//////////////////////////////////////////////////////////
// Filename: SmartJagMotor.cpp
// Author:   John Heden / Ken Cowan
// Date:     Feb 6, 2010
//
// Defines a SmartJagMotor that provides encapsulated Jag motor/encoder functionality
//
//////////////////////////////////////////////////////////
#include "SmartJagMotor.h"


#ifdef USE_JAG_SEMAPHORE
VX_BINARY_SEMAPHORE(JagSem);   /* declare the semaphore */
SEM_ID JagSemaphoreId;
#define SEM_TAKE 	semTake( JagSemaphoreId, WAIT_FOREVER );
#define SEM_GIVE	semGive( JagSemaphoreId );
#else
#define SEM_TAKE
#define SEM_GIVE
#endif


// Need the CAN address of this motor, encoder pulses per ft, and whether motors & encoders are reversed... 
SmartJaguarMotorEncoder::SmartJaguarMotorEncoder(UINT8 deviceNumber, UINT16 _pulsesPerFt, bool reverseMotor, bool reverseEncoder) 
: CANJaguar(deviceNumber),								// pass this address to base class constructor
  isMotorReversed(reverseMotor),
  isEncoderReversed(reverseEncoder),
  pulsesPerFt(_pulsesPerFt)
{

#ifdef USE_JAG_SEMAPHORE
	// Create a semaphore so we can serialize motor set calls and reading encoders
	// For now, our semaphore is static, so that we only access one jag at a time.
	// Only the first Jag needs to init the semaphore
	if (JagSemaphoreId == NULL)
	{
		printf("Initializing the Jaguar semaphore\n");
		JagSemaphoreId = semBInitialize(JagSem, SEM_Q_FIFO, SEM_FULL);
	}
#endif

}



void 
SmartJaguarMotorEncoder::ResetEncoder()
{
	SEM_TAKE
	
	CANJaguar::ControlMode oldMode = GetControlMode();
	//printf("Reseting encoder on Jag [%d], mode %d\n", m_deviceNumber, oldMode);

	SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	ConfigEncoderCodesPerRev(pulsesPerFt);
	
	ChangeControlMode(CANJaguar::kPosition);
	EnableControl(0.0);
	ChangeControlMode(oldMode);
	EnableControl();

	// GetPosition will take the semaphore too
	SEM_GIVE

	//printf("Reset encoder, initial val on Jag [%d] = %3.5lf\n", m_deviceNumber, GetPosition());
	

}


void
SmartJaguarMotorEncoder::Set(float value)
{
	SEM_TAKE
	
	float new_value = value;
	if(isMotorReversed)	//If motor is reversed, reverse setting
		new_value *= -1.0;

	CANJaguar::Set(new_value);
	
	SEM_GIVE
}

double SmartJaguarMotorEncoder::GetPosition()
{
	SEM_TAKE
	
	// Position as read from encoder, via CANJaguar
	double pure_pos = this->CANJaguar::GetPosition();

	if(isEncoderReversed)	//If encoder is reversed, reverse value
		pure_pos *= -1.0;
	
	SEM_GIVE
	
	return pure_pos;
}

double SmartJaguarMotorEncoder::GetPositionUnaltered()
{
	SEM_TAKE
	
	double pos = this->CANJaguar::GetPosition();
	
	SEM_GIVE
	
	return pos;
}
