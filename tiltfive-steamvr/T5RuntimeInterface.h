#pragma once
#include "include/TiltFiveNative.h"
#include "openvr_driver.h"
#include "d3d11.h"

class T5RuntimeInterface
{
public:
	void InitializeHeadset(ID3D11Device* pDevice);
	vr::DriverPose_t GetPose();

	//Must *only* be called for a frame where a valid T5 pose was acquired. Failing to do so result in silent asyncrhoneous errors from the TiltFiveNative library.
	void SendFrame(ID3D11Texture2D* eyeTextures[2], const T5_GlassesPose& originalPose);


	[[nodiscard]] bool IsInitialized() const { return glassesInitialized_; }

	~T5RuntimeInterface();

	T5_GlassesPose GetLastPose() const
	{	
		return lastPose_;
	}

private:

	T5_Context t5ctx;
	T5_Glasses glassesHandle;
	T5_GlassesPose lastPose_;

	bool glassesInitialized_ = false;

	double ipd_{ 0 };
	uint16_t framebufferWidth = 1216;
	uint16_t framebufferHeight = 768;
	float renderingFov = 48;

	static constexpr size_t GLASSES_BUFFER_SIZE{ 1024 };
	static constexpr size_t WAND_BUFFER_SIZE{ 4 };
	static constexpr size_t PARAM_BUFFER_SIZE{ 1024 };

	std::vector<std::string> glassesSerialNumber;
	std::string singleUserGlassesSN;
	std::string singleUserGlassesFriendlyName;
	char serviceVersion[T5_MAX_STRING_PARAM_LEN];

	ID3D11Device* d3d11device;
	ID3D11DeviceContext* d3d11ctx;
};