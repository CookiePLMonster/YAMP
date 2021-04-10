#include "Y6-VF5FS.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "wil/resource.h"

static const wchar_t* DLL_NAME = L"vf5fs-pxd-w64-Retail Steam_noaslr"; // Temporary, remove _noaslr later

// Contexts
// TODO: Move elsewhere, as they will get very, very long
// But not in V6-VF5FS.h as they are private!
struct sl_context_t
{
	uint32_t tag_id = 0x6C73424C;
	uint32_t version = 0x40601;
	uint32_t size_of_struct = sizeof(decltype(*this)); // Should be 61440 when complete

	// NOTE: This structure won't be used until size_of_struct is correct
};

struct gs_context_t
{
	uint32_t tag_id = 0x7367424C;
	uint32_t version = 0x40601;
	uint32_t size_of_struct = sizeof(decltype(*this)); // Should be 8128 when complete

	// NOTE: This structure won't be used until size_of_struct is correct
};

// Seems unused, so don't bother getting it right
struct ct_context_t
{
	uint32_t tag_id; // Unknown
	uint32_t version; // Unknown
	uint32_t size_of_struct = sizeof(decltype(*this)); // Should be 48 when complete
};

// TODO: Later these can be put by value, for now put them on the heap to make full use of page heap
// and catch any out-of-bounds access
sl_context_t* g_sl_context = new sl_context_t;
gs_context_t* g_gs_context = new gs_context_t;
ct_context_t* g_ct_context = new ct_context_t;

// Configs
struct vf5fs_game_config_t
{
	// TODO: Fill with defaults
	uint16_t energy;
	int8_t round;
	int8_t time;
	int8_t diff;
	int8_t game_mode;
	int8_t lang;
	bool is_triangle_start : 4;
	bool is_dural_unlocked : 4;
};
static_assert(sizeof(vf5fs_game_config_t) == 8);

using module_func_t = int(*)(size_t args, const void* argp);

void Y6::VF5FS::Run()
{
	wil::unique_hmodule gameDll(LoadLibraryW(DLL_NAME));
	THROW_LAST_ERROR_IF_NULL(gameDll);

	const auto module_start = reinterpret_cast<module_func_t>(GetProcAddress(gameDll.get(), "module_start"));
	const auto module_stop = reinterpret_cast<module_func_t>(GetProcAddress(gameDll.get(), "module_stop"));
	module_func_t module_main;

	const struct sl_module_t
	{
		size_t size = sizeof(decltype(*this));
		sl_context_t* context = g_sl_context;
	} sl_module;
	
	const struct gs_module_t
	{
		size_t size = sizeof(decltype(*this)); // Should be 80 when complete, but other fields are unknown so far
		gs_context_t* context = g_gs_context;
		uint8_t pad[64];
	} gs_module;

	const struct ct_module_t
	{
		size_t size = sizeof(decltype(*this));
		ct_context_t* context = g_ct_context;
	} ct_module;

	struct module_params_t
	{
		size_t size = sizeof(decltype(*this));
		const sl_module_t* sl_module;
		const gs_module_t* gs_module;
		const ct_module_t* ct_module;
		void* cri_ptr = nullptr;
		const char* root_path = "";
		module_func_t* module_main;
		vf5fs_game_config_t config;
	} params;
	static_assert(sizeof(params) == 64);

	params.sl_module = &sl_module;
	params.gs_module = &gs_module;
	params.ct_module = &ct_module;
	params.module_main = &module_main;

	// Kick off the game
	module_start(sizeof(params), &params);
}