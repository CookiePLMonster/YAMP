#pragma once

#include <cstdint>
#include <cstddef>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "../pxd_types.h"
#include "../sl_internal.h"

class isl_file_access;

// sl definitions for Yakuza 6

namespace sl {

// Imported function
extern handle_t* (*handle_create_internal)(handle_t* obj, void* ptr, uint32_t type);
handle_t handle_create(void* ptr, uint32_t type);

struct alignas(16) file_handle_internal_t : public file_handle_t
{
  volatile unsigned int m_flags;
  unsigned int m_buffer_size;
  unsigned int m_archive_file_no;
  unsigned int m_error_code;
  handle_t m_async_event;
  handle_t m_h_basefile;
  unsigned __int64 m_basefile_offset;
  void *mp_buffer;
  void *mp_cache; // TODO: csl_filecache_name
  uint64_t m_read_offset;
  uint64_t m_real_file_size;
  void (*mp_callback_func)(handle_t, unsigned int, void *);
  void *mp_callback_param;
  volatile uint64_t m_callback_execute_thread;
  FILE_ASYNC_METHOD m_callback_method;
  unsigned int m_req_item_index;
  unsigned int m_last_async_status;
  rwspinlock_t m_locked;
  file_handle_internal_t *mp_link;
};
static_assert(sizeof(file_handle_internal_t) == 0x4D0);
	
struct export_context_t
{
	size_t size_of_struct;
	void *p_context;
};

struct context_t
{
  uint32_t tag_id;
  uint32_t version;
  uint32_t size_of_struct;
  export_context_t export_context;
  uint32_t processor_num;
  uint64_t main_thread_id;
  uint64_t processor_affinity_mask;
  void *p_temp_work;
  size_t temp_work_size;
  uint64_t count_frequency;
  double count_per_milli_second;
  double count_per_micro_second;
  double count_per_nano_second;
  double count_per_tick;
  union
  {
	handle_internal_buffer_t *p_handle_buffer;
	handle_internal_t *p_handle_tbl;
  } handles;
  uint32_t handle_max;
  uint32_t file_handle_max;
  uint32_t file_callback_thread_stack_size;
  std::byte gap3[12];
  isl_file_access* p_file_access;
  std::byte gap[1380];
  t_locked_queue<handle_internal_buffer_t> handle_free_queue;
  std::byte gap2[5408];
  spinlock_t sync_file_handle_pool;
  std::byte gap5[4];
  t_fixed_deque<file_handle_internal_t*> file_handle_pool;
  std::byte gap4[32];
  handle_t sync_archive_condvar;
};
// Validate important offsets
static_assert(offsetof(context_t, handles) == 0x70);
static_assert(offsetof(context_t, p_file_access) == 0x90);
static_assert(offsetof(context_t, handle_free_queue) == 0x600);
static_assert(offsetof(context_t, sync_file_handle_pool) == 0x1B40);
static_assert(offsetof(context_t, file_handle_pool) == 0x1B48);
static_assert(offsetof(context_t, sync_archive_condvar) == 0x1B80);

// TODO: Consider changing this to a pointer to real sm_context
extern context_t* sm_context;

// Custom types
struct file_handle_lock
{
	std::byte gap1[4];
	handle_t eventHandle1;
	handle_t eventHandle2;
	std::byte gap2[20];
	CRITICAL_SECTION critSec1;
	std::byte gap3[8];
	CRITICAL_SECTION critSec2;
	std::byte gap4[4];
};
static_assert(sizeof(file_handle_lock) == 0x80);

struct file_handle_event
{
	void* gap;
	HANDLE eventHandle;
};
static_assert(sizeof(file_handle_event) == 16);

template<typename T>
inline T* handle_instance(handle_t handle, uint32_t type)
{
	T* result = nullptr;
	if (handle.h.data.m_bank < sm_context->handle_max)
	{
		const handle_internal_t& internalHandle = sm_context->handles.p_handle_tbl[handle.h.data.m_bank];
		if (internalHandle.intern.data.m_serial == handle.h.data.m_serial && internalHandle.intern.data.m_type == type)
		{
			result = reinterpret_cast<T*>(internalHandle.intern.data.m_ptr << 4);
		}
	}

	return result;
}

inline file_handle_internal_t* file_handle_instance(handle_t handle)
{
	return handle_instance<file_handle_internal_t>(handle, 5);
}

};