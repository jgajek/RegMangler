// RegMangler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <winternl.h>

#include <iostream>
#include <string>

#pragma comment(lib, "ntdll.lib")

using namespace std;

#define VID_VMWARE 0x15AD;
#define VID_ORACLE 0x80EE;

#define REG_KEY_PCI_ENUM L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\PCI"

typedef enum _KEY_INFORMATION_CLASS {
	KeyBasicInformation,
	KeyNodeInformation,
	KeyFullInformation,
	KeyNameInformation,
	KeyCachedInformation,
	KeyFlagsInformation,
	MaxKeyInfoClass
} KEY_INFORMATION_CLASS;

NTSTATUS NTAPI NtOpenKey(
	_Out_	PHANDLE KeyHandle,
	_In_	ACCESS_MASK DesiredAccess,
	_In_	POBJECT_ATTRIBUTES ObjectAttributes
);

NTSTATUS NTAPI NtQueryKey(
	_In_		HANDLE KeyHandle,
	_In_		KEY_INFORMATION_CLASS KeyInformationClass,
	_Out_opt_	PVOID KeyInformation,
	_In_		ULONG Length,
	_Out_		PULONG ResultLength
);

void PrintLastError(LPWSTR szApiCall)
{
	LPWSTR pMsgBuf = NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsgBuf, 0, NULL);
	wcout << wstring(szApiCall) << L": " << wstring(pMsgBuf) << endl;
	LocalFree(pMsgBuf);
}

void RegEnumPCIDevices(void)
{
	HANDLE hKey = NULL;
	NTSTATUS status;
	UNICODE_STRING keyname;
	OBJECT_ATTRIBUTES objattr;

	RtlInitUnicodeString(&keyname, REG_KEY_PCI_ENUM);
	InitializeObjectAttributes(&objattr, &keyname, OBJ_CASE_INSENSITIVE, NULL, NULL);

	status = NtOpenKey(&hKey, KEY_ENUMERATE_SUB_KEYS, &objattr);
}

int wmain(int argc, WCHAR* argv[])
{
	RegEnumPCIDevices();
	return 0;
}
