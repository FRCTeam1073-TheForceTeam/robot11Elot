//////////////////////////////////////////////////////////
// Filename: DashboarReceiver.cpp
// Author:   
// Date:     January 28, 2011
//
// Description: This file contains class defintion for the DashBoardReceiver responible all dashboard data through 
// UDP Port 1130...
//
//////////////////////////////////////////////////////////
#include "DashboardReceiver.h"
#include "Robot1073.h"
#include "Elevator.h"
#include "Navigation.h"
#include "KrakenDrive.h"

#define BUFLEN 512
#define PORT 1130

// This important static is to allow our launched receiver thread to access our main Robot class. 
static Robot1073 *pRobot1073;
static DashboardReceiver *pDashboardReceiver;

int lastID = -1;
static int lowFeet [3] = { 1, 3, 5};
static int highFeet [3] = { 2, 4, 6};



void
Robot1073::ProcessDashboardData(char *rxRequest)
{
	bool isOn;
	char *startPos;
	int x;
	int y;
	int y1to6;
	int index;
	int ID = atoi(strtok (rxRequest, "\t")); 

	if (ID == lastID)
		return;
	char dataType = strtok ( NULL , "\t")[0];
	switch (dataType)
	{
	case 'A':
		startPos = "Unknown";
		x = atoi(strtok ( NULL , "\t"));
		y = atoi(strtok ( NULL , "\t"));
		printf("The coordinates are (%d, %d)\n", x, y);
		if (x > 6 || x < 1)
		{
			printf("Invalid X cordinate");
			y1to6 = 0;
		}
		else if (x == 1)
		{
			//You cant start there because there is no line
			// JFH Should discuss this
			startPos = "Unable to start in position %d.\n";
			y1to6 = lowFeet [y-1];
		}
		else if (x == 2)
		{
			// we are on the Left most Straight
			startPos = "Straight, Left\n";
			y1to6 = highFeet [y-1];
		}
		else if (x == 3)
		{
			//we are on the Fork going Left
			startPos = "Fork, Left\n";
			y1to6 = lowFeet [y-1];
		}
		else if (x == 4)
		{
			// we are on the Fork going Right
			startPos = "Fork, Right\n";
			y1to6 = lowFeet [y-1];
		}
		else if (x == 5)
		{
			//we are on the Right most Straight
			startPos = "Straight, Right\n";
			y1to6 = highFeet [y-1];
		}
		else if (x == 6)
		{
			//You cant start there because there is no line
			startPos = "Unable to start in position %d.\n";
			y1to6 = lowFeet [y-1];
		}
		printf("Krkaken is heading %s" , startPos);
	
		SetTargetPoleAndFoot(x, y);
		
		break;
	case 'E':
		isOn = true;
		elevator->GoToFeederPosition(isOn);
		isOn = false;
		break; 	
	case 'O':
 		{
			if(pRobot1073->kraken->GetMode() == pRobot1073->kraken->IdleMode)
			{
				pRobot1073->kraken->SetAutoReleaseButton(true);
			}
			else
			{
				pRobot1073->kraken->SetAutoReleaseButton(false);
			}
 		}
	case 'R':
		{
			float xoff = (float)atof(strtok( NULL ,"\t"));
			// float yoff = atof(strtok( NULL ,"\t"));  LineFollower doesn't (yet?) need this variable
			lineFollower->SetXOff(xoff);
			//lineFollower->retroTape = true;

			printf("xoff: %f ", xoff);
			// JFH To be discussed...
			break;
		}
	case 'V':
		navigation->VisionDataReceived(rxRequest);
		break;
	
	case 'X':
		index = atoi(strtok ( NULL , "\t"));
		dashboardSender->SetDashboardIndex(index);
		break;
	
		
		
	default:
		printf("unknown command\n");
		break;
	}
	
	lastID = ID;
}



DashboardReceiver::DashboardReceiver( )
{
	
	if ((hSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
	    printf("Socket Error\n");
	    return;
	}
			    
	memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(hSocket, (struct sockaddr *)&si_me, sizeof(si_me))==-1){
			printf("Binding Error\n");
		    return;
	}
}


DashboardReceiver::~DashboardReceiver()
{
#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH  // Posix uses SHUT_RDWR, Win32 SD_BOTH
#endif
	shutdown(hSocket, SHUT_RDWR);	
}

void
DashboardReceiver::ReceiveData()
{	
		int slen=sizeof(si_other);
		char buf[BUFLEN];

		memset((char *) &buf[0], 0, BUFLEN);
		if (recvfrom(hSocket, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)==-1){
	          printf("recvfrom() Error\n");
	          return;
	          
		}
	    printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

	    pRobot1073->ProcessDashboardData(buf);
}



int
DashboardReceiverFunction()
{
	
	printf("Receiver Function called, waiting for Dashboard data...\n");
	while (1)  // Loop forever waiting for dahbboard data to arrive
	{
		pDashboardReceiver->ReceiveData();
		// Eventually need to call Robot with this information...
	}
}


//SEM_ID DashboardReceiverSemaphore = 0; // Not used and not supported by Sim.  Remove for now
Task *DashboardReceiverTask;
void
Robot1073::InitializeDashboardReceiverThread(Robot1073 *ptr, DashboardReceiver *rcv)
{
	printf("Receiver thread initialized...\n");
	pRobot1073 = ptr;
	pDashboardReceiver = rcv;
	DashboardReceiverTask = new Task("DashboardReceiverFunction", (FUNCPTR)DashboardReceiverFunction );
	//DashboardReceiverSemaphore = semMCreate(SEM_DELETE_SAFE | SEM_INVERSION_SAFE); // synchronize access to multi-value registers
	DashboardReceiverTask->Start();
}
