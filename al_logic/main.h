#pragma once
#include <iostream>

#define DLL

#ifdef DLL
#include <Windows.h>

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpReserved);

extern "C" __declspec(dllexport) int Calculate(const char* expr, const char** variables, int* variables_values, int variables_count);


#endif

#include "lexer.h"