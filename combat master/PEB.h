#pragma once
#include <windows.h>
typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;
typedef struct _PEB_LDR_DATA64
{
	DWORD Length;
	DWORD Initialized;
	DWORD64 SsHandle;
	LIST_ENTRY64 InLoadOrderModuleList;
	LIST_ENTRY64 InMemoryOrderModuleList;
	LIST_ENTRY64 InInitializationOrderModuleList;
	DWORD64 EntryInProgress;
	DWORD64 ShutdownInProgress;
	DWORD64 ShutdownThreadId;
} PEB_LDR_DATA64, * PPEB_LDR_DATA64;
typedef struct _PEB64
{
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID64 Reserved3[2];
	PPEB_LDR_DATA64 Ldr;
	PVOID64 ProcessParameters;
	BYTE Reserved4[104];
	PVOID64 Reserved5[52];
	PVOID64 PostProcessInitRoutine;
	BYTE Reserved6[128];
	PVOID64 Reserved7[1];
	ULONG SessionId;
} PEB64, * PPEB64;
typedef struct _LDR_DATA_TABLE_ENTRY64
{
	LIST_ENTRY64 InLoadOrderLinks;
	LIST_ENTRY64 InMemoryOrderLinks;
	LIST_ENTRY64 InInitializationOrderLinks;
	DWORD64 DllBase;
	DWORD64 EntryPoint;
	DWORD64 SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY64, * PLDR_DATA_TABLE_ENTRY64;