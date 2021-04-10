#pragma once

#include <cstdint>

// sl definitions for Yakuza 6

namespace sl {

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
  void** p_handle_buffer; // Fake type
  uint32_t handle_max;
  uint32_t file_handle_max;
  uint32_t file_callback_thread_stack_size;
};


};