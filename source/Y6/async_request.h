#pragma once

#include "sl.h"
#include "file_access.h"

class csl_file_async_request
{
public:
	csl_file_async_request(isl_file_access** pp_interface, uint32_t max_req_item);

public: // TODO: Make private once I figure out how to deal with offsetof below
	static constexpr size_t NUM_REQUEST_QUEUES = 6;

	struct req_item_t
	{
	  t_locked_queue_node<csl_file_async_request::req_item_t> m_node;
	  sl::handle_t m_h_file;
	  spinlock_t m_locked;
	  volatile unsigned int m_status = 0;
	  volatile unsigned int m_abort = 0;
	};

	sl::handle_t m_request;
	sl::handle_t m_h_thread;
	isl_file_access **mpp_file_access = nullptr;
	csl_file_async_request::req_item_t *mp_req_item_buf = nullptr;
	volatile int m_is_finish_req = 0;
	sl::handle_t m_h_busy_file;
	sl::mutex_t m_mutex_request;
	t_locked_queue<csl_file_async_request::req_item_t> m_free_queue;
	t_locked_queue<csl_file_async_request::req_item_t> m_busy_queue[NUM_REQUEST_QUEUES];
	t_locked_queue<csl_file_async_request::req_item_t> m_request_queue[NUM_REQUEST_QUEUES];

private:
	uint32_t thread_routine();

	static uint32_t sm_serial;

	static uint32_t stub_thread_routine(uint64_t arg);
};
static_assert(sizeof(csl_file_async_request) == 496);
static_assert(offsetof(csl_file_async_request, m_free_queue) == 0x50);