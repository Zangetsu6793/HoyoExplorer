#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>

#include <cstdint>
#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include <iostream>
#include <string>
#include <sstream>

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "UnityCore.h"
#include "Dumper.h"
#include "PatternScanner.h"
#include "Patterns.h"

#include <iostream>

#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <cstdio>

DWORD WINAPI MainThread(LPVOID lpReserved);
void DumpBytesAsInts(std::string Pattern);
inline void Test(); inline void Test2();