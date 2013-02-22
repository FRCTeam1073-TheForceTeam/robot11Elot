#include "math.h"  
#include "Robot1073.h"


/*****************************************************************************************************
 *  Zombie Zone --> Is the intermediate life outside of Autonomous and Teleoperated allowing background of our software prior to 
 *     Autonomous and teleoperated modes.  Continues life even when disabled....
 *     1) Dashboard data sends
 * 	   2) Menu Controls and updates
 * 	   3) LCD display updates
 * 	   4) Diagnostic updates
 *  
 * *****************************************************************************************************/

// This important static is to allow our launched ZombieZone thread to access our main Robot class. 
// This static is used exclusively to call the main bots ZombieZonePeriodicServics().
static Robot1073 *pRobot1073;
const float periodicIntervalSec = .05f;

int
ZombieFunction(...)
{
	pRobot1073->retroIlluminator->Set(.7);
	
	while (1)  // No stopping the Zombie, Life before Autonomous....
	{
		Wait(periodicIntervalSec);		// Wait 50ms for a 20Hz update rate...
		pRobot1073->ZombieZonePeriodicService();	
	}
}


SEM_ID ZombieSemaphore = 0;
Task *ZombieTask;
void
Robot1073::InitializeTheZombieZone(Robot1073 *ptr)
{
	printf("Zombie Initiation ocurred\n");
	pRobot1073 = ptr;
	ZombieTask = new Task("ZombieFunction", ZombieFunction );
	ZombieSemaphore = semMCreate(SEM_DELETE_SAFE | SEM_INVERSION_SAFE); // synchronize access to multi-value registers
	ZombieTask->Start();
}
