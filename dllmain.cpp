#include "Windows.H"
#include "pch.h"
#include <cstdio>
#include <Minhook.h>

uintptr_t gamebase = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssemblyDLL = (uintptr_t)GetModuleHandle("GameAssembly.dll");
uintptr_t FieldOfViewOffset = 0x54FBE0;


void Console()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}



void CallMinhookAndConsole()
{
    MH_Initialize();
    Console();
}

void(__fastcall* fieldofview_original)(DWORD*, float, DWORD*);
void __stdcall set_fieldofview(DWORD* __this, float value, DWORD* method)
{
    value = 120.f;
    printf("function called");

    return fieldofview_original(__this, value, method);

}


void Main()
{
    CallMinhookAndConsole();
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssemblyDLL + FieldOfViewOffset), &set_fieldofview, (LPVOID*)&fieldofview_original);
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssemblyDLL + FieldOfViewOffset));
}




BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

