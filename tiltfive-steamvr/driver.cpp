#include "driver.h"

vr::EVRInitError TiltFiveSteamVRDriver::Activate(uint32_t unObjectId)
{
	return vr::EVRInitError();
}

void TiltFiveSteamVRDriver::Deactivate()
{
}

void TiltFiveSteamVRDriver::EnterStandby()
{
}

void* TiltFiveSteamVRDriver::GetComponent(const char* pchComponentNameAndVersion)
{
	return nullptr;
}

void TiltFiveSteamVRDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize)
{
}

vr::DriverPose_t TiltFiveSteamVRDriver::GetPose()
{
	return vr::DriverPose_t();
}

void TiltFiveSteamVRDriver::Present(const vr::PresentInfo_t* pPresentInfo, uint32_t unPresentInfoSize)
{
}

void TiltFiveSteamVRDriver::WaitForPresent()
{
}

bool TiltFiveSteamVRDriver::GetTimeSinceLastVsync(float* pfSecondsSinceLastVsync, uint64_t* pulFrameCounter)
{
	return false;
}
