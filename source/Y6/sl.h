#pragma once

#include "common.h"

#include <cstdint>
#include <cstddef>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// sl definitions for Yakuza 6

namespace sl {

// These functions are implemented locally
void spinlock_lock(spinlock_t& spinlock);
void spinlock_unlock(spinlock_t& spinlock);


template<typename T>
struct t_locked_queue_node
{
  t_locked_queue_node<T> *volatile mp_next;
};

struct handle_internal_buffer_t
{
  t_locked_queue_node<handle_internal_buffer_t> m_node;
};


struct handle_internal_t
{
	union
	{
		uint64_t m_qhandle;
		union
		{
			uint64_t m_serial : 12;
			uint64_t m_ptr : 44;
			uint64_t m_type : 8;
		} data;
	} intern;
};


struct handle_t
{
	union
	{
		struct
		{
			uint32_t m_bank : 20;
			uint32_t m_serial : 12;
		} data;
		uint32_t m_handle;
	} h;
};

// Imported function
extern handle_t* (*handle_create_internal)(handle_t* obj, void* ptr, uint32_t type);
handle_t handle_create(void* ptr, uint32_t type);

template<typename T>
class alignas(16) t_locked_queue
{
public:
	void enqueue(handle_internal_buffer_t* p)
	{
		spinlock_lock(m_sync);

		m_size++;
		p->m_node.mp_next = nullptr;
		if (this->mp_head != nullptr)
			mp_tail->mp_next = &p->m_node;
		else
			this->mp_head = &p->m_node;
		this->mp_tail = &p->m_node;

		spinlock_unlock(m_sync);
	}

private:
	t_locked_queue_node<T> *mp_head;
	t_locked_queue_node<T> *mp_tail;
	spinlock_t m_sync;
	uint32_t m_size;
};
	
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
  std::byte gap[1400];
  t_locked_queue<handle_internal_buffer_t> handle_free_queue;
  std::byte gap2[5472];
  handle_t sync_file_handle_pool;
};

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

};