#include "T5RuntimeInterface.h"
#include <Windows.h>

#include "Utils.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <format>
void T5RuntimeInterface::InitializeHeadset(ID3D11Device* pDevice)
{
	//Cache d3d11 pointers
	d3d11device = pDevice;
	d3d11device->GetImmediateContext(&d3d11ctx);


	T5_ClientInfo clientInfo{
		"me.davidgoodman.tiltfivesteamvr", "0.1.0", 0,0
	};

	if(auto status = t5CreateContext(&t5ctx, &clientInfo, nullptr); status != T5_SUCCESS)
	{
		// :'-(
	}

	{
		bool waiting = false;
		for (;;)
		{
			size_t bufferSize = T5_MAX_STRING_PARAM_LEN;
			auto err = t5GetSystemUtf8Param(t5ctx, kT5_ParamSys_UTF8_Service_Version, serviceVersion, &bufferSize);

			if (!err)
			{
				//std::cout << "Tilt Five service version : " << serviceVersion << std::endl;
				break; //service found.
			}

			if (T5_ERROR_NO_SERVICE == err)
			{
				if (!waiting)
				{
					//std::cout << "Waiting for TiltFive service ... \n";
					//TODO print "waiting for service ..."
					waiting = true;
				}
			}
			else
			{
				const auto errorMessage = t5GetResultMessage(err);
				//std::cout << "Failed to obtain TiltFive service version : " << errorMessage << std::endl;
				//return false;
			}
		}
	}

	{
		size_t bufferSize = GLASSES_BUFFER_SIZE;
		char glassesListBuffer[GLASSES_BUFFER_SIZE];
		auto err = t5ListGlasses(t5ctx, glassesListBuffer, &bufferSize);

		if (!err)
		{
			const char* buffPtr = glassesListBuffer;
			for (;;)
			{
				const auto nameLen = strlen(buffPtr);
				if (nameLen == 0) break;

				std::string glassesName{ buffPtr, nameLen };
				buffPtr += nameLen;

				glassesSerialNumber.push_back(glassesName);

				if (buffPtr > glassesListBuffer + GLASSES_BUFFER_SIZE)
				{
					//std::cout << "WARN: truncated glasses list buffer\n";
					break;
				}
			}
		}

		if (glassesSerialNumber.empty())
		{
			//std::cout << "Did not detect any Tilt Five glasses.\n";
			//return false;
		}

		//std::cout << "found " << glassesSerialNumber.size() << " TiltFive glasses:\n";
		//for (const auto& glassesSN : glassesSerialNumber) { std::cout << "\t- " << glassesSN << "\n"; }

		singleUserGlassesSN = glassesSerialNumber.front();

		err = t5CreateGlasses(t5ctx, singleUserGlassesSN.c_str(), &glassesHandle);
		if (err)
		{
			//std::cout << "Error creating glasses " << singleUserGlassesSN << ": " << t5GetResultMessage(err);
			//return false;
		}

		char glassesName[T5_MAX_STRING_PARAM_LEN]{ 0 };
		size_t glassesNameSize = T5_MAX_STRING_PARAM_LEN;
		err = t5GetGlassesUtf8Param(glassesHandle, 0, kT5_ParamGlasses_UTF8_FriendlyName, glassesName, &glassesNameSize);
		if (!err)
		{
			singleUserGlassesFriendlyName = glassesName;
			//std::cout << "Galsses friendly name : " << singleUserGlassesFriendlyName << std::endl;
		}

		//TODO check connection routine :
		T5_ConnectionState connectionState;
		err = t5GetGlassesConnectionState(glassesHandle, &connectionState);

		//if (err) { return false; }

		err = t5ReserveGlasses(glassesHandle, "SteamVR Compositor");
		if (err)
		{
			//std::cout << "Failed to reserve glasses : " << t5GetResultMessage(err) << std::endl;
			//return false;
		}

		for (;;)
		{
			err = t5EnsureGlassesReady(glassesHandle);
			if (T5_ERROR_TRY_AGAIN == err) { continue; }

			break;
		}

		glassesInitialized_ = t5InitGlassesGraphicsContext(glassesHandle, kT5_GraphicsApi_D3D11, pDevice) == T5_SUCCESS;
	}
}

