#include "t5headset.h"

void T5Headset::NotifySteamVROfMyExistence()
{
	vr::VRServerDriverHost()->TrackedDeviceAdded("tiltfive-steamvr", vr::ETrackedDeviceClass::TrackedDeviceClass_HMD, this);
}

vr::EVRInitError T5Headset::Activate(uint32_t unObjectId)
{
	if(unObjectId == 0)
	{
		auto vr_properties = vr::VRProperties();
		auto prop_container = vr_properties->TrackedDeviceToPropertyContainer(unObjectId);
		vr_properties->SetStringProperty(prop_container, vr::Prop_TrackingSystemName_String, "TiltFive");
	}
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
	if (0 == stricmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)) {
		return &displayComponent_;
	}
	else if (0 == stricmp(pchComponentNameAndVersion, vr::IVRDriverDirectModeComponent_Version)) {
		return &directModeComponent_;
	}
	
	return nullptr;
}

void T5Headset::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize)
{
}

vr::DriverPose_t T5Headset::GetPose()
{
	vr::HmdQuaternion_t zeroQuat{
		1,0,0,0
	};

	vr::DriverPose_t pose = { 0 };
	pose.deviceIsConnected = true;
	pose.poseIsValid = true;
	pose.poseTimeOffset = 0;	//FIXME
	pose.qDriverFromHeadRotation = zeroQuat;
	pose.qRotation = zeroQuat;
	pose.qWorldFromDriverRotation = zeroQuat;
	pose.result = vr::ETrackingResult::TrackingResult_Running_OK;
	pose.shouldApplyHeadModel = false;

	/*pose.vecAngularAcceleration = zeroTranslation;
	pose.vecAngularVelocity = zeroTranslation;
	pose.vecDriverFromHeadTranslation = zeroTranslation;
	pose.vecPosition = zeroTranslation;
	pose.vecVelocity = zeroTranslation;
	pose.*/
	return pose;
}
