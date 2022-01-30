// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

void Console(std::string consoleName)
{
	DWORD b;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &b);
	*(BYTE*)FreeConsole = 0xC3;
	VirtualProtect(&FreeConsole, 1, b, &b);
	AllocConsole();
	freopen_s((FILE**)stdin, "conin$", "r", stdin);
	freopen_s((FILE**)stdout, "conout$", "w", stdout);
	SetConsoleTitleA(consoleName.c_str());
}

DWORD WINAPI ControlThread(LPVOID lpParam)
{
	Console("Koaxy");
	Hooking::Start((HMODULE)lpParam);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ControlThread, hModule, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Hooking::Cleanup();
		break;
	}
	return TRUE;
}


