#pragma once

#include "sl.h"

/* 12490 */
class __declspec(novtable) isl_file_access
{
public:
	virtual ~isl_file_access() {};
	virtual bool open(const char* path, sl::handle_t handle) = 0;
	virtual void create() = 0;
	virtual bool remove(const char *) = 0;
	virtual bool close(sl::handle_t) = 0;
	virtual bool is_exist(const char *) = 0;
	virtual unsigned __int64 read(sl::handle_t, void *, unsigned int) = 0;
	virtual unsigned __int64 write(sl::handle_t, const void *, unsigned int) = 0;
	virtual unsigned __int64 get_size(sl::handle_t) = 0;
	virtual unsigned __int64 seek(sl::handle_t, __int64, sl::FILE_SEEK) = 0;
	virtual unsigned __int64 read_offset(sl::handle_t, void *, unsigned int, unsigned __int64) = 0;
	virtual unsigned __int64 burst_read(sl::handle_t, void *, unsigned int, unsigned __int64) = 0;
	virtual unsigned __int64 burst_read_flush(sl::handle_t) = 0;
	virtual unsigned __int64 burst_read_wait(sl::handle_t) = 0;
	virtual void burst_status_write(unsigned int) = 0;
	virtual bool burst_status_is_complete(unsigned int) = 0;
	virtual void burst_status_wait(unsigned int) = 0;
};

isl_file_access* GetFileAccessImpl();