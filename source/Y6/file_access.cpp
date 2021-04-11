#include "file_access.h"

class csl_file_access final : public isl_file_access
{
	virtual void create1() override
	{
	}
	virtual void create2() override
	{
	}
	virtual bool remove(const char*) override
	{
		return false;
	}
	virtual bool close(sl::handle_t) override
	{
		return false;
	}
	virtual bool is_exist(const char*) override
	{
		return true;
	}
	virtual unsigned __int64 read(sl::handle_t, void*, unsigned int) override
	{
		return 0;
	}
	virtual unsigned __int64 write(sl::handle_t, const void*, unsigned int) override
	{
		return 0;
	}
	virtual unsigned __int64 get_size(sl::handle_t) override
	{
		return 0;
	}
	virtual unsigned __int64 seek(sl::handle_t, __int64, sl::FILE_SEEK) override
	{
		return 0;
	}
	virtual unsigned __int64 read_offset(sl::handle_t, void*, unsigned int, unsigned __int64) override
	{
		return 0;
	}
	virtual unsigned __int64 burst_read(sl::handle_t, void*, unsigned int, unsigned __int64) override
	{
		return 0;
	}
	virtual unsigned __int64 burst_read_flush(sl::handle_t) override
	{
		return 0;
	}
	virtual unsigned __int64 burst_read_wait(sl::handle_t) override
	{
		return 0;
	}
	virtual void burst_status_write(unsigned int) override
	{
	}
	virtual bool burst_status_is_complete(unsigned int) override
	{
		return false;
	}
	virtual void burst_status_wait(unsigned int) override
	{
	}
} file_access;

isl_file_access* GetFileAccessImpl()
{
	return &file_access;
}