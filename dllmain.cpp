#include "dllmain.h"

#include <windows.h>
#include <filesystem>
#include <fstream>
#include <iostream> // We use std::endl to ensure the log is written even if a crash occurs
#include <chrono>
#include <iomanip>
#include <string>

void log(const std::string& message) { std::ofstream("asiloader.log", std::ios::app) << message << std::endl; }

BOOL APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE;

	// We open in truncate mode in order to clear the log file
	std::ofstream logFile("asiloader.log", std::ios::trunc);

	// Log the timestamp
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	logFile << std::put_time(std::localtime(&time), "%Y-%m-%d %X: ") << "ASI Loader started" << std::endl;
	logFile.close(); 

	// Let's use a directory iterator to find and load .asi in the cwd
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (entry.is_directory() || entry.path().extension() != ".asi") continue; // Ignore directories and non-asi files

		const std::string filename = entry.path().filename().string();

		try {
			// * .asi files are just .dll files and DLL files usually start with the first two bytes as "MZ"
			// So we try opening the file and make sure the first two bytes are what we need, before loading it
			// https://en.wikipedia.org/wiki/Portable_Executable

			std::ifstream file(entry.path(), std::ios::binary);

			char header[2];

			if (!file.is_open()) {
				log("Error: Unable to open ASI: " + filename);
			} else if (!file.read(header, 2) || header[0] != 'M' || header[1] != 'Z') {
				log("Error: Invalid ASI file format: " + filename);
			} else {
				log("Loading " + filename);
				log(LoadLibraryW(entry.path().wstring().c_str()) ? "Loaded " + filename : "Error: Failed to load ASI: " + filename + " (Error code: " + std::to_string(GetLastError()) + ")");
			}
		} catch (const std::exception& e) { log("Error: Exception while processing ASI: " + filename + " - " + e.what()); }
	}

	log("Finished loading.");

	return TRUE; 
}