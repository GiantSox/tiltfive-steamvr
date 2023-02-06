#include "driver.h"
#include <Windows.h>

vr::EVRInitError TiltFiveSteamVRDriver::Init(vr::IVRDriverContext* pDriverContext)
{
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	headsetDevice.NotifySteamVROfMyExistence();
	return vr::EVRInitError::VRInitError_None;
}

void TiltFiveSteamVRDriver::Cleanup()
{
}

const char* const* TiltFiveSteamVRDriver::GetInterfaceVersions()
{
	return vr::k_InterfaceVersions;
}

void TiltFiveSteamVRDriver::RunFrame()
{
}

bool TiltFiveSteamVRDriver::ShouldBlockStandbyMode()
{
	return false;
}

void TiltFiveSteamVRDriver::EnterStandby()
{
}

void TiltFiveSteamVRDriver::LeaveStandby()
{
}
