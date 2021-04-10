#pragma once

#include <cstdint>

// gs definitions for Yakuza 6
namespace cdevice {

struct shared_symbol_st
{
	void* p_value[8];
};

}

namespace gs {

struct export_context_t
{
	size_t size_of_struct;
	void* p_context;
	cdevice::shared_symbol_st sbgl_context;
};

struct context_t
{
  uint32_t tag_id; // 0x7367424C
  uint32_t version; // 0x40601
  uint32_t size_of_struct; // Should be 8128 when complete
  uint32_t sbgl_initialize_flags;
  export_context_t export_context;
};

}