#pragma once

#include "../RenderWindow.h"
#include "sl.h"
#include "gs.h"

void PatchSl(sl::context_t* context);
void PatchGs(gs::context_t* context, const RenderWindow& window);
void ReinstateLogging(void* logFunc);