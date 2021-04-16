#include "Imports.h"

// TODO: Mutate this class depending on the DLL version! Right now it hardcodes day 1 symbols
namespace Imports {

static constexpr uintptr_t BASE_ADDRESS = 0x180000000; // Base address of the Y6 DLL

void* GetImportedFunction(HMODULE lib, Symbol symbol)
{
	const uintptr_t libAddr = reinterpret_cast<uintptr_t>(lib);
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
	default: // Unreachable
		symbolAddr = 0;
		break;
	}
	return reinterpret_cast<void*>(libAddr + (symbolAddr - BASE_ADDRESS));
}

}