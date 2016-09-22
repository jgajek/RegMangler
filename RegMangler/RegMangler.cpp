// RegMangler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"
#include "KtmW32.h"

#include <iostream>
#include <string>

#pragma comment(lib, "KtmW32.lib")

using namespace std;

void PrintLastError(LPWSTR szApiCall)
{
	LPWSTR pMsgBuf = NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsgBuf, 0, NULL);
	wcout << wstring(szApiCall) << L": " << wstring(pMsgBuf) << endl;
	LocalFree(pMsgBuf);
}

int wmain(int argc, WCHAR* argv[])
{
	HANDLE hTransaction;
	HKEY hKey, hKey2;
	DWORD dwDisposition;

	hTransaction = CreateTransaction(NULL, NULL, 0, 0, 0, 0, NULL);
	PrintLastError(L"CreateTransaction");

	RegCreateKeyTransacted(HKEY_CURRENT_USER, L"Software\\RegMangler", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition, hTransaction, NULL);
	PrintLastError(L"RegCreateKeyTransacted");

	for (DWORD n = 0; n < 100; n++) {
		RegSetValueEx(hKey, to_wstring(n).c_str(), 0, REG_DWORD, (PBYTE)&n, sizeof(DWORD));
		PrintLastError(L"RegSetValueEx");
	}

	CommitTransaction(hTransaction);
	PrintLastError(L"CommitTransaction");

	CloseHandle(hTransaction);
	CloseHandle(hKey);

	hTransaction = CreateTransaction(NULL, NULL, 0, 0, 0, 0, NULL);
	PrintLastError(L"CreateTransaction");

	RegOpenKeyTransacted(HKEY_CURRENT_USER, L"Software\\RegMangler", 0, KEY_ALL_ACCESS, &hKey, hTransaction, NULL);
	PrintLastError(L"RegOpenKeyTransacted");

	RegCreateKeyTransacted(HKEY_CURRENT_USER, L"Software\\RegManglerCopy", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey2, &dwDisposition, hTransaction, NULL);
	PrintLastError(L"RegCreateKeyTransacted");

	RegCopyTree(hKey, NULL, hKey2);
	PrintLastError(L"RegCopyTree");

	CommitTransaction(hTransaction);
	PrintLastError(L"CommitTransaction");

	CloseHandle(hTransaction);
	CloseHandle(hKey);
	CloseHandle(hKey2);

	hTransaction = CreateTransaction(NULL, NULL, 0, 0, 0, 0, NULL);
	PrintLastError(L"CreateTransaction");

	RegDeleteKeyTransacted(HKEY_CURRENT_USER, L"Software\\RegMangler", 0, 0, hTransaction, NULL);
	PrintLastError(L"RegDeleteKeyTransacted");

	RegDeleteKeyTransacted(HKEY_CURRENT_USER, L"Software\\RegManglerCopy", 0, 0, hTransaction, NULL);
	PrintLastError(L"RegDeleteKeyTransacted");

	CommitTransaction(hTransaction);
	PrintLastError(L"CommitTransaction");

	CloseHandle(hTransaction);
	CloseHandle(hKey);
	CloseHandle(hKey2);
	return 0;
}
