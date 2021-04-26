#include "Y6-VF5FS.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "wil/resource.h"

#include "criware/CriStub.h"

#include "Y6/sl.h"
#include "Y6/gs.h"
#include "Y6/Imports.h"
#include "Y6/Patch.h"
#include "Y6/sys_util.h"

static const wchar_t* DLL_NAME = L"vf5fs-pxd-w64-Retail Steam_noaslr"; // Temporary, remove _noaslr later

// Contexts
// TODO: Move elsewhere, as they will get very, very long
// But not in V6-VF5FS.h as they are private!

// Seems unused, so don't bother getting it right
struct ct_context_t
{
	uint32_t tag_id; // Unknown
	uint32_t version; // Unknown
	uint32_t size_of_struct = sizeof(decltype(*this)); // Should be 48 when complete
};

// TODO: Later these can be put by value, for now put them on the heap to make full use of page heap
// and catch any out-of-bounds access
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

struct alignas(16) vf5fs_execute_info_t
{
	enum assign_t
	{
		assign_invalid = 0x0,
		assign_none = 0x1,
		assign_p = 0x2,
		assign_k = 0x3,
		assign_g = 0x4,
		assign_pg = 0x5,
		assign_pkg = 0x6,
		assign_pk = 0x7,
		assign_kg = 0x8,
	};


	size_t size_of_struct;
	cgs_device_context* p_device_context;
	int status;
	int result;
	unsigned int output_texid;
	std::byte gap[4];
	uint8_t assign[2][8];
	csl_pad pad[2];
	std::byte gap2[16];
};
static_assert(sizeof(vf5fs_execute_info_t) == 800);
static_assert(offsetof(vf5fs_execute_info_t, assign) == 0x20);
static_assert(offsetof(vf5fs_execute_info_t, pad) == 0x30);

static void ImportFunctions(HMODULE dll)
{
	using namespace Imports;
	auto Import = [dll](auto& var, auto symbol)
	{
		var = static_cast<std::decay_t<decltype(var)>>(GetImportedFunction(dll, symbol));
	};

	Import(sl::sm_context, Symbol::SL_CONTEXT_INSTANCE);
	Import(gs::sm_context, Symbol::GS_CONTEXT_INSTANCE);
	Import(sl::file_create_internal, Symbol::SL_FILE_CREATE);
	Import(sl::file_open_internal, Symbol::SL_FILE_OPEN);
	Import(sl::file_read, Symbol::SL_FILE_READ);
	Import(sl::file_close, Symbol::SL_FILE_CLOSE);
	Import(sl::handle_create_internal, Symbol::SL_HANDLE_CREATE);
	Import(sl::file_handle_destroy, Symbol::SL_FILE_HANDLE_DESTROY);
	Import(sl::archive_lock_wlock, Symbol::ARCHIVE_LOCK_WLOCK);
	Import(sl::archive_lock_wunlock, Symbol::ARCHIVE_LOCK_WUNLOCK);
	Import(cgs_device_context::reset_state_all_internal, Symbol::DEVICE_CONTEXT_RESET_STATE_ALL);
	Import(gs::vb_create, Symbol::VB_CREATE);
	Import(gs::ib_create, Symbol::IB_CREATE);
	Import(screen_size, Symbol::SCREEN_SIZE_ARR);
}

static void PrefillVariables(HMODULE dll, const RenderWindow& window)
{
	using namespace Imports;
	auto Import = [dll](auto& var, auto symbol)
	{
		var = static_cast<std::decay_t<decltype(var)>>(GetImportedFunction(dll, symbol));
	};

	gs::context_t** ppContext;
	Import(ppContext, Symbol::GS_CONTEXT_PTR);
	*ppContext = gs::sm_context;

	ID3D11Device** ppDevice;
	Import(ppDevice, Symbol::D3DDEVICE);
	*ppDevice = window.GetD3D11Device();
}

static void InjectTraps(HMODULE dll)
{
#ifdef _DEBUG
	std::forward_list<void*> traps;
	using namespace Imports;
	auto Import = [dll](auto& var, auto symbol)
	{
		var = static_cast<std::decay_t<decltype(var)>>(GetImportedFunction(dll, symbol));
	};

	void* ptr;
	Import(ptr, Symbol::TRAP_ALLOC_INSTANCE_TBL); traps.push_front(ptr);

	InjectTraps(traps);
#endif
}

static wil::unique_hmodule gameDll;
HMODULE Y6::VF5FS::LoadDLL()
{
	// TODO: Clean up
	gameDll.reset(LoadLibraryW(DLL_NAME));
	THROW_LAST_ERROR_IF_NULL(gameDll);
	return gameDll.get();
}

