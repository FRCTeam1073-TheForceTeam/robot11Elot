#include "drivermessages.h"
#include "Robot1073.h"


DriverMessages::DriverMessages()
{
	displayIndex = 2;
}

void DriverMessages::PeriodicService(Robot1073 *p1073)
{
	bool isButtonClicked = p1073->leftJoystick->ButtonClickDown(CycleDriverStationDiagsButton);
	std::pair<double, double> xyVelocity;
	 
	// Since we're using a special mode on driver stick, there is code in LNDrive.cpp
	// Elevator brake needs to understand this test mode too
	bool isTestMode = p1073->rightJoystick->GetRawButton(DriverMessagesTestButton) && !p1073->rightJoystick->GetRawButton(MinibotSafetyButton);
	
	switch(displayIndex){
	
	case 0:		PrintIt(0, "Joystick Test");
				PrintIt(1, "L XY=%6.3f, %6.3f",p1073->leftJoystick->GetX(), p1073->leftJoystick->GetY());
				PrintIt(2, "R XY=%6.3f, %6.3f", p1073->rightJoystick->GetX(), p1073->rightJoystick->GetY());
				break;
	
	case 1:     PrintIt(0, "Gyro=%3.5f", p1073->gyro->GetAngle());
				PrintIt(1, "Heading=%3.5f", p1073->navigation->GetHeading());
				break;
	
	case 2:     
				{
					double leftPos = p1073->leftMotorJaguar->GetPosition();
					double rightPos = p1073->rightMotorJaguar->GetPosition();
					double elevatorPos = p1073->elevatorJaguarMotorA->GetPosition();
		 			PrintIt(0, "Encoders");
		 			PrintIt(1, "l:%3.5lf, r:%3.5lf", leftPos, rightPos);
		 			PrintIt(2, "e:%3.5lf", elevatorPos);
				}
	 			break;
	
	case 3:		PrintIt(0, "Elevator brake diags");
				PrintIt(1, "Servo: %f", p1073->elevatorBrakeServo->Get());
				if (isTestMode)
				{
					float yval = p1073->operatorJoystick->GetY();
					p1073->elevatorBrakeServo->Set(yval);
					PrintIt(2, "Changing servo: %f", yval);
				}
				else
				{
					PrintIt(2, "Limit: %s", p1073->elevator->GetLimitSwitch() ? "pushed" : "not pushed");
				}
				break;
	case 4:		PrintIt(0, "Heading=%3.5f", p1073->navigation->GetHeading());
				PrintIt(1, "x:%f", p1073->navigation->GetX());
				PrintIt(2, "y:%f", p1073->navigation->GetY());
				break;
	
	case 5:		PrintIt(0, "XVelocity = %f", p1073->navigation->GetXVelocity());
				PrintIt(1, "YVelocity: %f", p1073->navigation->GetYVelocity());
				break;
				
	case 6:		/*PrintIt(0, "Pincer encoder %3.5f", p1073->magPincerEncoder->GetVoltage());
				PrintIt(1, "Close: %s", p1073->pincer->IsClosedLimitSwitchActive()? "pushed" : "not pushed");
				PrintIt(2, "Open: %s",  p1073->pincer->IsOpenLimitSwitchActive() ? "pushed" : "not pushed");*/
				break;
	
	case 7:		/*PrintIt(0, "Arm limits");
				PrintIt(1, "Up: %s", p1073->arm->IsUpLimitSwitchActive()? "pushed" : "not pushed");
				PrintIt(2, "Down: %s",  p1073->arm->IsDownLimitSwitchActive()? "pushed" : "not pushed");*/
				break;
				
	case 8:
				PrintIt(0, "Camera servo: %f", p1073->cameraServo->Get());
				if (isTestMode)
				{
					float yval = p1073->operatorJoystick->GetY();
					p1073->cameraServo->Set(yval);
					PrintIt(1, "Changing servo: %f", yval);
				}
				break;
				
	
	case 9:	PrintIt(0, "Illuminator %f", p1073->retroIlluminator->Get());
				if (isTestMode)
				{
					float yval = p1073->operatorJoystick->GetY();
					p1073->retroIlluminator->Set(yval);
					PrintIt(1, "Setting illum: %f", yval);
				}

				break;

				
	case 10:	PrintIt(0, "Minibot stage 1");
				if (isTestMode)
				{
					float yval = p1073->rightJoystick->GetZ();
					float oldServoVal = p1073->minibotFirstStageServo->GetAngle();
					p1073->minibotFirstStageServo->SetAngle(yval * 170);
					PrintIt(1, "Was %3.5f, now: %3.5f", oldServoVal, yval);
				}
				else
				{
					PrintIt(1, "Was %3.5f", p1073->minibotFirstStageServo->GetAngle());
				}
				break;

	case 11:	PrintIt(0, "Minibot stage 2");
				if (isTestMode)
				{
					float yval = p1073->rightJoystick->GetZ();
					float oldServoVal = p1073->minibotSecondStageServo->GetAngle();
					p1073->minibotSecondStageServo->SetAngle(yval * 170);
					PrintIt(1, "Was %3.5f, now: %3.5f", oldServoVal, yval);
				}
				else
				{
					PrintIt(1, "Was %3.5f", p1073->minibotSecondStageServo->GetAngle());
				}
				break;
	case 12: 
				{
					bool leftIsOn = p1073->leftLineSensor->Get() != 0;
					bool middleIsOn = p1073->middleLineSensor->Get() != 0;
					bool rightIsOn = p1073->rightLineSensor->Get() != 0;
					PrintIt(0,"Line Sensors");
					PrintIt(1,"L:%s, M:%s, R:%s", leftIsOn ? "ON" : "OFF" ,
							middleIsOn ? "On" : "OFF", rightIsOn ? "ON" : "OFF" );

				}
				break;
	case 13:
				{
					float fTSpeed = p1073->topShooterMotorJaguar->GetSpeed();
					float fBSpeed = p1073->bottomShooterMotorJaguar->GetSpeed();
					float fTPos = p1073->topShooterMotorJaguar->GetPosition();
					float fBPos = p1073->bottomShooterMotorJaguar->GetPosition();
					float fTMin = p1073->shooter->GetTopMinimumSpeed();
					float fBMin = p1073->shooter->GetBottomMinimumSpeed();
					PrintIt(0,"Spe-T: %0.0f, B: %0.0f", fTSpeed, fBSpeed);
					PrintIt(1,"Pos-T: %0.0f, B: %0.0f", fTPos, fBPos);
					PrintIt(2,"Min-T %0.0f, B: %0.0f", fTMin, fBMin);
					break;
				}
	default:	displayIndex = 0;
				break;
	}
	SendTextLines();
		
	if(isButtonClicked)   		// Has the button clicked down
		displayIndex += 1;		// Set next display count, default will manage wrap..
}

void
DriverMessages::SendTextLines()
{
	DriverStationLCD *lcd =DriverStationLCD::GetInstance();
	
	for(int i = 0; i < 3;i++){
			lcd->Printf((DriverStationLCD::Line)i, 1, "%s", (char *)lineText[i]);
	}
	lcd->UpdateLCD();
	
}


void
DriverMessages::PrintIt(int lineIndex, char *fmt, ...)
{
  va_list       args;
  va_start (args, fmt);

  if ((lineIndex < 0) || (lineIndex > 6))
	  return;
  if(lineIndex == 0){		// If printing the first line, then assume 1,2, & 3 are blanked
	  for(int i = 0; i < 3; i++){
		  strcpy((char *)lineText[i], "                     ");
	  }
  }
  
  vsprintf (lineText[lineIndex], fmt, args);
  strcat(lineText[lineIndex], "                     ");
  va_end (args);

	
}
