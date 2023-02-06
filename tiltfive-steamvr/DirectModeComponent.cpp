#include "DirectModeComponent.h"

#include <cassert>

using namespace std::chrono_literals;

void DirectModeComponent::InitD3D()
{
	DWORD createFlags = 0;
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featLevel;

	//TODO: do we need to do some checking to make sure we're using the correct GPU on multi-GPU systems?
	auto result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createFlags, nullptr, 0,
		D3D11_SDK_VERSION, &dxDevice_, &featLevel, &dxDeviceContext_);

	assert(result == S_OK);

	t5RuntimeInterface_.InitializeHeadset(dxDevice_);

	dxInitialized_ = true;

	T5PumpThread = std::thread{ [&]
	{
		while (Tracking)
		{
			std::this_thread::sleep_for(1ms);
			DoTrack();
		}
	} };

	SetThreadDescription((HANDLE)T5PumpThread.native_handle(), L"TiltFive SteamVR Tracking Thread");
	T5PumpThread.detach();

}

DirectModeComponent::DirectModeComponent()
{

}

void DirectModeComponent::CreateSwapTextureSet(uint32_t unPid, const SwapTextureSetDesc_t* pSwapTextureSetDesc, SwapTextureSet_t* pOutSwapTextureSet)
{
	if (!dxInitialized_) 
		InitD3D();


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

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

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

		//Uhh should this be released?
		//resource->Release();
	}


}

void DirectModeComponent::DestroySwapTextureSet(vr::SharedTextureHandle_t sharedTextureHandle)
{

}

void DirectModeComponent::GetNextSwapTextureSetIndex(vr::SharedTextureHandle_t sharedTextureHandles[2], uint32_t(*pIndices)[2])
{
	(*pIndices)[0]++;
	(*pIndices)[0] %= 3;
	(*pIndices)[1]++;
	(*pIndices)[1] %= 3;
}

void DirectModeComponent::SubmitLayer(const SubmitLayerPerEye_t(&perEye)[2])
{
	ID3D11Texture2D* eyeTex[2];
	for (int i = 0; i < 2; ++i)
		dxDevice_->OpenSharedResource((HANDLE)perEye[i].hTexture, __uuidof(ID3D11Texture2D), (void**)&eyeTex[i]);


	t5RuntimeInterface_.SendFrame(eyeTex, t5RuntimeInterface_.GetLastPose());
}

void DirectModeComponent::DoTrack()
{
	if (!t5RuntimeInterface_.IsInitialized())return;

	auto nextFramePose = t5RuntimeInterface_.GetPose();
	vr::VRServerDriverHost()->TrackedDevicePoseUpdated(vr::k_unTrackedDeviceIndex_Hmd, nextFramePose, sizeof(nextFramePose));
}

void DirectModeComponent::Present(vr::SharedTextureHandle_t syncTexture)
{
	//todo: caching so that we don't call opensharedresource every constantly?
	ID3D11Texture2D* dxSyncTex;
	auto hr = dxDevice_->OpenSharedResource((HANDLE)syncTexture, __uuidof(ID3D11Texture2D), (void**)&dxSyncTex);

	IDXGIKeyedMutex* dxgiMutex;
	hr = dxSyncTex->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&dxgiMutex);
	if (FAILED(hr)) {
		return;
	}

	hr = dxgiMutex->AcquireSync(0, 10);
	if (FAILED(hr)) {
		dxgiMutex->Release();
		return;
	}

	std::this_thread::sleep_for(6ms);

	dxgiMutex->ReleaseSync(0);
	dxgiMutex->Release();
}

