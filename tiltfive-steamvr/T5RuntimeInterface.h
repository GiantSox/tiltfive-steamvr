#pragma once
#include "include/TiltFiveNative.hpp"
#include "openvr_driver.h"
#include "d3d11.h"

class T5RuntimeInterface
{
public:
	void InitializeHeadset(ID3D11Device* pDevice);
	vr::DriverPose_t GetPose();

	void SendFrame(ID3D11Texture2D* eyeTextures[2], const T5_GlassesPose& originalPose);

private:
	std::shared_ptr<tiltfive::Glasses> glasses_;

	bool glassesInitialized_ = false;

	double ipd_{ 0 };
	uint16_t framebufferWidth = 1216;
	uint16_t framebufferHeight = 768;
	float renderingFov = 48;
};