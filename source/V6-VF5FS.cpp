#include "Y6-VF5FS.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "wil/resource.h"

#include "criware/CriStub.h"

#include "Y6/sl.h"
#include "Y6/gs.h"
#include "Imports.h"
#include "Y6/Patch.h"
#include "Y6/sys_util.h"
#include "Y6/cs_game.h"

#include "Y6/ImportSymbols.h"
#include "Imports.h"
#include "YAMPGeneral.h"

#include "Utils/MemoryMgr.h"

static const wchar_t* DLL_NAME = L"vf5fs-pxd-w64-Retail Steam.dll";

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

static void ImportFunctions(const Imports& symbols)
{
	auto Import = [&symbols](auto& var, auto symbol)
	{
		var = static_cast<std::decay_t<decltype(var)>>(symbols.GetSymbol(symbol));
	};

	Import(sl::sm_context, ImportSymbol::SL_CONTEXT_INSTANCE);
	Import(gs::sm_context, ImportSymbol::GS_CONTEXT_INSTANCE);
	Import(sl::file_create_internal, ImportSymbol::SL_FILE_CREATE);
	Import(sl::file_open_internal, ImportSymbol::SL_FILE_OPEN);
	Import(sl::file_read, ImportSymbol::SL_FILE_READ);
	Import(sl::file_close, ImportSymbol::SL_FILE_CLOSE);
	Import(sl::handle_create_internal, ImportSymbol::SL_HANDLE_CREATE);
	Import(sl::file_handle_destroy, ImportSymbol::SL_FILE_HANDLE_DESTROY);
	Import(sl::archive_lock_wlock, ImportSymbol::ARCHIVE_LOCK_WLOCK);
	Import(sl::archive_lock_wunlock, ImportSymbol::ARCHIVE_LOCK_WUNLOCK);
	Import(cgs_device_context::reset_state_all_internal, ImportSymbol::DEVICE_CONTEXT_RESET_STATE_ALL);
	Import(gs::vb_create, ImportSymbol::VB_CREATE);
	Import(gs::ib_create, ImportSymbol::IB_CREATE);
	Import(shift_next_mode, ImportSymbol::SHIFT_NEXT_MODE);
	Import(shift_next_mode_sub, ImportSymbol::SHIFT_NEXT_MODE_SUB);
}

static void PrefillVariables(const Imports& symbols, const RenderWindow& window)
{
	auto Import = [&symbols](auto& var, auto symbol)
	{
		var = static_cast<std::decay_t<decltype(var)>>(symbols.GetSymbol(symbol));
	};

	gs::context_t** ppContext;
	Import(ppContext, ImportSymbol::GS_CONTEXT_PTR);
	*ppContext = gs::sm_context;

	ID3D11Device** ppDevice;
	Import(ppDevice, ImportSymbol::D3DDEVICE);
	*ppDevice = window.GetD3D11Device();
}

static bool ResolveSymbolsAndInstallPatches(void* dll, const RenderWindow& window) try
{
	const Imports symbolMap = BuildSymbolMap(dll);

	const ScopedUnprotect::Section text(static_cast<HMODULE>(dll), ".text");
	const ScopedUnprotect::Section rdata(static_cast<HMODULE>(dll), ".rdata");

	// Patch up structures and do post-DllMain work here
	// Saves having to reimplement all the complex constructors and data types
	ImportFunctions(symbolMap);
	PrefillVariables(symbolMap, window); // Pre-fill those variables gs/sl initialization relies on

	if (!gGeneral.GetSettings()->m_dontApplyPatches)
	{
		// Install hooks re-adding logging
		ReinstateLogging(dll, symbolMap);
		// Install additional "assertions"
		InjectTraps(symbolMap);

		// Restore saving
		Patch_SysUtil(dll, symbolMap);

		// Misc patches - bugfixes, un-folding no-ops etc
		Patch_Misc(dll, symbolMap);
		Patch_CsGame(dll, symbolMap);
	}

	PatchSl(sl::sm_context);
	PatchGs(gs::sm_context, window);

	return true;
}
catch (...)
{
	// TODO: Show this in native UI
	const std::wstring str(L"Failed to resolve imports and/or patch " + std::wstring(DLL_NAME) + L".dll!\n\nIt's either not a valid Virtua Fighter 5: Final Showdown DLL file from Yakuza 6, "
		"or the game has been updated and YAMP is not forward compatible with that new version.");
	MessageBoxW(nullptr, str.c_str(), L"Yakuza Arcade Machines Player", MB_ICONERROR | MB_OK);

	return false;
}

// TODO: Move elsewhere
static std::filesystem::path gamePath;

static wil::unique_hmodule gameDll;
HMODULE Y6::VF5FS::LoadDLL()
{
	// TODO: Clean up
	{
		DWORD dwSize = GetCurrentDirectoryW(0, nullptr);
		auto buf = std::make_unique<wchar_t[]>(dwSize);
		GetCurrentDirectoryW(dwSize, buf.get());
		gamePath.assign(buf.get());
	}

	gameDll.reset(LoadLibraryW((gamePath / DLL_NAME).c_str()));
	if (gameDll == nullptr)
	{
		// Try loading from a subdirectory
		gamePath.append(L"vf5fs");
		gameDll.reset(LoadLibraryW((gamePath / DLL_NAME).c_str()));
	}

	if (!gameDll)
	{
		const std::wstring str(L"Could not load " + std::wstring(DLL_NAME) + L".dll!\n\nMake sure that YAMP.exe is located in your Yakuza 6: The Song of Life directory or its \"vf5fs\" subdirectory, next to the DLL file.");
		MessageBoxW(nullptr, str.c_str(), L"Yakuza Arcade Machines Player", MB_ICONERROR | MB_OK);
	}
	else
	{
		gGeneral.SetDLLName(WcharToUTF8(DLL_NAME));

		// Get the checksum
		PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(gameDll.get());
		PIMAGE_NT_HEADERS ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<char*>(dosHeader) + dosHeader->e_lfanew);
		gGeneral.SetDLLTimestamp(ntHeader->FileHeader.TimeDateStamp);
	}

	return gameDll.get();
}

