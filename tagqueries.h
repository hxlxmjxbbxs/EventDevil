#include <iostream>
#include <Windows.h>
#include <winternl.h>


typedef enum _SC_SERVICE_TAG_QUERY_TYPE {
	ServiceNameFromTagInformation = 1,
	ServiceNameReferencingModuleInformation,
	ServiceNameTagMappingInformation,
} SC_SERVICE_TAG_QUERY_TYPE, * PSC_SERVICE_TAG_QUERY_TYPE;

typedef struct _SC_SERVICE_TAG_QUERY {
	ULONG   processId;
	ULONG   serviceTag;
	ULONG   reserved;
	PVOID   pBuffer;
} SC_SERVICE_TAG_QUERY, * PSC_SERVICE_TAG_QUERY;


using NtQueryInformationThreadx = NTSTATUS(NTAPI*)(
	IN HANDLE          ThreadHandle,
	IN THREADINFOCLASS ThreadInformationClass,
	OUT PVOID          ThreadInformation,
	IN ULONG           ThreadInformationLength,
	OUT PULONG         ReturnLength
	);


//https://docs.microsoft.com/en-us/windows/win32/secauthz/enabling-and-disabling-privileges-in-c--
#pragma once
