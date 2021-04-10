#include "Patch.h"

#include "../wil/common.h"

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

	// Create sync_file_handle_pool
	sl::file_handle_event* event1 = new sl::file_handle_event {};
	sl::file_handle_event* event2 = new sl::file_handle_event {};
	event1->eventHandle = CreateEventW(nullptr, TRUE, TRUE, nullptr);
	event2->eventHandle = CreateEventW(nullptr, TRUE, TRUE, nullptr);

	sl::file_handle_lock* lock = new sl::file_handle_lock {};
	lock->eventHandle1 = sl::handle_create(event1, 3);
	lock->eventHandle2 = sl::handle_create(event2, 3);
	InitializeCriticalSectionAndSpinCount(&lock->critSec1, 4096);
	InitializeCriticalSectionAndSpinCount(&lock->critSec2, 4096);

	context->sync_file_handle_pool = sl::handle_create(lock, 4);
}

void PatchGs(gs::context_t* context, const RenderWindow& window)
{
	// Fill the export context
	auto& export_context = context->export_context;
	export_context.size_of_struct = sizeof(export_context);
	export_context.sbgl_context.p_value[0] = window.GetD3D11Device();
}