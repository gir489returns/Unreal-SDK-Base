// dllmain.cpp : Defines the entry point for the DLL application.
#include "SDK.hpp"
#include "MinHook/include/MinHook.h"
#include "CommonVariables.hpp"
#include "PatternScanner.hpp"

#ifdef DXGI_WRAPPER
#ifdef _WIN64
#define DLLPATH "\\\\.\\GLOBALROOT\\SystemRoot\\System32\\dxgi.dll"
#else
#define DLLPATH "\\\\.\\GLOBALROOT\\SystemRoot\\SysWOW64\\dxgi.dll"
#endif // _WIN64

#pragma comment(linker, "/EXPORT:ApplyCompatResolutionQuirking=" DLLPATH ".ApplyCompatResolutionQuirking")
#pragma comment(linker, "/EXPORT:CompatString=" DLLPATH ".CompatString")
#pragma comment(linker, "/EXPORT:CompatValue=" DLLPATH ".CompatValue")
#pragma comment(linker, "/EXPORT:CreateDXGIFactory=" DLLPATH ".CreateDXGIFactory")
#pragma comment(linker, "/EXPORT:CreateDXGIFactory1=" DLLPATH ".CreateDXGIFactory1")
#pragma comment(linker, "/EXPORT:CreateDXGIFactory2=" DLLPATH ".CreateDXGIFactory2")
#pragma comment(linker, "/EXPORT:DXGID3D10CreateDevice=" DLLPATH ".DXGID3D10CreateDevice")
#pragma comment(linker, "/EXPORT:DXGID3D10CreateLayeredDevice=" DLLPATH ".DXGID3D10CreateLayeredDevice")
#pragma comment(linker, "/EXPORT:DXGID3D10GetLayeredDeviceSize=" DLLPATH ".DXGID3D10GetLayeredDeviceSize")
#pragma comment(linker, "/EXPORT:DXGID3D10RegisterLayers=" DLLPATH ".DXGID3D10RegisterLayers")
#pragma comment(linker, "/EXPORT:DXGIDeclareAdapterRemovalSupport=" DLLPATH ".DXGIDeclareAdapterRemovalSupport")
#pragma comment(linker, "/EXPORT:DXGIDisableVBlankVirtualization=" DLLPATH ".DXGIDisableVBlankVirtualization")
#pragma comment(linker, "/EXPORT:DXGIDumpJournal=" DLLPATH ".DXGIDumpJournal")
#pragma comment(linker, "/EXPORT:DXGIGetDebugInterface1=" DLLPATH ".DXGIGetDebugInterface1")
#pragma comment(linker, "/EXPORT:DXGIReportAdapterConfiguration=" DLLPATH ".DXGIReportAdapterConfiguration")
#pragma comment(linker, "/EXPORT:PIXBeginCapture=" DLLPATH ".PIXBeginCapture")
#pragma comment(linker, "/EXPORT:PIXEndCapture=" DLLPATH ".PIXEndCapture")
#pragma comment(linker, "/EXPORT:PIXGetCaptureState=" DLLPATH ".PIXGetCaptureState")
#pragma comment(linker, "/EXPORT:SetAppCompatStringPointer=" DLLPATH ".SetAppCompatStringPointer")
#pragma comment(linker, "/EXPORT:UpdateHMDEmulationStatus=" DLLPATH ".UpdateHMDEmulationStatus")
#endif

HANDLE main_thread{};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        static auto this_module = hModule;
        main_thread = CreateThread(nullptr, 0,
            [](PVOID) -> DWORD {
                MH_Initialize();

                //Do hooks here.

#ifndef DLL_WRAPPER
                while (!GetAsyncKeyState(VK_F10) & 1) {}

                vars::is_running = false;

                //Disable your hookos here.
                MH_Uninitialize();

                CloseHandle(main_thread);
                FreeLibraryAndExitThread(this_module, 0);
#else
                ExitThread(0);
#endif
            }, nullptr, 0, nullptr);
    }
    return TRUE;
}

