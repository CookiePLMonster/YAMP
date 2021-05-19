#include "Patch.h"

#include "file_access.h"
#include "../pxd_types.h"
#include "async_request.h"

#include "../wil/common.h"
#include "../Utils/MemoryMgr.h"
#include "../Utils/Trampoline.h"

#include "Imports.h"
#include "sys_util.h"

void PatchSl(sl::context_t* context)
{
	// Populate handle_free_queue
	static constexpr size_t NUM_HANDLES = 1000;

	context->handle_max = NUM_HANDLES;
	context->handles.p_handle_buffer = new sl::handle_internal_buffer_t[NUM_HANDLES];

	for (auto& handle : wil::make_range(context->handles.p_handle_buffer, NUM_HANDLES))
	{
		context->handle_free_queue.enqueue(&handle);
	}

	// Create sync_archive_condvar

	sl::archive_lock* lock = new sl::archive_lock;
	lock->_afterConstruct();

	context->sync_archive_condvar = sl::handle_create(lock, 4);

	// Set up file access
	context->p_file_access = new csl_file_access;
	context->p_archive_access = new csl_file_access_archive;

	// Set up file_handle_pool
	static constexpr uint32_t NUM_FILE_HANDLES = 250;
	context->file_handle_pool.reserve(NUM_FILE_HANDLES);

	// TODO: Validate this
	sl::file_handle_internal_t* handles = new sl::file_handle_internal_t[NUM_FILE_HANDLES] {};

	for (auto& handle : wil::make_range(handles, NUM_FILE_HANDLES))
	{
		handle._afterConstruct();
		// TODO: Especially this
		const auto handlePtr = &handle;
		context->file_handle_pool.push_back(&handlePtr);
	}

	// Set up async file requests
	static constexpr uint32_t NUM_REQUESTS = NUM_FILE_HANDLES + 64;

	context->p_file_async_request = new csl_file_async_request(&context->p_file_access, NUM_REQUESTS);
	context->p_archive_async_request = new csl_file_async_request(reinterpret_cast<isl_file_access**>(&context->p_archive_access), NUM_REQUESTS);
}

void PatchGs(gs::context_t* context, const RenderWindow& window)
{
	// Initialize cgs_device_context
	cgs_device_context* device_context = new cgs_device_context{};

	context->sbgl_device.initialize(window);

	static constexpr size_t NUM_CONTEXTES = 16; // TODO: Uneducated guess, real value comes from the init struct
	for (size_t i = 0; i < NUM_CONTEXTES; i++)
	{
		cgs_cb_pool* cbPool = new cgs_cb_pool;
		context->stack_cb_pool.push(cbPool);

		// TODO: Figure out proper sizes ASAP, now hardcoded for both which is terrible and probably wrong
		constexpr unsigned int UP_VB_SIZE = 4096, UP_IB_SIZE = 4096;
		cgs_up_pool* upPool = new cgs_up_pool;
		upPool->initialize(UP_VB_SIZE, UP_IB_SIZE, true);
		context->stack_up_pool.push(upPool);

		cgs_shader_uniform* uniform = new cgs_shader_uniform;
		uniform->initialize();

		context->stack_shader_uniform.push(uniform);
	}
	
	device_context->initialize(reinterpret_cast<sbgl::ccontext*>(context->sbgl_device.m_pD3DDeviceContext));
	context->p_device_context = device_context;

	constexpr unsigned int FX_MAX = 256;
	constexpr unsigned int VS_MAX = 512;
	constexpr unsigned int PS_MAX = 512;
	constexpr unsigned int GS_MAX = 256;
	constexpr unsigned int DS_MAX = 256;
	constexpr unsigned int HS_MAX = 256;
	constexpr unsigned int GTS_MAX = 256;
	constexpr unsigned int TEX_MAX = 1024;
	context->handle_tex.initialize(nullptr, TEX_MAX);
	context->handle_vs.initialize(nullptr, VS_MAX);
	context->handle_ps.initialize(nullptr, PS_MAX);
	context->handle_gs.initialize(nullptr, GS_MAX);
	context->handle_ds.initialize(nullptr, DS_MAX);
	context->handle_hs.initialize(nullptr, HS_MAX);
	context->handle_gts.initialize(nullptr, GTS_MAX);
	context->handle_fx.initialize(nullptr, FX_MAX);

	// Fill the export context
	auto& export_context = context->export_context;
	export_context.size_of_struct = sizeof(export_context);
	export_context.sbgl_context.p_value[0] = window.GetD3D11Device();
	export_context.sbgl_context.p_value[1] = static_cast<sbgl::cdevice_native*>(&context->sbgl_device);
	export_context.sbgl_context.p_value[2] = &context->sbgl_device.m_swap_chain;

	gs::primitive_initialize();
}

