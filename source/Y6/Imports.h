#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>

// "Imported" variables and functions from the DLL
namespace Imports {

enum class Symbol
{
	SL_CONTEXT_INSTANCE,
	GS_CONTEXT_INSTANCE,
	GS_CONTEXT_PTR,
	D3DDEVICE,

	SL_HANDLE_CREATE,
	SL_FILE_HANDLE_DESTROY,
	PRJ_TRAP,

	ARCHIVE_LOCK_WLOCK,
	ARCHIVE_LOCK_WUNLOCK,

	DEVICE_CONTEXT_RESET_STATE_ALL,
	VB_CREATE,
	IB_CREATE,

	TRAP_ALLOC_INSTANCE_TBL,

	// Lists
	SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH,

};

void* GetImportedFunction(void* lib, Symbol symbol);
std::vector<void*> GetImportedFunctionsList(void* lib, Symbol symbol);

}