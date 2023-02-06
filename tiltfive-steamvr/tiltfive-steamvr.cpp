#include "tiltfive-steamvr.h"
#include "driver.h"
#include "Utils.hpp"

//I may or may not delete this comment later, but I've been having a very hard time focusing these past few days,
// and can't really tell if journaling or working on this project is going to be the best use of my time right now.
// So I figured I'd meet in the middle somehow, and use commments as my mental scratchpad, and try to give myself the freedom
// to do whatever is going through my head at a given moment, even if writing code is too hard.
//
//Reestablishing the structure that work gave me for personal projects is taking some time, and it's not easy when
// other life chaos is going on at the same time.
//
//Ugh, it'd be so nice if there wasn't COVID in the household. I'm sitting and working outside right now, since I figured it'd
// be my best bet at getting some focus time in, whatever I end up doing with it. It really stinks not feeling like you can relax
// in your own home. But getting COVID right now would be pretty inconvenient, so I'm just gonna cross my fingers I haven't gotten it,
// that I'm able to be productive and happy in my temporary location, and that I can return home very soon.
// 
// With that out of the way, what are we gonna work on now?
// 
// So I remember I was implementing the DriverDirectMode interface, and copied some code from ALVR. Maybe a better place to start would
// be going up the chain from ALVR's HmdDriverFactory, and figuring out how to actually provide the HMD object to SteamVR.
// - Look at the Rust code first. 
// - Pull it down if necessary
// - Document and understand how it works.
// - Take a break, and process it, and come back to plan next steps.
// 
// Are code comments usually so personal? :P
// 
// The Twitter verification drama is on my mind too, but I'll try not to think about that too much.
//

//Here we go. HmdDriverFactory notes:
// - ALVR passes a CServerDriver_DisplayRedirect object back through the factory func. What is it?
// - SDDR (see above) derives from IServerTrackedDeviceProvider
// - SDDR owns serveral objects: a CRemoteHmd, a Listener, and an IPCMutex, all are initialized in SDDR::Init()
// - CRemoteHmd dervies from ItrackedDeviceServerDriver, which according to the docs, reps 1 tracked device
// - I don't quite understand how CRemoteHmd connects to the OpenVR API. Gonna read through its functions/cbs.
//     - It turns out SDDR initializes CRemoteHmd by calling CRH::Enable()
//
// - CRH calls vr::VRServerDriverHost()->TrackedDeviceAdded() and passes itself to SteamVR. THIS TELLS STEAMVR DEVICE EXIST
//     - Afterwards, SteamVR will call the device's Activate() method.
//	   - Inside Activate(), CRH sets a bunch of properties like FPS, ipd, and model number (alvr_server.cpp:824)
//     - Inside Activate(), CRH constructs a DisplayComponent and DirectModeComponent
//         - (and a VsyncThread, CD3DRender and CEncoder, but idk how relevant those are)
// 
// I think it's time for a pee and a break.
//
//


#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )

TiltFiveSteamVRDriver driverInstance;

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {

	if (strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName) == 0) {
		return &driverInstance;
	}

	if (pReturnCode)
		*pReturnCode = vr::VRInitError_Init_InterfaceNotFound;

	return NULL;
}


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
		MessageBoxA(nullptr, "Do it you won't", "Attach a debugger now poopoohead", MB_ICONINFORMATION | MB_OK);
        utils::OpenDebugConsole(); 
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}