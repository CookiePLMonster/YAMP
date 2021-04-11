#pragma once

#include "sl.h"

class __declspec(novtable) isl_file_access
{
public:
	virtual ~isl_file_access() {};
	virtual bool open(const char* path, sl::handle_t handle) = 0;
	virtual void create() = 0;
	virtual bool remove(const char*) = 0;
	virtual bool close(sl::handle_t handle) = 0;
	virtual bool is_exist(const char *) = 0;
	virtual int64_t read(sl::handle_t handle, void* buffer, unsigned int size) = 0;
	virtual int64_t write(sl::handle_t, const void *, unsigned int) = 0;
	virtual int64_t get_size(sl::handle_t handle) = 0;
	virtual int64_t seek(sl::handle_t handle, int64_t offset, sl::FILE_SEEK seekMode) = 0;
	virtual int64_t read_offset(sl::handle_t handle, void* buffer, unsigned int size, uint64_t offset) = 0;

	// These don't seem to exist in Yakuza 6, but keep them for now
	virtual uint64_t burst_read(sl::handle_t, void *, unsigned int, unsigned __int64) = 0;
	virtual uint64_t burst_read_flush(sl::handle_t) = 0;
	virtual uint64_t burst_read_wait(sl::handle_t) = 0;
	virtual void burst_status_write(unsigned int) = 0;
	virtual bool burst_status_is_complete(unsigned int) = 0;
	virtual void burst_status_wait(unsigned int) = 0;
};

isl_file_access* GetFileAccessImpl();