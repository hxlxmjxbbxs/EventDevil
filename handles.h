#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>

HANDLE serviceProcessHandle;
HANDLE snapshotHandle;
HANDLE threadHandle;
SERVICE_STATUS_PROCESS svcStat = {};
DWORD bytesNeeded = 0;
HANDLE hSvcProc = NULL;
HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
THREADENTRY32 te32;
HMODULE modules[256] = {};
SIZE_T modulesSize = sizeof(modules);
DWORD sizeNeeded = 0;
SIZE_T mCountr = 0;
WCHAR rModule[128] = {};
HMODULE serviceModule = NULL;
MODULEINFO moduleInfo = {};
DWORD_PTR threadStartAddress = 0;