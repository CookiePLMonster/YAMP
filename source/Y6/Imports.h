#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

	TRAP_ALLOC_INSTANCE_TBL
};

void* GetImportedFunction(HMODULE lib, Symbol symbol);

}