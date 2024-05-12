#include "dllmain.h"

#include <windows.h>
#include <filesystem>
#include <fstream>

bool isPortableExecutable(const std::filesystem::path& asiFile) {
	std::ifstream file(asiFile, std::ios::binary);
	if (!file.is_open()) return FALSE; // idk, better safe than sorry

	char header[2];
	file.read(header, 2);

	// * .asi files are just .dll files and DLL files which usually start with the first two bytes as "MZ"
	// https://en.wikipedia.org/wiki/Portable_Executable
	return header[0] == 'M' && header[1] == 'Z';
}

BOOL APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE; 

	// Let's use a directory iterator to find and load .asi in the cwd
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
		if (entry.path().extension() == ".asi" && isPortableExecutable(entry.path()))
			LoadLibraryW(entry.path().wstring().c_str());

	return TRUE; 
}