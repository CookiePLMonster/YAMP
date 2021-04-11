#include "file_access.h"

#include <string_view>

static std::wstring UTF8ToWchar(std::string_view text)
{
	std::wstring result;

	const int count = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
	if ( count != 0 )
	{
		result.resize(count);
		MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), count);
	}

	return result;
}

static std::string WcharToUTF8(std::wstring_view text)
{
	std::string result;

	const int count = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
	if ( count != 0 )
	{
		result.resize(count);
		WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), count, nullptr, nullptr);
	}

	return result;
}

class csl_file_access final : public isl_file_access
{
	virtual bool open(const char* path, sl::handle_t handle) override
	{
		return true;
	}
	virtual void create() override
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
	virtual bool is_exist(const char* path) override
	{
		return GetFileAttributesW(UTF8ToWchar(path).c_str()) != INVALID_FILE_ATTRIBUTES;
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