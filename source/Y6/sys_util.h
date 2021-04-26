#pragma once

enum SCREEN_MODE
{
	QVGA = 0x0,
	VGA = 0x1,
	SVGA = 0x2,
	XGA = 0x3,
	SXGA = 0x4,
	SXGA_P = 0x5,
	UXGA = 0x6,
	WVGA = 0x7,
	WSVGA = 0x8,
	WXGA = 0x9,
	WXGA2 = 0xA,
	WUXGA = 0xB,
	WQXGA = 0xC,
	HDTV_720 = 0xD,
	HDTV_1080 = 0xE,
	SCREEN_MODE_MAX = 0xF,
	IGNORE_SCREEN_MODE = 0xF,

	// Added
	CUSTOM_SCREEN_MODE = 0x10
};

struct ScreenSize
{
	int width;
	int height;
	bool wide_flag;
	int render_xoffset;
	int render_yoffset;
	int render_width;
	int render_height;
};
inline ScreenSize* screen_size;

bool sys_util_check_enable_storage(int port);
void sys_util_start_load_systemdata_task(int port, void* buf, unsigned int buf_size, bool autoload, bool create, void (*cbfunc)(bool success, bool creator));
void sys_util_start_save_systemdata_task(int port, const void* buf, unsigned int buf_size, bool autosave, void (*cbfunc)(bool success, bool unknown));
SCREEN_MODE sys_util_get_startup_screen_mode();

// Custom
void sys_util_init_storage_directory();
void sys_util_get_render_window(const class RenderWindow& window);