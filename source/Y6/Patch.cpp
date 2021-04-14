#include "Patch.h"

#include "file_access.h"
#include "../pxd_types.h"
#include "async_request.h"

#include "../wil/common.h"
#include "../Utils/MemoryMgr.h"
#include "../Utils/Trampoline.h"

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
	sbgl::cgs_device_context* device_context = new sbgl::cgs_device_context{};

	context->sbgl_device.initialize(window);
	
	device_context->initialize(reinterpret_cast<sbgl::ccontext*>(context->sbgl_device.m_pD3DDeviceContext));
	context->p_device_context = device_context;

	// Fill the export context
	auto& export_context = context->export_context;
	export_context.size_of_struct = sizeof(export_context);
	export_context.sbgl_context.p_value[0] = window.GetD3D11Device();
	export_context.sbgl_context.p_value[1] = static_cast<sbgl::cdevice_native*>(&context->sbgl_device);
	export_context.sbgl_context.p_value[2] = &context->sbgl_device.m_swap_chain;
}

static void prj_trap(const char* format, ...)
{
	char buf[512];
	va_list vl;
	va_start(vl, format);
	vsprintf_s(buf, format, vl);
	va_end(vl);

	OutputDebugStringA(buf);
}

void ReinstateLogging(void* logFunc)
{
	Trampoline* t = Trampoline::MakeTrampoline(logFunc);
	Memory::VP::InjectHook(logFunc, t->Jump(&prj_trap), PATCH_JUMP);
}
