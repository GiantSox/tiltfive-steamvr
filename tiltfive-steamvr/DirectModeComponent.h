#pragma once
#include "../deps/openvr/openvr_driver.h"
#include <d3d11_4.h>
//#pragma comment(lib, "d3d11.lib")

class DirectModeComponent : vr::IVRDriverDirectModeComponent
{
public:
	DirectModeComponent();

	virtual void CreateSwapTextureSet(uint32_t unPid, const SwapTextureSetDesc_t* pSwapTextureSetDesc, SwapTextureSet_t* pOutSwapTextureSet);
	virtual void DestroySwapTextureSet(vr::SharedTextureHandle_t sharedTextureHandle);
	virtual void GetNextSwapTextureSetIndex(vr::SharedTextureHandle_t sharedTextureHandles[2], uint32_t(*pIndices)[2]);
	virtual void SubmitLayer(const SubmitLayerPerEye_t(&perEye)[2]);
	virtual void Present(vr::SharedTextureHandle_t syncTexture);


protected:

	void InitD3D();

	bool dxInitialized_ = false;
	ID3D11Device* dxDevice_ = nullptr;
	ID3D11DeviceContext* dxDeviceContext_ = nullptr;
};