void Y6::VF5FS::Run(const RenderWindow& window)
{
	const auto module_start = reinterpret_cast<module_func_t>(GetProcAddress(gameDll.get(), "module_start"));
	THROW_LAST_ERROR_IF_NULL(module_start);
	const auto module_stop = reinterpret_cast<module_func_t>(GetProcAddress(gameDll.get(), "module_stop"));
	THROW_LAST_ERROR_IF_NULL(module_stop);
	module_func_t module_main;

	// Patch up structures and do post-DllMain work here
	// Saves having to reimplement all the complex constructors and data types
	ImportFunctions(gameDll.get());
	PrefillVariables(gameDll.get(), window); // Pre-fill those variables gs/sl initialization relies on

	// Install hooks re-adding logging
	ReinstateLogging(Imports::GetImportedFunction(gameDll.get(), Imports::Symbol::PRJ_TRAP));
	// Install additional "assertions"
	InjectTraps(gameDll.get());

	// Restore saving
	Patch_SysUtil(gameDll.get());

	// Misc patches - bugfixes, un-folding no-ops etc
	Patch_Misc(gameDll.get());

	// High res rendering support
	Patch_RenderRes(gameDll.get(), window);

	PatchSl(sl::sm_context);
	PatchGs(gs::sm_context, window);

	sys_util_init_storage_directory();
	sys_util_get_render_window(window);

	// Initialize Criware stub and module stubs
	CriStub criware_stub;

	struct sl_module_t
	{
		size_t size = sizeof(decltype(*this));
		sl::context_t* context;
	} sl_module;
	sl_module.context = sl::sm_context;
	
	struct gs_module_t
	{
		size_t size = sizeof(decltype(*this)); // Should be 80 when complete, but other fields are unknown so far
		gs::context_t* context;
		uint8_t pad[64];
	} gs_module;
	gs_module.context = gs::sm_context;

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
		const icri* cri_ptr;
		const char* root_path = ".";
		module_func_t* module_main;
		vf5fs_game_config_t config {};
	} params;
	static_assert(sizeof(params) == 64);

	params.sl_module = &sl_module;
	params.gs_module = &gs_module;
	params.ct_module = &ct_module;
	params.cri_ptr = &criware_stub;
	params.module_main = &module_main;

	params.config.is_dural_unlocked = false;
	params.config.is_triangle_start = false;
	params.config.game_mode = 0;
	params.config.lang = 1;
	params.config.diff = 1;
	params.config.energy = 200;
	params.config.round = 2;
	params.config.time = 60;

	// Kick off the game
	if (module_start(sizeof(params), &params) == 0)
	{
		while (!window.IsShuttingDown())
		{
			if (!GameLoop(module_main)) break;
		}

		// TODO: module_stop
	}
}

bool Y6::VF5FS::GameLoop(module_func_t func)
{
	vf5fs_execute_info_t execute_info {};
	execute_info.size_of_struct = sizeof(execute_info);
	execute_info.p_device_context = gs::sm_context->p_device_context;

	// TODO: This is needed, figure out what fields exactly cause it
	execute_info.pad[0] = {};

	// TODO: Set up mappings better
	{
		size_t i = 0;
		auto& mappings = execute_info.assign[0];
		mappings[i++] = vf5fs_execute_info_t::assign_p;
		mappings[i++] = vf5fs_execute_info_t::assign_k;
		mappings[i++] = vf5fs_execute_info_t::assign_g;
		mappings[i++] = vf5fs_execute_info_t::assign_p;
		mappings[i++] = vf5fs_execute_info_t::assign_pg;
		mappings[i++] = vf5fs_execute_info_t::assign_pkg;
		mappings[i++] = vf5fs_execute_info_t::assign_pk;
		mappings[i++] = vf5fs_execute_info_t::assign_kg;
	}

	// TODO: Beautify
	execute_info.pad[0].set_state();
	execute_info.status |= 2;

	if (func(sizeof(execute_info), &execute_info) != 0) return false;

	cgs_tex* display_tex = gs::sm_context->handle_tex.get(execute_info.output_texid);
	if (display_tex == nullptr) return false;
	if (display_tex->m_type != 2) return false;
	cgs_rt* display_rt = display_tex->mp_rt;

	// TODO: Beautify this
	auto& swapChain = gs::sm_context->sbgl_device.m_swap_chain;
	IDXGISwapChain* nativeSwapChain = swapChain.m_pDXGISwapChain;
	ID3D11DeviceContext* context = gs::sm_context->p_device_context->mp_sbgl_context;

	// TODO: This will need a proper Draw, for now CopyResource should work
	wil::com_ptr<ID3D11Resource> destination;
	HRESULT hr = nativeSwapChain->GetBuffer(0, IID_PPV_ARGS(destination.addressof()));
	if (FAILED(hr)) return false;

	ID3D11Resource* source = display_rt->mp_sbgl_resource->m_pD3DResource;
	context->CopyResource(destination.get(), source);

	hr = nativeSwapChain->Present(1, 0);
	if (FAILED(hr)) return false;

	gs::sm_context->frame_counter++;
	return true;
}
