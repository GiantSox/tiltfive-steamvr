#pragma once
#include "../deps/openvr/openvr_driver.h"

class TiltFiveSteamVRDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRVirtualDisplay 
{
	//ITrackedDeviceServerDriver:


	// ------------------------------------
	// Management Methods
	// ------------------------------------
	/** This is called before an HMD is returned to the application. It will always be
	* called before any display or tracking methods. Memory and processor use by the
	* ITrackedDeviceServerDriver object should be kept to a minimum until it is activated.
	* The pose listener is guaranteed to be valid until Deactivate is called, but
	* should not be used after that point. */
	virtual vr::EVRInitError Activate(uint32_t unObjectId) override;

	/** This is called when The VR system is switching from this Hmd being the active display
	* to another Hmd being the active display. The driver should clean whatever memory
	* and thread use it can when it is deactivated */
	virtual void Deactivate() override;

	/** Handles a request from the system to put this device into standby mode. What that means is defined per-device. */
	virtual void EnterStandby() override;

	/** Requests a component interface of the driver for device-specific functionality. The driver should return NULL
	* if the requested interface or version is not supported. */
	virtual void* GetComponent(const char* pchComponentNameAndVersion) override;

	/** A VR Client has made this debug request of the driver. The set of valid requests is entirely
	* up to the driver and the client to figure out, as is the format of the response. Responses that
	* exceed the length of the supplied buffer should be truncated and null terminated */
	virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;

	// ------------------------------------
	// Tracking Methods
	// ------------------------------------
	virtual vr::DriverPose_t GetPose() override;



	//IVRVirtualDisplay:

	/** Submits final backbuffer for display. */
	virtual void Present(const vr::PresentInfo_t* pPresentInfo, uint32_t unPresentInfoSize) override;

	/** Block until the last presented buffer start scanning out. */
	virtual void WaitForPresent() override;

	/** Provides timing data for synchronizing with display. */
	virtual bool GetTimeSinceLastVsync(float* pfSecondsSinceLastVsync, uint64_t* pulFrameCounter) override;

};