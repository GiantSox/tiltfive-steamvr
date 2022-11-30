#include "DirectModeComponent.h"

DirectModeComponent::DirectModeComponent()
{
	DWORD createFlags = 0;
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featLevel;

	//TODO: do we need to do some checking to make sure we're using the correct GPU on multi-GPU systems?
	auto result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createFlags, nullptr, 0,
		D3D11_SDK_VERSION, &dxDevice_, &featLevel, &dxDeviceContext_);

}

void DirectModeComponent::CreateSwapTextureSet(uint32_t unPid, const SwapTextureSetDesc_t* pSwapTextureSetDesc, SwapTextureSet_t* pOutSwapTextureSet)
{
	auto texhandle = pOutSwapTextureSet->rSharedTextureHandles;

	D3D11_TEXTURE2D_DESC desc;
	//this feels hacky af, but it appears other drivers just assume nFormat is a DXGI_FORMAT. Valve did say this API is Windows-only...
	desc.Format = (DXGI_FORMAT)pSwapTextureSetDesc->nFormat;
	desc.Width = pSwapTextureSetDesc->nWidth;
	desc.Height = pSwapTextureSetDesc->nHeight;

	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = pSwapTextureSetDesc->nSampleCount;
	desc.SampleDesc.Quality = 0;

	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

	//Gonna create 3 textures because that's what ALVR does. Dunno if there's documentation for this anywhere.

	for (int i = 0; i < 3; i++)
	{
		ID3D11Texture2D* texture;
		auto result = dxDevice_->CreateTexture2D(&desc, NULL, &texture);

		IDXGIResource* resource;
		result = texture->QueryInterface(__uuidof(IDXGIResource), (void**)&resource);

		HANDLE sharedHandle;
		result = resource->GetSharedHandle(&sharedHandle);

		pOutSwapTextureSet->rSharedTextureHandles[i] = (vr::SharedTextureHandle_t)sharedHandle;

		resource->Release();
	}


}

void DirectModeComponent::DestroySwapTextureSet(vr::SharedTextureHandle_t sharedTextureHandle)
{
}

void DirectModeComponent::SubmitLayer(const SubmitLayerPerEye_t(&perEye)[2])
{
}

void DirectModeComponent::Present(vr::SharedTextureHandle_t syncTexture)
{
}
