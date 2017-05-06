#include <Windows.h>
#include <tchar.h>

#include <conio.h>

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

#if defined(UNICODE) || defined(_UNICODE)
#define tcin std::wcin
#define tcout std::wcout
#else
#define tcin std::cin
#define tcout std::cout
#endif

int main() {
    const SIZE_T size = 4096;
    const LPTSTR MappingName = _T("TTI_SHARED_MEMORY_EXAMPLE");
    HANDLE hMapping;
    LPTSTR pMemory;

    hMapping = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // hFile,
        NULL,                   // lpAttributes,
        PAGE_READWRITE,         // flProtect,
        size >> 32,             // dwMaximumSizeHigh,
        size & 0xFFFFFFFF,      // dwMaximumSizeLow,
        MappingName             // lpName
    );
    if(hMapping == NULL) {
        tcout << _T("Failed to create file mapping!") << endl
            << _T("Error: ") << GetLastError() << endl;
    }
    if(GetLastError() == ERROR_ALREADY_EXISTS) {
        tcout << _T("Opened already existing object.") << endl;
    }

    pMemory = (LPTSTR)MapViewOfFile(
        hMapping,               // hFileMappingObject,
        FILE_MAP_ALL_ACCESS,    // dwDesiredAccess,
        0,                      // dwFileOffsetHigh,
        0,                      // dwFileOffsetLow,
        0                       // dwNumberOfBytesToMap
    );

    tcout << setfill(_T('0'));
    tcout << _T("Mapped @ 0x")
        << setw(16) << (LPVOID)pMemory
        << endl;

    tcout << _T("r = read // w = write // q = quit") << endl;

    bool done = false;
    while(!done) {
        switch(getch()) {
        case 'r':
            tcout << _T("R> ")
                << pMemory << endl;
            break;
        case 'w':
            tcout << _T("W> ");
            tcin.getline(pMemory, size);
            break;
        case 'q':
            done = true;
            break;
        }
    }

    UnmapViewOfFile(pMemory);
    CloseHandle(hMapping);

    return 0;
}
