#pragma once
#include "openvr_driver.h"
#include "DisplayComponent.h"
#include "DirectModeComponent.h"

class T5Headset : public vr::ITrackedDeviceServerDriver
{
public:
	T5Headset()
	{

	}

	//// ------------------------------------
	//// Management Methods
	//// ------------------------------------
	///** This is called before an HMD is returned to the application. It will always be
	//* called before any display or tracking methods. Memory and processor use by the
	//* ITrackedDeviceServerDriver object should be kept to a minimum until it is activated.
	//* The pose listener is guaranteed to be valid until Deactivate is called, but
	//* should not be used after that point. */
	//virtual vr::EVRInitError Activate(uint32_t unObjectId) override;

	///** This is called when The VR system is switching from this Hmd being the active display
	//* to another Hmd being the active display. The driver should clean whatever memory
	//* and thread use it can when it is deactivated */
	//virtual void Deactivate() override;

	///** Handles a request from the system to put this device into standby mode. What that means is defined per-device. */
	//virtual void EnterStandby() override;

	///** Requests a component interface of the driver for device-specific functionality. The driver should return NULL
	//* if the requested interface or version is not supported. */
	//virtual void* GetComponent(const char* pchComponentNameAndVersion) override;

	///** A VR Client has made this debug request of the driver. The set of valid requests is entirely
	//* up to the driver and the client to figure out, as is the format of the response. Responses that
	//* exceed the length of the supplied buffer should be truncated and null terminated */
	//virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;


	void NotifySteamVROfMyExistence();

	// Inherited via ITrackedDeviceServerDriver
	vr::EVRInitError Activate(uint32_t unObjectId) override;

	void Deactivate() override;

	void EnterStandby() override;

	void* GetComponent(const char* pchComponentNameAndVersion) override;

	void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;

	vr::DriverPose_t GetPose() override;

protected:
	DisplayComponent displayComponent_;
	DirectModeComponent directModeComponent_;
};
