#pragma once

#include "../RenderWindow.h"
#include "sl.h"
#include "gs.h"

#include <forward_list>

void PatchSl(sl::context_t* context);
void PatchGs(gs::context_t* context, const RenderWindow& window);
void ReinstateLogging(void* logFunc);
void InjectTraps(const std::forward_list<void*>& addresses);

void Patch_SysUtil(void* dll);