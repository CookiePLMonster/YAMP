#pragma once

#include "../RenderWindow.h"
#include "sl.h"
#include "gs.h"

#include "../Imports.h"

void PatchSl(sl::context_t* context);
void PatchGs(gs::context_t* context, const RenderWindow& window);
void ReinstateLogging(void* dll, const Imports& symbols);
void InjectTraps(const Imports& symbols);

void Patch_SysUtil(void* dll, const Imports& symbols);
void Patch_CsGame(void* dll, const Imports& symbols);
void Patch_Misc(void* dll, const Imports& symbols);