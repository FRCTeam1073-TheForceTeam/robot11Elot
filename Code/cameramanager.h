//////////////////////////////////////////////////////////
// Filename: CameraManager.h
// Author:   
// Date:     January 23, 2011
//
// Description: This file contains the basic camera manager.  It is starting out as a simple stub with a time 
// holdoff to ensure the camera is pwered up and functional prior to program execution.
//
//////////////////////////////////////////////////////////
#ifndef __CameraManager_h__
#define __CameraManager_h__

#include "WPILib.h"
#include "Vision\AxisCamera.h"
#include <stdio.h>


class CameraManager
{
	
public:
	CameraManager();
	
	void StartCamera();
	
private:
    Timer timer;  //comment
    AxisCamera *camera;
};

#endif
