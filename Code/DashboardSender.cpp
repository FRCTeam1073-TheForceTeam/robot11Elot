///////////////////////////////////////////////////////////
// Filename: DashboardSender.cpp
// Author:   
// Date:     January 23, 2011
// Last Edited: 2/12/11
//
// Description: This file contains class defintion for the DashBoardSender responible for sending all data 
// to the custom dashboard.
//
//////////////////////////////////////////////////////////

/* print statements can be commented in/out for debug purposes.
 * not everything has a printf... YET.
 */

#include "Robot1073.h"
#include "DashboardSender.h"
#include "Encoders1073.h"
#include "SmartJoystick.h"
#include "SmartGyro.h"
#include "userincludes.h"
#include "Elevator.h"
#include "Pincer.h"


DashboardSender::DashboardSender()
{
	
}
void
DashboardSender::SendData(Robot1073 *p)
{
	static unsigned int packetCt = 0;
	float tempFloat;
	DriverStation *ds = DriverStation::GetInstance();
	Dashboard &dash = ds->GetHighPriorityDashboardPacker();
	dash.AddCluster();
	
	dash.AddU32(packetCt++);
	
	dash.AddU32(0xFFFFFFFF);
	unsigned short packData = 1;
	if(p->IsEnabled()){packData += 2;}
	if(false){packData += 4;} // has tube
	if(p->IsOperatorControl()){packData += 8;}
	else if(p->IsAutonomous()){packData += 16;}
	else {packData += 24;}
	if(false){packData += 32;} // has line
	if(p->arm->IsUpLimitSwitchActive()){packData += 64;}
	if(p->arm->IsDownLimitSwitchActive()){packData += 128;}
	if(p->pincer->IsClosedLimitSwitchActive()){packData += 256;}
	if(p->pincer->IsOpenLimitSwitchActive()){packData += 512;}
	
	if(p->leftLineSensor->Get()){ packData += 1024; }
	if(p->middleLineSensor->Get()){packData += 2048;  }
	if(p->rightLineSensor->Get()){packData += 4096; }
	
	if(p->kraken->GetMode() != p->kraken->IdleMode ) { packData += 1 << 13; }
	
	dash.AddU16(packData);
	
	//printf("left = %d middle = %d right = %d\n", p->leftLineSensor->Get(),p->middleLineSensor->Get(),p->rightLineSensor->Get());
	
	for (int i = 1; i <= 8; i++) {
		tempFloat = (float) AnalogModule::GetInstance(1)->GetAverageVoltage(i);
		dash.AddFloat(tempFloat); // analogs
		//printf("Float %d = %fV\n", i, tempFloat);
	}
	int module = 1;

	DigitalModule * digitalModule = DigitalModule::GetInstance(module);
	unsigned char relayForward = digitalModule->GetRelayForward();
	dash.AddU8(relayForward); // relays (forward)
	dash.AddU8(DigitalModule::GetInstance(module)->GetRelayReverse()); // relays (reverse)
	
	dash.AddU16((short)DigitalModule::GetInstance(module)->GetDIO()); // state
	dash.AddU16(DigitalModule::GetInstance(module)->GetDIODirection());//direction

	for (int i = 1; i <= 10; i++) {
			dash.AddU8((unsigned char) DigitalModule::GetInstance(module)->GetPWM(i)); // pwm's
			//printf("PWM %d %02X\n" ,i, DigitalModule::GetInstance(module)->GetPWM(i));
	}

	dash.AddFloat(p->matchTimer->GetTimeRemaining());
	dash.AddFloat(ds->GetBatteryVoltage());
	dash.AddFloat(p->gyro->GetAngle());
	dash.AddFloat(p->leftJoystick->GetX());
	dash.AddFloat(p->rightJoystick->GetX());
	dash.AddFloat(p->leftJoystick->GetY());
	dash.AddFloat(p->rightJoystick->GetY());
	std::pair<double, double> lrDistance = p->encoders->GetDistance();
	dash.AddFloat((float)lrDistance.first); 
	dash.AddFloat((float)lrDistance.second);

	// Navigation Data
	dash.AddFloat(p->navigation->GetX());
	dash.AddFloat(p->navigation->GetY());
	dash.AddFloat(5);//accel temp
	dash.AddFloat(5);//accel temp
	dash.AddFloat(p->navigation->GetXVelocity());
	dash.AddFloat(p->navigation->GetYVelocity());
	dash.AddFloat(sqrt(p->navigation->GetXVar())); 
	dash.AddFloat(sqrt(p->navigation->GetYVar())); 
	dash.AddFloat(p->navigation->GetHeading());
	dash.AddFloat(p->elevator->GetCurrentPositionFeet());
	dash.AddFloat(p->elevator->GetTargetPositionFeet());
	
	// jags++
	//+++++++++++++++++++++++
	dash.AddFloat((float)p->leftMotorJaguar->GetOutputCurrent());
	dash.AddFloat((float)p->rightMotorJaguar->GetOutputCurrent());
	dash.AddFloat((float)p->pincerJaguar->GetOutputCurrent());
	dash.AddFloat((float)p->armJaguar->GetOutputCurrent());
	dash.AddFloat((float)p->elevatorJaguarMotorA->GetOutputCurrent());

	dash.AddFloat(35); // pincher % open
	dash.AddU32(dashboardIndex);
	dash.AddFloat(p->matchTimer->GetElapsedTime());	

	dash.AddFloat((float)p->systemTimer->Get());
	int x = p->GetTargetPole();
	int y = (p->GetTargetFoot()+1)/2;

	dash.AddU8(x);
	dash.AddU8(y);

	dash.AddFloat(p->navigation->GetHeadingToPeg(x));
	dash.AddFloat(p->navigation->GetHeadingToBait(x));
	dash.AddFloat(p->navigation->GetDistanceToPeg(x));
	dash.AddFloat(p->navigation->GetDistanceToBait(x));
	
	dash.AddFloat(p->magEncoder->GetVoltage());
	// dash.AddFloat(p->arm->magEncoder->GetVoltage());
	dash.AddFloat(42.0);

	dash.AddU32(0xDCBA25); // temporary test data to make sure data is aligned
	
	
	dash.FinalizeCluster();
	dash.Finalize();
	
}
void
DashboardSender::SendConstants()
{
/* this function is to be called once as it sends static data... to ensure this...
 * (we apparently make extra mistakes at the compeition [not that this being
 * called twice would do much] anyway yeah... it can be called once. that's it.
 */
}
#if 0	
	
	/* for pack data add:
	 * if red allin add 2
	 * blue is 1... as defined.
	 */ 
	unsigned short packData = 1;
	//packet count
		//any 32 byte val
	//
	dash.AddU16(packData);
	DriverStation *ds = DriverStation::GetInstance();
	Dashboard &dash = ds->GetHighPriorityDashboardPacker();
	dash.AddCluster();
	
	
	//left right
	dash.AddI32(CAN_LeftMotorAddress);
	dash.AddI32(CAN_RightMotorAddress);
	
	//elevator
	dash.AddI32(CAN_ElevatorUpDownA);
	dash.AddI32(CAN_ElevatorUpDownB);
	
	dash.AddI32(CAN_Pincer);
	
	dash.AddI32(CAN_ElevatorArm);
	
	//servo	two different pwms
	dash.AddI32(PWM_CameraTiltServoPort);
	dash.AddI32(PWM_ElevatorBrakeServo);
			
	//digital io
	dash.AddI32(DIO_LeftLightSensorPort);
	dash.AddI32(DIO_MiddleLightSensorPort);
	dash.AddI32(DIO_RightLightSensorPort);
	
	//relays
	dash.AddI32(RELAY_MinibotDeployer);
	dash.AddI32(RELAY_PincerRoller);
	dash.AddI32(RELAY_RetroIlluminator);
	
	//analog ports
	dash.AddI32(ANALOG_GyroPort);
