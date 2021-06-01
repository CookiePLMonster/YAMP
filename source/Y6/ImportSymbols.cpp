#include "ImportSymbols.h"
#include "../Imports.h"

#include "../Utils/Patterns.h"
#include "../Utils/MemoryMgr.h"

using namespace hook::txn;

template<typename T = void>
static auto get_module_pattern(void* module, std::string_view pattern_string, ptrdiff_t offset = 0)
{
	return pattern(module, std::move(pattern_string)).get_first<T>(offset);
}

static void* immediate(void* addr)
{
	void* val;
	Memory::ReadOffsetValue(addr, val);
	return val;
}

static void* immediate8(void* addr)
{
	intptr_t srcAddr = (intptr_t)addr;
	intptr_t dstAddr = srcAddr + 1 + *(int8_t*)srcAddr;
	return reinterpret_cast<void*>(dstAddr);
}

Imports BuildSymbolMap(void* dll)
{
	using S = ImportSymbol;

	Imports symbols{
		// Functions/globals
		{ S::SL_CONTEXT_INSTANCE, immediate(get_module_pattern(dll, "48 89 5C 24 ? 48 8D 3D", 5 + 3)) },
		{ S::GS_CONTEXT_INSTANCE, immediate(get_module_pattern(dll, "48 8D 2D ? ? ? ? 48 89 68 08", 3)) },
		{ S::GS_CONTEXT_PTR, immediate(get_module_pattern(dll, "48 8B 05 ? ? ? ? 8B F1 BA", 3)) },
		{ S::D3DDEVICE, immediate(get_module_pattern(dll, "48 89 05 ? ? ? ? 48 8B 41 28", 3)) },
		{ S::SL_FILE_CREATE, get_module_pattern(dll, "48 8B 05 ? ? ? ? 48 8B F9", -0x13) },
		{ S::SL_FILE_OPEN, get_module_pattern(dll, "48 8B 05 ? ? ? ? 48 8B D9 45 33 F6", -0x12) },
		{ S::SL_FILE_READ, get_module_pattern(dll, "4C 8B 0D ? ? ? ? 8B C1", -0x6) },
		{ S::SL_FILE_CLOSE, immediate(get_module_pattern(dll, "E8 ? ? ? ? 48 C7 44 3B", 1)) },
		{ S::SL_HANDLE_CREATE, get_module_pattern(dll, "48 8B 3D ? ? ? ? 48 8B F1 45 33 FF", -0x18) },
		{ S::SL_FILE_HANDLE_DESTROY, get_module_pattern(dll, "48 89 5C 24 ? 48 81 C1", -0x11) },
		{ S::PRJ_TRAP, immediate(get_module_pattern(dll, "E8 ? ? ? ? 8B 43 0C 4C 3B E0", 1)) },
		{ S::ARCHIVE_LOCK_WLOCK, immediate(get_module_pattern(dll, "E8 ? ? ? ? 8B 7B 18", 1)) },
		{ S::ARCHIVE_LOCK_WUNLOCK, immediate(get_module_pattern(dll, "E8 ? ? ? ? 90 83 7B 24 00", 1)) },
		{ S::DEVICE_CONTEXT_RESET_STATE_ALL, get_module_pattern(dll, "48 83 EC 30 48 83 09 01", -0x6) },
		{ S::VB_CREATE, get_module_pattern(dll, "48 83 EC 40 48 8B 05 ? ? ? ? 48 8B F1", -0x14) },
		{ S::IB_CREATE, get_module_pattern(dll, "48 8B 05 ? ? ? ? 44 8B F1", -0x18) },
		{ S::SHIFT_NEXT_MODE, immediate(get_module_pattern(dll, "E8 ? ? ? ? 33 C9 83 7B 58 01", 1)) },
		{ S::SHIFT_NEXT_MODE_SUB, immediate(get_module_pattern(dll, "E8 ? ? ? ? 41 B7 01", 1)) },

		// Places to patch
		{ S::TRAP_ALLOC_INSTANCE_TBL, immediate8(get_module_pattern(dll, "73 ? 4C 8B 41 08", 1)) },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "E8 ? ? ? ? 84 C0 74 0D 33 D2") },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "8B C8 E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 8D 8B", 2) },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "E8 ? ? ? ? 48 8B CB 84 C0 75 26") },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "E8 ? ? ? ? 84 C0 74 09 40 84 ED") },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "8B CB E8 ? ? ? ? 84 C0 74 6C", 2) },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "8B CB E8 ? ? ? ? 84 C0 74 56", 2) },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "B9 FF FF FF FF E8 ? ? ? ? 84 C0 75 0D", 5) },
		{ S::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH, get_module_pattern(dll, "4C 89 7C 24 ? E8 ? ? ? ? 84 C0", 5) },
		{ S::SYS_UTIL_START_LOAD_SYSTEMDATA_TASK_PATCH, get_module_pattern(dll, "40 88 7C 24 ? E8 ? ? ? ? 48 8B 5C 24", 5) },
		{ S::SYS_UTIL_START_SAVE_SYSTEMDATA_TASK_PATCH, get_module_pattern(dll, "41 B8 FC 4F 00 00 8B CB E8", 6 + 2) },
		{ S::SYS_UTIL_IS_ENTER_CIRCLE_PATCH, get_module_pattern(dll, "E8 ? ? ? ? 84 C0 4C 8D 15") },
		{ S::SYS_UTIL_IS_ENTER_CIRCLE_PATCH, get_module_pattern(dll, "E8 ? ? ? ? 84 C0 75 2A 83 FB 30") },
		{ S::ASSIGN_HELPER_ENABLE_SHARED_FROM_THIS_PATCH, get_module_pattern(dll, "48 8B D6 48 8D 4C 24 ? E8 ? ? ? ? 90", 3 + 5) },
		{ S::VF5_APP_CTOR_PATCH, get_module_pattern(dll, "E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 03 EB 03") },
		{ S::PRESS_START_POS_Y_PATCH, immediate(get_module_pattern(dll, "75 3F C5 FA 10 0D", 2 + 4)) },
	};

	// Other special cases
	auto cs_switch_mapping_override = make_module_pattern(dll, "66 42 89 84 33").count(8);
	cs_switch_mapping_override.for_each_result([&symbols](hook::pattern_match match) {
		symbols.Add(S::CS_SWITCH_MAPPING_OVERRIDE_PATCH, match.get<void>());
		});

	auto sys_util_is_enter_circle = make_module_pattern(dll, "57 48 83 EC 20 E8 ? ? ? ? 84 C0").count(3); // + 2 from a different pattern
	sys_util_is_enter_circle.for_each_result([&symbols](hook::pattern_match match) {
		symbols.Add(S::SYS_UTIL_IS_ENTER_CIRCLE_PATCH, match.get<void>(1 + 4));
		});

	auto task_pause_ctrl_countdown = make_module_pattern(dll, "C5 FA 10 71 ? E8 ? ? ? ? C5 CA 5C D0").count(2);
	task_pause_ctrl_countdown.for_each_result([&symbols](hook::pattern_match match) {
		symbols.Add(S::TASK_PAUSE_CTRL_COUNTDOWN_PATCH, match.get<void>(5));
		});

	auto press_start_pos_x_ptr = make_module_pattern(dll, "C5 FA 10 05 ? ? ? ? 41 B8 09 00 00 00 E8 ? ? ? ? B9 19 00 00 00").count(3);
	press_start_pos_x_ptr.for_each_result([&symbols](hook::pattern_match match) {
		symbols.Add(S::PRESS_START_POS_X_PTR_PATCH, match.get<void>(4));
		});

	struct ModeSubTable
	{
	  uint32_t name;
	  bool pause_enable;
	  bool (__fastcall *func[3])();
	};
	auto mode_sub_table = static_cast<ModeSubTable*>(immediate(get_module_pattern(dll, "4C 8D 05 ? ? ? ? 49 8B C0 40 32 FF", 3)));
	symbols.Add(S::DEST_CS_AUTOLOAD_PATCH, &mode_sub_table[5].func[2]);

	return symbols;
}
