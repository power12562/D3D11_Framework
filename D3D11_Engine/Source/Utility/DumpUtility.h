#pragma once
#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <iostream>

/*�̴� ���� ���� �Լ�.*/
void CreateMiniDump(_EXCEPTION_POINTERS* pExceptionPointers, int MINIDUMP_TYPE);

/*���� �ݹ� �Լ�.*/
__callback LONG WINAPI CustomUnhandledExceptionFilter(_In_ _EXCEPTION_POINTERS* pExceptionPointers);

