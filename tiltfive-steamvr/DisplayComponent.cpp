#include "DisplayComponent.h"

DisplayComponent::DisplayComponent()
{
}

DisplayComponent::~DisplayComponent()
{
}

void DisplayComponent::GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight)
{
	//Not relevant, we're not using extended mode.
	*pnX = 0;
	*pnY = 0;
	*pnWidth = 1216;
	*pnHeight = 768;
}

bool DisplayComponent::IsDisplayOnDesktop()
{
	return false;
}

bool DisplayComponent::IsDisplayRealDisplay()
{
	//WTF is this API supposed to do?
	//Apparently SteamVR tries to use my monitor as an extended mode display if I set this to false?
	//Shouldn't that be what IsDisplayOnDesktop() does? Estoy confundido...
	return true;
}

void DisplayComponent::GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight)
{
	//TODO: obviously these need to be changed to real values.
	*pnWidth = 1216;
	*pnHeight = 768;
}

void DisplayComponent::GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight)
{
	*pnX = 0;
	*pnY = 0;
	*pnWidth = 1216;
	*pnHeight = 768;
}

void DisplayComponent::GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom)
{
	//Let's just pretend each eye has a 90x90 FOV for now.

	static float deg_to_rad = 3.14159265359/180;
	float halfEyeFov = 45 * deg_to_rad;

	*pfLeft = -tan(halfEyeFov);
	*pfRight = tan(halfEyeFov);
	*pfTop = -tan(halfEyeFov);
	*pfBottom = tan(halfEyeFov);

}

vr::DistortionCoordinates_t DisplayComponent::ComputeDistortion(vr::EVREye eEye, float fU, float fV)
{
	//We're going to assume that the compositor we're submitting the textures to handles all distortion correction.
	static vr::DistortionCoordinates_t noDistortion;


	noDistortion.rfRed[0] = fU;
	noDistortion.rfGreen[0] = fU;
	noDistortion.rfBlue[0] = fU;

	noDistortion.rfRed[1] = fU;
	noDistortion.rfGreen[1] = fU;
	noDistortion.rfBlue[1] = fU;

	return noDistortion;
}
