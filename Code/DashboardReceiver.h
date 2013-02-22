//////////////////////////////////////////////////////////
// Filename: DashboardReceiver.h
// Author:   
// Date:     January 23, 2011
//
// Description: This file contains class defintion for the DashBoardReceiver responible for sending all data 
// to the custom dashboard.
///
///////////////////////////////////////////////////////////
#ifndef DASHBOARDRECEIVER_H_
#define DASHBOARDRECEIVER_H_

#include "userincludes.h"

class DashboardReceiver
{
	public:
		
		DashboardReceiver();
		~DashboardReceiver();
		void ReceiveData(void);
		
	private:
		struct sockaddr_in si_me, si_other;
		int hSocket;

};

#endif
