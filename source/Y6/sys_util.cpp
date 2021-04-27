#include "sys_util.h"

#include "sl.h"
#include "../RenderWindow.h"
#include "../Utils/MemoryMgr.h"

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

static const RenderWindow* s_render_window_ptr = nullptr;
void sys_util_get_render_window(const class RenderWindow& window)
{
	s_render_window_ptr = &window;
}


void get_screen_conv_pos(vecmath::Vector2<float>* dst_pos, SCREEN_MODE dst_mode, const vecmath::Vector2<float>* src_pos, SCREEN_MODE src_mode)
{
	if (dst_pos == nullptr || src_pos == nullptr) return;

	if (dst_mode == SCREEN_MODE::IGNORE_SCREEN_MODE || src_mode == SCREEN_MODE::IGNORE_SCREEN_MODE)
	{
		*dst_pos = *src_pos;
		return;
	}

	const ScreenSize& src_res = screen_size[src_mode];
	const ScreenSize& dst_res = screen_size[dst_mode];

	float width_scale = 1.0f;
	float height_scale = 1.0f;
	if (dst_mode != src_mode)
	{
		const float width_res_scale = static_cast<float>(dst_res.width) / src_res.width;
		const float height_res_scale = static_cast<float>(dst_res.height) / src_res.height;
		if (src_res.wide_flag == dst_res.wide_flag)
		{
			const bool src_is_hd = src_mode == SCREEN_MODE::WXGA2 || (src_mode >= SCREEN_MODE::HDTV_720 && src_mode <= SCREEN_MODE::HDTV_1080);
			const bool dst_is_hd = dst_mode == SCREEN_MODE::WXGA2 || (dst_mode >= SCREEN_MODE::HDTV_720 && dst_mode <= SCREEN_MODE::HDTV_1080);
			width_scale = width_res_scale;
			height_scale = height_res_scale;
			if (src_is_hd != dst_is_hd)
				height_scale = width_scale;
		}
		else
		{
			assert(!"Unimplemented!");
		}
	}

	height_scale *= src_pos->y - (src_res.height / 2.0f);
	dst_pos->y = height_scale + (dst_res.height / 2.0f);

	width_scale *= (src_pos->x - (src_res.width / 2.0f));
	dst_pos->x = width_scale + (dst_res.width / 2.0f);

	// TODO: This hack present in the game code breaks scaling, figure out how to fix it
	if ((src_mode == SCREEN_MODE::XGA || src_mode == SCREEN_MODE::WXGA) && dst_mode == SCREEN_MODE::HDTV_720)
	{
		const float height_difference = src_res.height - dst_res.height;
		dst_pos->y -= height_difference / 2.0f;
	}
}

void get_screen_conv_scale(vecmath::Vector2<float>* scale, SCREEN_MODE dst_mode, SCREEN_MODE src_mode)
{
	if (scale == nullptr) return;

	*scale = { 1.0f, 1.0f };
	if (dst_mode == SCREEN_MODE::IGNORE_SCREEN_MODE || src_mode == SCREEN_MODE::IGNORE_SCREEN_MODE) return;
	if (dst_mode == src_mode) return;

	const ScreenSize& src_res = screen_size[src_mode];
	const ScreenSize& dst_res = screen_size[dst_mode];

	scale->x = static_cast<float>(dst_res.width) / src_res.width;
	scale->y = static_cast<float>(dst_res.height) / src_res.height;
	if (src_res.wide_flag == dst_res.wide_flag)
	{
		const bool src_is_hd = src_mode == SCREEN_MODE::WXGA2 || (src_mode >= SCREEN_MODE::HDTV_720 && src_mode <= SCREEN_MODE::HDTV_1080);
		const bool dst_is_hd = dst_mode == SCREEN_MODE::WXGA2 || (dst_mode >= SCREEN_MODE::HDTV_720 && dst_mode <= SCREEN_MODE::HDTV_1080);
		if (src_is_hd != dst_is_hd)
			scale->y = scale->x;
	}
	else
	{
		if (src_mode != SCREEN_MODE::XGA)
		{
			if (src_mode == HDTV_720)
			{
				scale->x = scale->y = dst_res.height / 768.0f;
			}
			else if (src_mode == HDTV_1080)
			{
				scale->x = scale->y = dst_res.height / 1200.0f;
			}
		}
		else
		{
			if (dst_mode == SCREEN_MODE::HDTV_720)
			{
				scale->x = scale->y = 1.0f;
			}
		}
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

SCREEN_MODE sys_util_get_startup_screen_mode()
{
	// TODO: Is screen size for letterboxing or internal res scale?
	auto& res = screen_size[SCREEN_MODE::CUSTOM_SCREEN_MODE];
	
	// This memory is write protected, so cheat a bit
	using namespace Memory::VP;
	Patch(&res.width, s_render_window_ptr->GetWidth());
	Patch(&res.render_width, s_render_window_ptr->GetWidth());
	Patch(&res.height, s_render_window_ptr->GetHeight());
	Patch(&res.render_height, s_render_window_ptr->GetHeight());
	res.wide_flag = true;
	res.render_xoffset = 0;
	res.render_yoffset = 0;

	//Patch(&res.wide_flag, false);

	return SCREEN_MODE::HDTV_720;
}