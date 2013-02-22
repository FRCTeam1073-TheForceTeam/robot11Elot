#include <stdio.h>

#include "WPILib.h"
#include "CameraManager.h"



CameraManager::CameraManager()
{
	timer.Start();
}


void
CameraManager::StartCamera()
{
#if 1
	while(timer.Get() < 10)
	{
		int timeLeft = (int)(10.0 - timer.Get());
		
		printf("Time until initialization (%d)\n", timeLeft);
		Wait(1);
	}
	
	AxisCamera &camera = AxisCamera::GetInstance();
	camera.WriteResolution(AxisCameraParams::kResolution_320x240);
	camera.WriteBrightness(0);
#endif	
}
