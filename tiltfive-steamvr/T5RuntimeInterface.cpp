#include "T5RuntimeInterface.h"
#include <Windows.h>

void T5RuntimeInterface::InitializeHeadset()
{
	//TODO: fill in platformContext (last param). Prob used for gfx init
	auto client = tiltfive::obtainClient("me.davidgoodman.tiltfivesteamvr", "0.1.0", nullptr);
	if (!client) {
		OutputDebugStringA("Failed to create T5 client\n");
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
		OutputDebugStringA(strcat("Found ", glassesInstance.c_str()));
	}

	auto glassesObtainResult = tiltfive::obtainGlasses(glassesList->front(), *client);
	glasses_ = *glassesObtainResult;

	auto glassesFriendlyName = glasses_->getFriendlyName();
	OutputDebugStringA(strcat("Using ", (*glassesFriendlyName).c_str()));

	auto connectionHelper = glasses_->createConnectionHelper("Tilt Five SteamVR Driver");
	auto connectionResult = connectionHelper->awaitConnection(std::chrono::seconds(10));
	if (connectionResult)
		OutputDebugStringA("exclusive glasses connection acquired\n");
	else
		OutputDebugStringA("exclusive glasses connection failed\n");

	glassesInitialized_ = true;
}

vr::DriverPose_t T5RuntimeInterface::GetPose()
{
	vr::DriverPose_t outputPose = { 0 };


	if (!glassesInitialized_)
		return outputPose;

	auto poseResult = glasses_->getLatestGlassesPose();
	if (poseResult) {
		const auto& pose = poseResult;

		outputPose.qRotation = vr::HmdQuaternion_t{ 
			pose->rotToGLS_GBD.w, pose->rotToGLS_GBD.x, pose->rotToGLS_GBD.y, pose->rotToGLS_GBD.z};
		outputPose.vecPosition[0] = pose->posGLS_GBD.x;
		outputPose.vecPosition[1] = pose->posGLS_GBD.y;
		outputPose.vecPosition[2] = pose->posGLS_GBD.z;

		outputPose.result = vr::ETrackingResult::TrackingResult_Running_OK;
		outputPose.poseIsValid = true;
		outputPose.deviceIsConnected = true;
		
	}

	return outputPose;
	

}