#pragma once
#include "../deps/openvr/openvr_driver.h"

class DisplayComponent : vr::IVRDisplayComponent
{
public:
	DisplayComponent();
	~DisplayComponent();

private:


	// Inherited via IVRDisplayComponent
	virtual void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override;

	virtual bool IsDisplayOnDesktop() override;

	virtual bool IsDisplayRealDisplay() override;

	virtual void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) override;

	virtual void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override;

	virtual void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) override;

	virtual vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV) override;

};