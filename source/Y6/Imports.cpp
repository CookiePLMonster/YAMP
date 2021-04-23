#include "Imports.h"
#include <cassert>

// TODO: Mutate this class depending on the DLL version! Right now it hardcodes day 1 symbols
namespace Imports {

static constexpr uintptr_t BASE_ADDRESS = 0x180000000; // Base address of the Y6 DLL

void* GetImportedFunction(void* lib, Symbol symbol)
{
	uintptr_t symbolAddr;
	switch (symbol)
	{
	case Symbol::SL_CONTEXT_INSTANCE:
		symbolAddr = 0x180AD50C0;
		break;
	case Symbol::GS_CONTEXT_INSTANCE:
		symbolAddr = 0x180AD3040;
		break;
	case Symbol::GS_CONTEXT_PTR:
		symbolAddr = 0x180ACF888;
		break;
	case Symbol::D3DDEVICE:
		symbolAddr = 0x180AD2DD8;
		break;
	case Symbol::SL_HANDLE_CREATE:
		symbolAddr = 0x1800214B0;
		break;
	case Symbol::SL_FILE_HANDLE_DESTROY:
		symbolAddr = 0x180021800;
		break;
	case Symbol::PRJ_TRAP:
		symbolAddr = 0x1801D2ED0;
		break;
	case Symbol::ARCHIVE_LOCK_WLOCK:
		symbolAddr = 0x1800237E0;
		break;
	case Symbol::ARCHIVE_LOCK_WUNLOCK:
		symbolAddr = 0x180023960;
		break;
	case Symbol::DEVICE_CONTEXT_RESET_STATE_ALL:
		symbolAddr = 0x180008A90;
		break;
	case Symbol::VB_CREATE:
		symbolAddr = 0x18000D6D0;
		break;
	case Symbol::IB_CREATE:
		symbolAddr = 0x18000EDE0;
		break;

	// Debug only
	case Symbol::TRAP_ALLOC_INSTANCE_TBL:
		symbolAddr = 0x180010833;
		break;
	default:
		assert(!"Unreachable!");
		break;
	}
	const uintptr_t libAddr = reinterpret_cast<uintptr_t>(lib);
	return reinterpret_cast<void*>(libAddr + (symbolAddr - BASE_ADDRESS));
}

std::vector<void*> GetImportedFunctionsList(void* lib, Symbol symbol)
{
	std::vector<void*> result;
	switch (symbol)
	{
	case Symbol::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH:
		result = { (void*)0x1800F0345, (void*)0x1801AF21A, (void*)0x1801AF2CF, (void*)0x1801AF61C, (void*)0x1801B6BE6, (void*)0x1801B6E8C, (void*)0x1801E639E, (void*)0x18020B32E };
		break;
	default:
		assert(!"Unreachable!");
		break;
	}

	const uintptr_t libAddr = reinterpret_cast<uintptr_t>(lib);
	for (auto& symbolAddr : result)
	{
		symbolAddr = reinterpret_cast<void*>(libAddr + (reinterpret_cast<uintptr_t>(symbolAddr) - BASE_ADDRESS));
	}

	return result;
}

}