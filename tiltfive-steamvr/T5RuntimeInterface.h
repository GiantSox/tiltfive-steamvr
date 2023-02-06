#pragma once
#include "include/TiltFiveNative.hpp"
#include "../deps/openvr/openvr_driver.h"

class T5RuntimeInterface
{
public:
	void InitializeHeadset();
	vr::DriverPose_t GetPose();

private:
	std::shared_ptr<tiltfive::Glasses> glasses_;

	bool glassesInitialized_ = false;
};