#include "dllmain.h"

#include <windows.h>
#include <filesystem>

BOOL APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE; 

	// Let's use a directory iterator to find and load .asi in the cwd
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
		if (entry.path().extension() == ".asi")
			LoadLibraryW(entry.path().wstring().c_str());

	return TRUE; 
}