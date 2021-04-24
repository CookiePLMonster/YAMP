#include "sys_util.h"

#include "sl.h"

#include <ShlObj.h>
#include <filesystem>
#include "../wil/resource.h"

namespace fs = std::filesystem;

static const char* SAVE_FILE_NAME = u8"system.dat";

struct sys_util_save_header
{
	uint32_t version; // 100
	std::byte unk[8];
	uint32_t size;
};
static_assert(sizeof(sys_util_save_header) == 16);

static fs::path s_path_to_systemdata;
void sys_util_init_storage_directory()
{
	// TODO: Allow for portable mode
	wil::unique_cotaskmem_string str;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, str.addressof())))
	{
		s_path_to_systemdata = str.get();
		s_path_to_systemdata.append(u8"Sega");
		s_path_to_systemdata.append(u8"Virtua Fighter 5 Final Showdown");
	}
}


bool sys_util_check_enable_storage(int port)
{
	// TODO: Implement properly
	return true;
}

void sys_util_start_load_systemdata_task(int port, void* buf, unsigned int buf_size, bool autoload, bool create, void(*cbfunc)(bool success, bool creator))
{
	bool cbSuccess = false;
	auto cbOnExit = wil::scope_exit([&] {
		if (cbfunc != nullptr)
			cbfunc(cbSuccess, create);
	});

	// TODO: Do we need to use the port parameter?
	// TODO: Should we call the callback if 'create' is set to off and the file doesn't exist?
	const auto utf8PathToFile = (s_path_to_systemdata / SAVE_FILE_NAME).u8string();
	sl::handle_t file = create ? sl::file_create(utf8PathToFile.c_str()) : sl::file_open(utf8PathToFile.c_str());
	if (file == sl::INVALID_HANDLE) return;

	sys_util_save_header header;
	if (sl::file_read(file, &header, sizeof(header)) == sizeof(header))
	{
		// TODO: What about the unknown fields? Should the size check be == and not <=?
		if (header.version == 100 && header.size <= buf_size)
		{
			cbSuccess = sl::file_read(file, buf, header.size) == header.size;
		}
	}
	sl::file_close(file);
}

void sys_util_start_save_systemdata_task(int port, const void* buf, unsigned int buf_size, bool autosave, void(*cbfunc)(bool success, bool unknown))
{
	bool cbSuccess = false;
	auto cbOnExit = wil::scope_exit([&] {
		if (cbfunc != nullptr)
			cbfunc(cbSuccess, false);
	});

	// Create the entire directory structure if needed
	std::error_code ec;
	if (!fs::create_directories(s_path_to_systemdata, ec)) return;

	const auto utf8PathToFile = (s_path_to_systemdata / SAVE_FILE_NAME).u8string();
	sl::handle_t file = sl::file_create(utf8PathToFile.c_str());
	if (file == sl::INVALID_HANDLE) return;

	sys_util_save_header header {};
	header.version = 100;
	header.size = buf_size;
	if (sl::file_write(file, &header, sizeof(header)) == sizeof(header))
	{
		cbSuccess = sl::file_write(file, buf, buf_size) == buf_size;
	}
	sl::file_close(file);
}
