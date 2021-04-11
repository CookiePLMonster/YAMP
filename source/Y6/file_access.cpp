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
		HANDLE file = CreateFileW(UTF8ToWchar(path).c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (file == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		sl::file_handle_internal_t* internalHandle = sl::file_handle_instance(handle);
		internalHandle->m_h_native = file;

		return true;
	}
	virtual void create() override
	{
		// Deliberately unimplemented for now
		assert(!"csl_file_access::create unimplemented!");
	}
	virtual bool remove(const char*) override
	{
		// Deliberately unimplemented for now
		assert(!"csl_file_access::remove unimplemented!");
		return false;
	}
	virtual bool close(sl::handle_t handle) override
	{
		sl::file_handle_internal_t* internalHandle = sl::file_handle_instance(handle);
		if (internalHandle != nullptr)
		{
			HANDLE& nativeHandle = reinterpret_cast<HANDLE&>(internalHandle->m_h_native);
			CloseHandle(nativeHandle);
			nativeHandle = INVALID_HANDLE_VALUE;
			return true;
		}
		return false;
	}
	virtual bool is_exist(const char* path) override
	{
		return GetFileAttributesW(UTF8ToWchar(path).c_str()) != INVALID_FILE_ATTRIBUTES;
	}
	virtual int64_t read(sl::handle_t handle, void* buffer, unsigned int size) override
	{
		sl::file_handle_internal_t* internalHandle = sl::file_handle_instance(handle);
		if (internalHandle != nullptr)
		{
			HANDLE nativeHandle = reinterpret_cast<HANDLE>(internalHandle->m_h_native);
			if (nativeHandle != INVALID_HANDLE_VALUE)
			{
				DWORD numBytesRead = 0;
				if (ReadFile(nativeHandle, buffer, size, &numBytesRead, nullptr) != FALSE)
				{
					internalHandle->m_file_pointer += numBytesRead;
					return numBytesRead;
				}
			}
		}
		return -1;
	}
	virtual int64_t write(sl::handle_t, const void*, unsigned int) override
	{
		// Deliberately unimplemented for now
		assert(!"csl_file_access::write unimplemented!");
		return 0;
	}
	virtual int64_t get_size(sl::handle_t handle) override
	{
		int64_t size = -1;
		const sl::file_handle_internal_t* internalHandle = sl::file_handle_instance(handle);
		if (internalHandle != nullptr)
		{
			HANDLE nativeHandle = reinterpret_cast<HANDLE>(internalHandle->m_h_native);
			if (nativeHandle != INVALID_HANDLE_VALUE)
			{
				LARGE_INTEGER s;
				if (GetFileSizeEx(nativeHandle, &s) != FALSE)
				{
					size = s.QuadPart;
				}
			}
		}
		return size;
	}
	virtual int64_t seek(sl::handle_t handle, int64_t offset, sl::FILE_SEEK seekMode) override
	{
		int64_t newPointer = -1;
		sl::file_handle_internal_t* internalHandle = sl::file_handle_instance(handle);
		if (internalHandle != nullptr)
		{
			HANDLE nativeHandle = reinterpret_cast<HANDLE>(internalHandle->m_h_native);
			if (nativeHandle != INVALID_HANDLE_VALUE)
			{
				LARGE_INTEGER newOffset;
				LARGE_INTEGER off;
				off.QuadPart = offset;
				if (SetFilePointerEx(nativeHandle, off, &newOffset, seekMode != sl::FILE_SEEK_SET ? FILE_CURRENT : FILE_BEGIN) != FALSE)
				{
					newPointer = newOffset.QuadPart;
					internalHandle->m_file_pointer = newOffset.QuadPart;
				}
			}
		}
		return newPointer;
	}
	virtual int64_t read_offset(sl::handle_t handle, void* buffer, unsigned int size, uint64_t offset) override
	{
		seek(handle, offset, sl::FILE_SEEK_SET);
		return read(handle, buffer, size);
	}
	virtual uint64_t burst_read(sl::handle_t, void*, unsigned int, unsigned __int64) override
	{
		return 0;
	}
	virtual uint64_t burst_read_flush(sl::handle_t) override
	{
		return 0;
	}
	virtual uint64_t burst_read_wait(sl::handle_t) override
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