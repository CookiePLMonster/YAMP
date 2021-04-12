#include "async_request.h"

#include <cstdio>

uint32_t csl_file_async_request::sm_serial = 0;

csl_file_async_request::csl_file_async_request(isl_file_access** pp_interface, uint32_t max_req_item)
	: mpp_file_access(pp_interface)
{
	char threadName[128];
	sprintf_s(threadName, "file_async(%d)", sm_serial++);
	m_request = sl::semaphore_create(0);
	mp_req_item_buf = new req_item_t[max_req_item];
	for (uint32_t i = 0; i < max_req_item; i++)
	{
		m_free_queue.enqueue(&mp_req_item_buf[max_req_item - i - 1]);
	}
}
