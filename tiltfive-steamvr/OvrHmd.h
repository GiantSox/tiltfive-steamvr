#pragma once
#include "../deps/openvr/openvr_driver.h"

class OvrHmd : public vr::ITrackedDeviceServerDriver,
    vr::IVRDisplayComponent {

    virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);
    virtual void Deactivate();
    virtual void EnterStandby() {}
    void* GetComponent(const char* pchComponentNameAndVersion);
    virtual void DebugRequest(const char* request, char* response_buffer, uint32_t size) {}
    virtual vr::DriverPose_t GetPose();

    //void OnPoseUpdated(uint64_t targetTimestampNs, float predictionS, AlvrDeviceMotion motion);

    void StartStreaming();

    void OnStreamStart();

    //void SetViewsConfig(ViewsConfigData config);

    //bool IsTrackingRef() const { return m_deviceClass == vr::TrackedDeviceClass_TrackingReference; }
    //bool IsHMD() const { return m_deviceClass == vr::TrackedDeviceClass_HMD; }



    // IVRDisplayComponent

    virtual void GetWindowBounds(int32_t* x, int32_t* y, uint32_t* width, uint32_t* height);
    virtual bool IsDisplayOnDesktop() { return false; }
    virtual bool IsDisplayRealDisplay();
    virtual void GetRecommendedRenderTargetSize(uint32_t* width, uint32_t* height);
    virtual void GetEyeOutputViewport(
        vr::EVREye eye, uint32_t* x, uint32_t* y, uint32_t* width, uint32_t* height);
    virtual void
        GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom);
    virtual vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV);
};