#pragma once
#include "openvr_driver.h"
#include <d3d11_4.h>
#include "T5RuntimeInterface.h"
#include <thread>

class DirectModeComponent : vr::IVRDriverDirectModeComponent
{
public:
	DirectModeComponent();

	void CreateSwapTextureSet(uint32_t unPid, const SwapTextureSetDesc_t* pSwapTextureSetDesc, SwapTextureSet_t* pOutSwapTextureSet) override;
	void DestroySwapTextureSet(vr::SharedTextureHandle_t sharedTextureHandle) override;
	void GetNextSwapTextureSetIndex(vr::SharedTextureHandle_t sharedTextureHandles[2], uint32_t(*pIndices)[2]) override;
	void SubmitLayer(const SubmitLayerPerEye_t(&perEye)[2]) override;
	void Present(vr::SharedTextureHandle_t syncTexture) override;

	void DoTrack();

protected:

	void InitD3D();


	bool dxInitialized_ = false;
	ID3D11Device* dxDevice_ = nullptr;
	ID3D11DeviceContext* dxDeviceContext_ = nullptr;

	T5RuntimeInterface t5RuntimeInterface_;

	std::thread T5PumpThread;
	std::atomic_bool Tracking = true;
};

