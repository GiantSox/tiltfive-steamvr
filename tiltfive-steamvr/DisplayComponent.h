#pragma once
#include "../deps/openvr/openvr_driver.h"

class DisplayComponent : vr::IVRDisplayComponent
{
public:
	DisplayComponent();
	~DisplayComponent();

private:


	// Inherited via IVRDisplayComponent
	void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override;

	bool IsDisplayOnDesktop() override;

	bool IsDisplayRealDisplay() override;

	void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) override;

	void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override;

	void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) override;

	vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV) override;

};