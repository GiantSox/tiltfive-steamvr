#pragma once
#include "openvr_driver.h"
#include "t5headset.h"

class TiltFiveSteamVRDriver : public vr::IServerTrackedDeviceProvider
{
public:
	// Inherited via IServerTrackedDeviceProvider
	vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override;
	void Cleanup() override;
	const char* const* GetInterfaceVersions() override;
	void RunFrame() override;
	bool ShouldBlockStandbyMode() override;
	void EnterStandby() override;
	void LeaveStandby() override;

protected:
	T5Headset headsetDevice;
};