//////////////////////////////////////////////////////////
// Filename: Autonomous.cpp
// Author:   
// Date:     February 2, 2011
// 
// Description: This file contains the main definition for the Team1073 Robot.
//
//////////////////////////////////////////////////////////
#include "Robot1073.h"
void Robot1073::Autonomous(void)
{
	
	encoders->ResetEncoders();
	
	matchTimer->StartAutonomous();
	navigation->SetStartPosition();
	navigation->Start();
	pincer->Close();
	retroIlluminator->Set(1.0);

	while (IsAutonomous())
	{
		// Service the Kraken, elevator, arm, pincer, etc.
		DoPeriodicServiceFunctions();
		Wait(WaitTime);				// wait for a motor update time
	}
	kraken->StopAll();
}