void Y6::VF5FS::PreInitialize()
{
	gGeneral.SetDataPath(u8"Sega", u8"Virtua Fighter 5 Final Showdown");
	gGeneral.LoadSettings();
}

static void CheckForExecutable()
{
	// TODO: Make more graceful instead of killing the app
	const std::wstring executablePath = (gamePath.parent_path() / L"Yakuza6.exe").native();

	// We'll consider the file valid if it has MZ and PE magic values
	wil::unique_hfile file(CreateFileW(executablePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
	FAIL_FAST_IMMEDIATE_IF(!file);

	wil::unique_handle fileMapping(CreateFileMapping(file.get(), nullptr, PAGE_READONLY, 0, 0, nullptr));
	FAIL_FAST_IMMEDIATE_IF_NULL(fileMapping);

	wil::unique_mapview_ptr<IMAGE_DOS_HEADER> dosHeaderView(static_cast<PIMAGE_DOS_HEADER>(MapViewOfFile(fileMapping.get(), FILE_MAP_READ, 0, 0, 0)));
	FAIL_FAST_IMMEDIATE_IF_NULL(dosHeaderView);

	FAIL_FAST_IMMEDIATE_IF(dosHeaderView->e_magic != 'ZM');

	PIMAGE_NT_HEADERS64 peHeaderView = reinterpret_cast<PIMAGE_NT_HEADERS64>(reinterpret_cast<char*>(dosHeaderView.get()) + dosHeaderView->e_lfanew);

	FAIL_FAST_IMMEDIATE_IF(peHeaderView->Signature != 'EP');
}

void Y6::VF5FS::Run(RenderWindow& window)
{
	const auto module_start = reinterpret_cast<module_func_t>(GetProcAddress(gameDll.get(), "module_start"));
	THROW_LAST_ERROR_IF_NULL(module_start);
	const auto module_stop = reinterpret_cast<module_func_t>(GetProcAddress(gameDll.get(), "module_stop"));
	THROW_LAST_ERROR_IF_NULL(module_stop);
	module_func_t module_main;

	CheckForExecutable();

	if (!ResolveSymbolsAndInstallPatches(gameDll.get(), window))
	{
		// Init failed
		return;
	}

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
		const char* root_path;
		module_func_t* module_main;
		vf5fs_game_config_t config{};
	} params;
	static_assert(sizeof(params) == 64);

	const std::string utf8Path = gamePath.u8string();

	params.sl_module = &sl_module;
	params.gs_module = &gs_module;
	params.ct_module = &ct_module;
	params.cri_ptr = &criware_stub;
	params.module_main = &module_main;
	params.root_path = utf8Path.c_str();

	const auto* settings = gGeneral.GetSettings();

	params.config.is_dural_unlocked = false;
	params.config.is_triangle_start = false;
	params.config.game_mode = settings->m_arcadeMode ? 1 : 0;
	params.config.lang = settings->m_language;
	params.config.diff = 1;
	params.config.energy = 200;
	params.config.round = 2;
	params.config.time = 60;

	// Kick off the game
	if (module_start(sizeof(params), &params) == 0)
	{
		while (!window.IsShuttingDown())
		{
			if (!GameLoop(module_main, window)) break;
		}

		// TODO: module_stop
	}
}

bool Y6::VF5FS::GameLoop(module_func_t func, RenderWindow& window)
{
	vf5fs_execute_info_t execute_info{};
	execute_info.size_of_struct = sizeof(execute_info);
	execute_info.p_device_context = gs::sm_context->p_device_context;

	// TODO: Set up mappings better
	// They're ignored now, in-game mappings are used instead
	/*{
		size_t i = 0;
		auto& mappings1P = execute_info.assign[0];
		auto& mappings2P = execute_info.assign[1];
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_p; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_k; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_g; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_p; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_pg; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_pkg; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_pk; i++;
		mappings1P[i] = mappings2P[i] = vf5fs_execute_info_t::assign_kg; i++;
	}*/

	// TODO: Beautify
	execute_info.pad[0].set_state(0);
	execute_info.pad[1].set_state(1);
	execute_info.status |= 2;

	window.NewImGuiFrame();

	if (func(sizeof(execute_info), &execute_info) != 0) return false;

	cgs_tex* display_tex = gs::sm_context->handle_tex.get(execute_info.output_texid);
	if (display_tex == nullptr) return false;
	if (display_tex->m_type != 2) return false;

	// TODO: Beautify this
	auto& swapChain = gs::sm_context->sbgl_device.m_swap_chain;
	window.BlitGameFrame(display_tex->mp_sbgl_resource->m_pD3DShaderResourceView);

	window.RenderImGui();

	HRESULT hr = swapChain.m_pDXGISwapChain->Present(1, 0);
	if (FAILED(hr)) return false;

	gs::sm_context->frame_counter++;
	return true;
}