#if 0
	dash.AddI32(ANALOG_XAxisAccelerometerPort);
	dash.AddI32(ANALOG_YAxisAccelerometerPort);
#endif
	dash.AddI32(ANALOG_PincerMagneticEncoder);
	dash.AddI32(ANALOG_ElevatorArmMagneticEncoder);
	
//in user includes it was said to be obsolete... i if 0ed it to be safe...	
#if 0
	dash.AddI32(PWM_LeftMotorPort);
	dash.AddI32(PWM_RightMotorPort);
	dash.AddI32(DIO_LeftEncoderAPort);
	dash.AddI32(DIO_LeftEncoderBPort);
	dash.AddI32(DIO_RightEncoderAPort);
	dash.AddI32(DIO_RightEncoderBPort);
	dash.AddI32(DIO_ElevatorUpDownEncoderAPort);
	dash.AddI32(DIO_ElevatorUpDownEncoderBPort);		
#endif

	//motor + encoder
	dash.AddBoolean(IsLeftMotorReversed);
	dash.AddBoolean(IsRightMotorReversed);
	
	dash.AddBoolean(IsLeftEncoderReversed);
	dash.AddBoolean(IsRightEncoderReversed);
	
	//usb stuff
	dash.AddI32(USB_LeftJoystickPort);
	dash.AddI32(USB_RightJoystickPort);
	dash.AddI32(USB_OperatorJoystickPort);
	 
	//left joy
	dash.AddI32(LeftTurboButton);
	dash.AddI32(TurnToRackButton);
	
	//right joy
	dash.AddI32(RightTurboButton);
	dash.AddI32(LineFollowButton);

	//operator joystick
	dash.AddI32(GripGrabButton);
	dash.AddI32(GripReleaseButton);
	
	dash.AddI32(MinibotDeployButton);
	#if 0
	dash.AddI32(GripRollerUpButton);
	dash.AddI32(GripRollerDownButton);
	dash.AddI32(GripTiltUpButton);
	dash.AddI32(GripTiltDownButton);
	#endif
	dash.AddI32(ArmUpButton);	
	dash.AddI32(ArmDownButton);
	
	//misc consts... sort of just there... have fun Ryan; have fun...
	dash.AddFloat(WaitTime);
	dash.AddFloat(Pi);
	dash.AddFloat(PulsesPerFoot);
	dash.AddFloat(AngleStraightHome);
	dash.AddFloat(AngleStraightAway);
#endif	
