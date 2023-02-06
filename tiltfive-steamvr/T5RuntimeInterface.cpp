#include "T5RuntimeInterface.h"
#include <Windows.h>

#include "Utils.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

void T5RuntimeInterface::InitializeHeadset(ID3D11Device* pDevice)
{
	//TODO: fill in platformContext (last param).
	auto client = tiltfive::obtainClient("me.davidgoodman.tiltfivesteamvr", "0.1.0", nullptr);
	if (!client) {
		utils::log("Failed to create T5 client\n");
	}

	auto serviceVersion = (*client)->getServiceVersion();

	auto glassesList = (*client)->listGlasses();

	//TODO: add "give up" timeout if no glasses are connected
	while (glassesList->empty())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		glassesList = (*client)->listGlasses();
	}

	for (auto& glassesInstance : *glassesList) {
		utils::log("Found " + glassesInstance);
	}

	auto glassesObtainResult = tiltfive::obtainGlasses(glassesList->front(), *client);
	glasses_ = *glassesObtainResult;

	auto glassesFriendlyName = glasses_->getFriendlyName();
	utils::log("Using " + *glassesFriendlyName);

	auto connectionHelper = glasses_->createConnectionHelper("Tilt Five SteamVR Driver");

	auto connectionResult = connectionHelper->awaitConnection(std::chrono::seconds(10));
	bool isExclusive = false;
	if (connectionResult)
	{
		isExclusive = true;
		utils::log("exclusive glasses connection acquired\n");
	}
	else
		utils::log("exclusive glasses connection failed\n");

	const auto ipdResult = glasses_->getIpd();
	if (ipdResult)
		ipd_ = *ipdResult;

	//we cannnot do anything useful, unless we're in exclusive mode
	if (isExclusive)
		glassesInitialized_ = glasses_->initGraphicsContext(kT5_GraphicsApi_D3D11, (LPVOID)pDevice) ? true : false;
}

vr::DriverPose_t T5RuntimeInterface::GetPose()
{
	vr::DriverPose_t outputPose = { 0 };

	if (!glassesInitialized_)
		return outputPose;

	auto poseResult = glasses_->getLatestGlassesPose(kT5_GlassesPoseUsage_GlassesPresentation);
	if (poseResult) 
	{
		const auto& pose = *poseResult;

		//TODO the co-ordinate system of T5 and of SteamVR do not align
		//TODO maybe you'll want to be able to scale up or down the world so the display in the T5 headset makes any sense.

		outputPose.qRotation = vr::HmdQuaternion_t{
			pose.rotToGLS_GBD.w,
			pose.rotToGLS_GBD.x,
			pose.rotToGLS_GBD.y,
			pose.rotToGLS_GBD.z };

		outputPose.vecPosition[0] = pose.posGLS_GBD.x;
		outputPose.vecPosition[1] = pose.posGLS_GBD.y;
		outputPose.vecPosition[2] = pose.posGLS_GBD.z;

		outputPose.result = vr::ETrackingResult::TrackingResult_Running_OK;
		outputPose.poseIsValid = true;
		outputPose.deviceIsConnected = true;
	}

	else
	{
		outputPose.result = vr::ETrackingResult::TrackingResult_Running_OutOfRange; //Most likely user looked away from gameboard.
	}

	return outputPose;
}

inline T5_Vec3 TranslateByHalfIPD(const float halfIPD, const T5_Quat& headRot, const T5_Vec3& position)
{
	const glm::quat rot(headRot.w, headRot.x, headRot.y, headRot.z);
	const glm::vec3 pos{ position.x, position.y, position.z };
	const glm::vec3 ipdOffset{ halfIPD, 0, 0 };

	const auto out = pos + rot * ipdOffset;
	return { out.x, out.y, out.z };
}

void T5RuntimeInterface::SendFrame(ID3D11Texture2D* eyeTextures[2], const T5_GlassesPose& originalPose)
{
	T5_FrameInfo frameInfo{};
	frameInfo.leftTexHandle = eyeTextures[0];
	frameInfo.rightTexHandle = eyeTextures[1];
	frameInfo.isSrgb = false;
	frameInfo.isUpsideDown = false;

	//It seems it is required to send back the pose of the virtual camera to T5 
	frameInfo.posLVC_GBD = TranslateByHalfIPD(-ipd_ / 2, originalPose.rotToGLS_GBD, originalPose.posGLS_GBD);
	frameInfo.posRVC_GBD = TranslateByHalfIPD(ipd_ / 2, originalPose.rotToGLS_GBD, originalPose.posGLS_GBD);
	frameInfo.rotToLVC_GBD = originalPose.rotToGLS_GBD;
	frameInfo.rotToRVC_GBD = originalPose.rotToGLS_GBD;
	frameInfo.vci.startY_VCI = -tan(glm::radians(renderingFov) * 0.5f);
	frameInfo.vci.startX_VCI = frameInfo.vci.startY_VCI * (float)framebufferWidth / (float)framebufferHeight;
	frameInfo.vci.width_VCI = -2.0f * frameInfo.vci.startX_VCI;
	frameInfo.vci.height_VCI = -2.0f * frameInfo.vci.startY_VCI;
	frameInfo.texHeight_PIX = framebufferHeight;
	frameInfo.texWidth_PIX = framebufferWidth;

	const auto result = glasses_->sendFrame(&frameInfo);
	if(!result)
	{
		utils::log("Failed to send frame to glasses.\n");
	}
}
