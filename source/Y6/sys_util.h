#pragma once

bool sys_util_check_enable_storage(int port);
void sys_util_start_load_systemdata_task(int port, void* buf, unsigned int buf_size, bool autoload, bool create, void (*cbfunc)(bool success, bool creator));
void sys_util_start_save_systemdata_task(int port, const void* buf, unsigned int buf_size, bool autosave, void (*cbfunc)(bool success, bool unknown));

// Custom
void sys_util_init_storage_directory();