#include "DumpUtility.h"
#include <filesystem>

void CreateMiniDump(_EXCEPTION_POINTERS* pExceptionPointers, int MINIDUMP_TYPE)
{
    constexpr wchar_t dumpPath[] = L"dumps/";
    namespace fs = std::filesystem;
    // ���� �ð� ��������
    SYSTEMTIME localTime;   
    GetLocalTime(&localTime);

    // ���� ���� �̸� ��������
    TCHAR moduleName[MAX_PATH];
    GetModuleFileName(nullptr, moduleName, MAX_PATH);
    TCHAR* baseName = _tcsrchr(moduleName, _T('\\'));
    baseName = (baseName != nullptr) ? (baseName + 1) : moduleName; // ���ϸ� ����
    TCHAR* extension = _tcsrchr(baseName, _T('.'));
    if (extension) {
        *extension = _T('\0'); // Ȯ���� ����
    }
 
    // ���丮 ���� Ȯ�� ��, ������ ����
    if (!fs::exists(dumpPath)) 
    {
         fs::create_directories(dumpPath);  // ���丮 ����
    }

    //���� ���� �̸� ���� (�� �̸� + ��¥ + �ð�)
    TCHAR dumpFileName[MAX_PATH];
    _stprintf_s(dumpFileName, _T("%s%s_%04d-%02d-%02d_%02d-%02d-%02d.dmp"),
        dumpPath,
        baseName,
        localTime.wYear, localTime.wMonth, localTime.wDay,
        localTime.wHour, localTime.wMinute, localTime.wSecond);

    // ���� ���� ����
    HANDLE hFile = CreateFile(dumpFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) 
    {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ExceptionPointers = pExceptionPointers;
        dumpInfo.ClientPointers = FALSE;

        BOOL success = MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hFile,
            (_MINIDUMP_TYPE)MINIDUMP_TYPE,
            &dumpInfo,
            nullptr,
            nullptr
        );

        if (!success) 
        {
            //���� ���� ���н�

        }
        CloseHandle(hFile);
    }
    else 
    {
        //���� ���� ���н�

    }
}

__callback LONG WINAPI CustomUnhandledExceptionFilter(_In_ _EXCEPTION_POINTERS* pExceptionPointers)
{
#ifdef _DEBUG
    CreateMiniDump(pExceptionPointers, MiniDumpWithFullMemory | MiniDumpWithThreadInfo);
#else //������
    CreateMiniDump(pExceptionPointers, MiniDumpWithDataSegs | MiniDumpWithThreadInfo);
#endif
    return EXCEPTION_EXECUTE_HANDLER;
}