static void prj_trap(const char* format, ...)
{
	char buf[512];
	va_list vl;
	va_start(vl, format);
	vsprintf_s(buf, format, vl);
	va_end(vl);

	OutputDebugStringA(buf);
#ifdef _DEBUG
	__debugbreak();
#endif
}

void ReinstateLogging(void* dll)
{
	Trampoline* t = Trampoline::MakeTrampoline(dll);
	{
		void* func = Imports::GetImportedFunction(dll, Imports::Symbol::PRJ_TRAP);
		Memory::VP::InjectHook(func, t->Jump(&prj_trap), PATCH_JUMP);
	}
}

void InjectTraps(const std::forward_list<void*>& addresses)
{
	for (void* ptr : addresses)
	{
		Memory::VP::Patch<uint8_t>(ptr, 0xCC);
	}
}

void Patch_SysUtil(void* dll)
{
	// TODO: Unprotect before patching and lose the ::VP namespace
	Trampoline* hop = Trampoline::MakeTrampoline(dll);
	{
		void* sys_util_enable_storage = hop->Jump(&sys_util_check_enable_storage);
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::SYS_UTIL_CHECK_ENABLE_STORAGE_PATCH))
		{
			Memory::VP::InjectHook(addr, sys_util_enable_storage);
		}
	}

	{
		void* sys_util_load_systemdata_task = hop->Jump(&sys_util_start_load_systemdata_task);
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::SYS_UTIL_START_LOAD_SYSTEMDATA_TASK_PATCH))
		{
			Memory::VP::InjectHook(addr, sys_util_load_systemdata_task);
		}
	}

	{
		void* sys_util_save_systemdata_task = hop->Jump(&sys_util_start_save_systemdata_task);
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::SYS_UTIL_START_SAVE_SYSTEMDATA_TASK_PATCH))
		{
			Memory::VP::InjectHook(addr, sys_util_save_systemdata_task);
		}
	}

	{
		void* sys_util_circle_enter = hop->Jump(&sys_util_is_enter_circle);
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::SYS_UTIL_IS_ENTER_CIRCLE_PATCH))
		{
			Memory::VP::InjectHook(addr, sys_util_circle_enter);
		}
	}
}

static void assign_helper_enable_shared_from_this(...)
{
}

static float get_frame_speed_pause_stub()
{
	return 1.0f;
}

static void* (*orgVF5AppCtor)(void* obj, int argc, char** argv);
static void* VF5AppCtor_arguments(void* obj, int /*argc*/, char** /*argv*/)
{
	return orgVF5AppCtor(obj, __argc, __argv);
}

void Patch_Misc(void* dll)
{
	// TODO: Unprotect before patching and lose the ::VP namespace
	Trampoline* hop = Trampoline::MakeTrampoline(dll);
	// prj::shared_ptr_internal::assign_helper_enable_shared_from_this folds with prj_trap, causing a false-positive log and eventually crashing
	{
		void* assign_helper = hop->Jump(&assign_helper_enable_shared_from_this);
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::ASSIGN_HELPER_ENABLE_SHARED_FROM_THIS_PATCH))
		{
			Memory::VP::InjectHook(addr, assign_helper);
		}
	}

	// Fix pause countdown not counting down
	// In Y6 this code uses frame time, in YLAD it just uses count-- - a possible failed attempt at making the code support high framerates?
	{
		void* get_frame_speed_stub = hop->Jump(&get_frame_speed_pause_stub);
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::TASK_PAUSE_CTRL_COUNTDOWN_PATCH))
		{
			Memory::VP::InjectHook(addr, get_frame_speed_stub);
		}
	}

	// Pass commandline arguments from the launcher
	{
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::VF5_APP_CTOR_PATCH))
		{
			Memory::VP::ReadCall(addr, orgVF5AppCtor);
			Memory::VP::InjectHook(addr, hop->Jump(VF5AppCtor_arguments));;
		}	
	}

	// Reinstate "Press START button"
	{
		for (void* addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::PRESS_START_POS_Y_PATCH))
		{
			Memory::VP::Patch<float>(addr, 320.0f);
		}

		float& posX = hop->Reference<float>();
		posX = 250.0f;

		for (auto addr : Imports::GetImportedFunctionsList(dll, Imports::Symbol::PRESS_START_POS_X_PTR_PATCH))
		{
			Memory::VP::WriteOffsetValue(addr, &posX);
		}
	}
}