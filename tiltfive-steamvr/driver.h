#pragma once
#include "../deps/openvr/openvr_driver.h"
#include "t5headset.h"

class TiltFiveSteamVRDriver : public vr::IServerTrackedDeviceProvider
{
public:
	// Inherited via IServerTrackedDeviceProvider
	virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override;
	virtual void Cleanup() override;
	virtual const char* const* GetInterfaceVersions() override;
	virtual void RunFrame() override;
	virtual bool ShouldBlockStandbyMode() override;
	virtual void EnterStandby() override;
	virtual void LeaveStandby() override;

protected:
	T5Headset headsetDevice;
};