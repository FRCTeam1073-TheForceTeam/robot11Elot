//////////////////////////////////////////////////////////
// Filename: DashboardSender.h
// Author:   
// Date:     January 23, 2011
// Last Edited: 2/12/11
//
// Description: This file contains class defintion for the DashBoardSender responible for sending all data 
// to the custom dashboard.
//
//////////////////////////////////////////////////////////
#ifndef DASHBOARDSENDER_H_
#define DASHBOARDSENDER_H_

#include "userincludes.h"

class DashboardSender;
class Encoders1073;
class SmartJoystick;
class SmartGyro;
class CANJaguar;
class SmartJaguarMotorEncoder;

class Robot1073;

class DashboardSender
{
	public:
		
		DashboardSender();
					
		void SendData(Robot1073 *p);
		void SendConstants(void);
		
		void SetDashboardIndex(int val) { dashboardIndex = val; };

	private:
		int dashboardIndex;
};

#endif
