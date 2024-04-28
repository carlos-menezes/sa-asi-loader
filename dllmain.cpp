#include "dllmain.h"

#include <windows.h>
#include <string>
#include <vector>
#include <filesystem>

HMODULE DLL_ENTRY_POINT;

static void GetASIFiles(const std::wstring& directory, std::vector<std::wstring>* out) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(directory.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                out->push_back(directory.substr(0, directory.find_last_of(L'\\') + 1) + findFileData.cFileName);
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

static void InitializeASILoader() {
    wchar_t modulePath[MAX_PATH];
    GetModuleFileNameW(nullptr, modulePath, MAX_PATH);

    // Get the directory containing the DLL
    std::wstring dllDirectory = modulePath;
    dllDirectory = dllDirectory.substr(0, dllDirectory.find_last_of(L"\\") + 1);

    // Find all .asi files in the directory
    std::vector<std::wstring> asiFiles;
    GetASIFiles(dllDirectory + L"*.asi", &asiFiles);

    // Load each .asi file
    for (const auto& asiFile : asiFiles) {
        LoadLibraryW(asiFile.c_str());
    }
}

static BOOL APIENTRY DllMain(HMODULE hModule, const DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DLL_ENTRY_POINT = hModule;
        InitializeASILoader();
    }
    return TRUE;
}
