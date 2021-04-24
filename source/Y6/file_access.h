#pragma once

#include "sl.h"

class __declspec(novtable) isl_file_access
{
public:
	virtual ~isl_file_access() {};
	virtual bool open(const char* path, sl::handle_t handle) = 0;
	virtual bool create(const char* path, sl::handle_t handle) = 0;
	virtual bool remove(const char*) = 0;
	virtual bool close(sl::handle_t handle) = 0;
	virtual bool is_exist(const char *) = 0;
	virtual int64_t read(sl::handle_t handle, void* buffer, unsigned int size) = 0;
	virtual int64_t write(sl::handle_t handle, const void* buffer, unsigned int size) = 0;
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

class csl_file_access final : public isl_file_access
{
public:
	virtual bool open(const char* path, sl::handle_t handle) override;
	virtual bool create(const char* path, sl::handle_t handle) override;
	virtual bool remove(const char*) override;
	virtual bool close(sl::handle_t handle) override;
	virtual bool is_exist(const char* path) override;
	virtual int64_t read(sl::handle_t handle, void* buffer, unsigned int size) override;
	virtual int64_t write(sl::handle_t handle, const void* buffer, unsigned int size) override;
	virtual int64_t get_size(sl::handle_t handle) override;
	virtual int64_t seek(sl::handle_t handle, int64_t offset, sl::FILE_SEEK seekMode) override;
	virtual int64_t read_offset(sl::handle_t handle, void* buffer, unsigned int size, uint64_t offset) override;
	virtual uint64_t burst_read(sl::handle_t, void*, unsigned int, unsigned __int64) override;
	virtual uint64_t burst_read_flush(sl::handle_t) override;
	virtual uint64_t burst_read_wait(sl::handle_t) override;
	virtual void burst_status_write(unsigned int) override;
	virtual bool burst_status_is_complete(unsigned int) override;
	virtual void burst_status_wait(unsigned int) override;
};

// TODO: Alignment seems wrong
class alignas(32) csl_file_access_archive final : public isl_file_access
{
public:
	csl_file_access_archive();

	virtual bool open(const char* path, sl::handle_t handle) override;
	virtual bool create(const char* path, sl::handle_t handle) override;
	virtual bool remove(const char*) override;
	virtual bool close(sl::handle_t handle) override;
	virtual bool is_exist(const char* path) override;
	virtual int64_t read(sl::handle_t handle, void* buffer, unsigned int size) override;
	virtual int64_t write(sl::handle_t, const void*, unsigned int) override;
	virtual int64_t get_size(sl::handle_t handle) override;
	virtual int64_t seek(sl::handle_t handle, int64_t offset, sl::FILE_SEEK seekMode) override;
	virtual int64_t read_offset(sl::handle_t handle, void* buffer, unsigned int size, uint64_t offset) override;
	virtual uint64_t burst_read(sl::handle_t, void*, unsigned int, unsigned __int64) override;
	virtual uint64_t burst_read_flush(sl::handle_t) override;
	virtual uint64_t burst_read_wait(sl::handle_t) override;
	virtual void burst_status_write(unsigned int) override;
	virtual bool burst_status_is_complete(unsigned int) override;
	virtual void burst_status_wait(unsigned int) override;

	struct sector_cache_t
	{
		short m_prev = 0;
		short m_next = 0;
		sl::handle_t m_h_file;
		uint64_t m_offset = 0;
	};

private:
	void* mp_sector_cache_buffer = nullptr;
	sl::mutex_t m_mutex_sector_cache;
	sl::mutex_t m_mutex_decode_buffer;
	sector_cache_t m_sector_cache_tbl[16];
	t_pointer_list<sector_cache_t> m_sector_cache_list;
	void *mp_decode_buffer[2] = {};
};
static_assert(sizeof(csl_file_access_archive) == 416);

// This class is implemented in the DLL, we only need the interface (for now)
class __declspec(novtable) isl_archive
{
public:
	virtual ~isl_archive() {};
	virtual unsigned int release() = 0;
	virtual unsigned int get_subdirectory_num(unsigned int parent_directory) = 0;
	virtual unsigned int get_directory_id(unsigned int parent_directory, unsigned int entry_no) = 0;
	virtual unsigned int get_directory_name(unsigned int parent_directory, unsigned int entry_no, char* p_name, unsigned int max_name_length) = 0;
	virtual unsigned int get_file_num(unsigned int parent_directory) = 0;
	virtual unsigned int get_file_id(unsigned int parent_directory, unsigned int entry_no) = 0;
	virtual unsigned int get_file_name(unsigned int parent_directory, unsigned int entry_no, char* p_name, unsigned int max_name_length) = 0;
	virtual int find_file(const char* psz_file_name, unsigned int* p_file_index) = 0;
	virtual unsigned int extract_file(unsigned int file_id, void* p_buffer, unsigned int buffer_size) = 0;
	virtual unsigned int get_file_size(unsigned int file_index) = 0;
};

// We leave it as abstract as there's no need to instantiate this class (so far)
class csl_archive : public isl_archive
{
public:
	enum ASYNC_LOAD_STEP
	{
		ASYNC_LOAD_STEP_READY = 0x0,
		ASYNC_LOAD_STEP_OPEN = 0x1,
		ASYNC_LOAD_STEP_READ_HEADER = 0x2,
		ASYNC_LOAD_STEP_READ = 0x3,
	};

	virtual int initialize(void* p_image, sl::handle_t h_file) = 0;
	virtual uint64_t read_file(sl::handle_t h_file, unsigned int read_bytes) = 0;
	virtual void async_load_process_func(ASYNC_LOAD_STEP load_step) = 0;

	static csl_archive* create_instance(sl::handle_t handle);

	uint32_t add_ref() { return m_ref_count++; }

public:
	std::byte gap[16];
	uint32_t m_ref_count;
};
static_assert(offsetof(csl_archive, m_ref_count) == 24);