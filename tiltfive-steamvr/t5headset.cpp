#include "t5headset.h"

void T5Headset::NotifySteamVROfMyExistence()
{
	vr::VRServerDriverHost()->TrackedDeviceAdded("tiltfive-steamvr", vr::ETrackedDeviceClass::TrackedDeviceClass_HMD, this);
}

vr::EVRInitError T5Headset::Activate(uint32_t unObjectId)
{
	return vr::EVRInitError::VRInitError_None;
}

void T5Headset::Deactivate()
{
}

void T5Headset::EnterStandby()
{
}

void* T5Headset::GetComponent(const char* pchComponentNameAndVersion)
{
	return nullptr;
}

void T5Headset::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize)
{
}

vr::DriverPose_t T5Headset::GetPose()
{
	vr::DriverPose_t pose;
	pose.deviceIsConnected = true;
	pose.poseIsValid = true;
	pose.poseTimeOffset = 0;	//FIXME
	//pose.
	return pose;
}