vr::DriverPose_t T5RuntimeInterface::GetPose()
{
	vr::DriverPose_t outputPose = { 0 };

	//normalize these quaternions, but keep them identity.
	outputPose.qDriverFromHeadRotation.w = 1;
	outputPose.qWorldFromDriverRotation.w = 1;

	if (!glassesInitialized_)
		return outputPose;

	T5_GlassesPose pose;
	auto err = t5GetGlassesPose(glassesHandle, kT5_GlassesPoseUsage_GlassesPresentation, &pose);

	if (!err)
	{
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
		lastPose_ = pose;

		OutputDebugStringA(std::format("pose pos({},{},{}) rot({},{},{},{})\n",
			outputPose.vecPosition[0],
			outputPose.vecPosition[1],
			outputPose.vecPosition[2],
			outputPose.qRotation.x,
			outputPose.qRotation.y,
			outputPose.qRotation.z,
			outputPose.qRotation.w
			).c_str());

		//TODO since we have timestamps, we can cache the last frame and derive position over time to get speed
		//TODO of the TODO since we will compute speed as mentioned by the above TODO item, we can derive  speed over time to compute acceleration
		//NOTODO thankfully VR developers do not need jerk vectors. But we could derive  acceleration over time to computer jerk	
		//NOTODO of the NOTODO in case you actually wanted to know about jerk, you might be interested in the rate of change of jerk. We could also derive jerk to get snap vectors!
		//TODONT of the NOTODO of the NOTODO, could also derive  the snap to get the crackle.
		//REALLY I SHOULD GO TO SLEEP TODO item: Could always derive  crackle to get pop. I do not even know in what field of engineering you actually need that.
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
	//You'll excuse me for this oh so horrible hack
	static ID3D11Texture2D* intermediateTextures[2] = { nullptr, nullptr };
	for (int i = 0; i < 2; ++i)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		eyeTextures[i]->GetDesc(&textureDesc);
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT; //SteamVR outputs _UNORM. as of NDK 1.3.0, T5 only compatible with UINT. 

		//Lazilly allocate intermediate buffers. Also reallocate them in case they change sizes or
		//any other metadata (unlikely)
		if (!intermediateTextures[i]) [[unlikely]]
		{
		create:
			d3d11device->CreateTexture2D(&textureDesc, nullptr, &intermediateTextures[i]);
		}
		else
		{
			D3D11_TEXTURE2D_DESC intermedateDesc;
			intermediateTextures[i]->GetDesc(&intermedateDesc);
			if (0 != memcmp(&intermedateDesc, &textureDesc, sizeof(D3D11_TEXTURE2D_DESC))) [[unlikely]]
			{
				intermediateTextures[i]->Release();
				intermediateTextures[i] = nullptr;
				goto create;
			}
		}
		d3d11ctx->CopyResource(intermediateTextures[i], eyeTextures[i]);
	}

	T5_FrameInfo frameInfo{};
	frameInfo.leftTexHandle = intermediateTextures[0];
	frameInfo.rightTexHandle = intermediateTextures[1];
	frameInfo.isSrgb = true;
	frameInfo.isUpsideDown = true;

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

	auto err = t5SendFrameToGlasses(glassesHandle, &frameInfo);
	if (err)
	{
		utils::log("Failed to send frame to glasses.\n");
	}
}

T5RuntimeInterface::~T5RuntimeInterface()
{
	if (IsInitialized())
	{
		t5DestroyGlasses(&glassesHandle);
		t5DestroyContext(&t5ctx);
	}
}
