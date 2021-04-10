#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// "Imported" variables and functions from the DLL
namespace Imports {

enum class Symbol
{
	SL_CONTEXT_INSTANCE,
	GS_CONTEXT_INSTANCE,
};

void* GetImportedFunction(HMODULE lib, Symbol symbol);

}