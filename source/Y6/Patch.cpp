#include "Patch.h"

#include "file_access.h"
#include "../pxd_types.h"

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
	sl::file_handle_event* event1 = new sl::file_handle_event {};
	sl::file_handle_event* event2 = new sl::file_handle_event {};
	event1->eventHandle = CreateEventW(nullptr, TRUE, TRUE, nullptr);
	event2->eventHandle = CreateEventW(nullptr, TRUE, TRUE, nullptr);

	sl::file_handle_lock* lock = new sl::file_handle_lock {};
	lock->eventHandle1 = sl::handle_create(event1, 3);
	lock->eventHandle2 = sl::handle_create(event2, 3);
	InitializeCriticalSectionAndSpinCount(&lock->critSec1, 4096);
	InitializeCriticalSectionAndSpinCount(&lock->critSec2, 4096);

	context->sync_archive_condvar = sl::handle_create(lock, 4);

	// Set up file access
	context->p_file_access = GetFileAccessImpl();

	// Set up file_handle_pool
	static constexpr uint32_t NUM_FILE_HANDLES = 250;
	context->file_handle_pool._afterConstruct(NUM_FILE_HANDLES);

	// TODO: Validate this
	sl::file_handle_internal_t* handles = new sl::file_handle_internal_t[NUM_FILE_HANDLES] {};

	for (auto& handle : wil::make_range(handles, NUM_FILE_HANDLES))
	{
		// TODO: Especially this
		const auto handlePtr = &handle;
		context->file_handle_pool.push_back(&handlePtr);
	}
}

void PatchGs(gs::context_t* context, const RenderWindow& window)
{
	// Fill the export context
	auto& export_context = context->export_context;
	export_context.size_of_struct = sizeof(export_context);
	export_context.sbgl_context.p_value[0] = window.GetD3D11Device();
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
