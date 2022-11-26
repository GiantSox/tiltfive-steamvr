#include "driver.h"
#include <Windows.h>

vr::EVRInitError TiltFiveSteamVRDriver::Init(vr::IVRDriverContext* pDriverContext)
{
	MessageBoxA(nullptr, "Do it you won't", "Attach a debugger now poopoohead", MB_ICONINFORMATION | MB_OK);
	headsetDevice.NotifySteamVROfMyExistence();
	return vr::EVRInitError::VRInitError_None;
}

void TiltFiveSteamVRDriver::Cleanup()
{
}

const char* const* TiltFiveSteamVRDriver::GetInterfaceVersions()
{
	return nullptr;
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
