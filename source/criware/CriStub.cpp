#include "CriStub.h"
#include <Windows.h>
#include <intrin.h>

#include <cstdlib>

struct CriAtomExPlayerTag {};

// TODO: None of these are final
namespace XAudio2Impl
{
	class AtomExPlayer : public CriAtomExPlayerTag
	{
	};
}

// Stub implementation for icri.h interfaces
CriAtomExPlayerTag* CriStub::criAtomExPlayer_Create(CriAtomExPlayerConfigTag*, void*, int)
{
	return new XAudio2Impl::AtomExPlayer;
}

int CriStub::criAtomExPlayer_CalculateWorkSize(CriAtomExPlayerConfigTag*)
{
	// TODO
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return sizeof(void*);
}

unsigned int CriStub::criAtomExPlayer_Start(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return 0;
}

void CriStub::criAtomExPlayer_Stop(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_StopWithoutReleaseTime(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_ResetParameters(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_UpdateAll(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetPitch(CriAtomExPlayerTag*, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetPan3dAngle(CriAtomExPlayerTag*, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_Pause(CriAtomExPlayerTag*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetAisacControlByName(CriAtomExPlayerTag*, const char*, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

int CriStub::criAtomExPlayer_IsPaused(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return 0;
}

void CriStub::criAtom_ExecuteMain()
{
	// TODO
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetData(CriAtomExPlayerTag*, void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetFormat(CriAtomExPlayerTag*, unsigned int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetNumChannels(CriAtomExPlayerTag*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetSamplingRate(CriAtomExPlayerTag*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetFile(CriAtomExPlayerTag*, CriFsBinderHnObjTag*, const char*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_LimitLoopCount(CriAtomExPlayerTag*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetVoicePriority(CriAtomExPlayerTag*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

int CriStub::criManaPlayer_CalculateHandleWorkSize()
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return sizeof(void*);
}

CriManaPlayerTag* CriStub::criManaPlayer_Create(void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return nullptr;
}

void CriStub::criManaPlayer_Destroy(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_SetFile(CriManaPlayerTag*, CriFsBinderHnObjTag*, const char*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criManaPlayer_Start(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_Stop(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_StopAndWaitCompletion(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_Pause(CriManaPlayerTag*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

int CriStub::criManaPlayer_IsPaused(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criManaPlayer_GetTime(CriManaPlayerTag*, unsigned __int64*, unsigned __int64*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

CriManaPlayerStatus CriStub::criManaPlayer_GetStatus(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return CriManaPlayerStatus();
}

int CriStub::criManaPlayer_ReferFrame(CriManaPlayerTag*, CriManaFrameInfo*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

int CriStub::criManaPlayer_IsFrameOnTime(CriManaPlayerTag*, CriManaFrameInfo*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criManaPlayer_CopyFrameToBuffersYUV(CriManaPlayerTag*, CriManaFrameInfo*, CriManaTextureBuffersYUV*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_DiscardFrame(CriManaPlayerTag*, CriManaFrameInfo*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

float CriStub::criManaPlayer_GetVolume(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0.0f;
}

void CriStub::criManaPlayer_SetVolume(CriManaPlayerTag*, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

int CriStub::criManaPlayer_GetPlaybackWorkParam(CriManaPlayerTag*, CriManaPlaybackBasicWorkConfig*, CriManaPlaybackExWorkConfig*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

int CriStub::criManaPlayer_CalculatePlaybackWorkSize(CriManaPlayerTag*, CriManaPlaybackBasicWorkConfig*, CriManaPlaybackExWorkConfig*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criManaPlayer_SetPlaybackWork(CriManaPlayerTag*, CriManaPlaybackBasicWorkConfig*, CriManaPlaybackExWorkConfig*, void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_FreePlaybackWork(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_Prepare(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criManaPlayer_DecodeHeader(CriManaPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criMana_SyncMasterTimer()
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criMana_ExecuteMain()
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void* CriStub::alloc(size_t size, size_t align)
{
	return _aligned_malloc(size, align);
}

void CriStub::free(void* mem)
{
	_aligned_free(mem);
}

int CriStub::criAtomEx_CalculateWorkSizeForRegisterAcfData(void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criAtomEx_RegisterAcfData(void*, int, void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomEx_UnregisterAcf()
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

int CriStub::criAtomEx_CalculateWorkSizeForDspBusSetting(const char*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

int CriStub::criAtomEx_CalculateWorkSizeForDspBusSettingFromAcfData(void*, int, const char*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criAtomEx_AttachDspBusSetting(const char*, void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

CriAtomExVoicePoolTag* CriStub::criAtomExVoicePool_AllocateStandardVoicePool(CriAtomExStandardVoicePoolConfigTag*, void*, int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return nullptr;
}

void CriStub::criAtomExVoicePool_Free(CriAtomExVoicePoolTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomDbas_Destroy(int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomEx_DetachDspBusSetting()
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomEx_ExecuteMain()
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetBusSendLevelByName(CriAtomExPlayerTag*, const char*, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetBusSendLevelOffsetByName(CriAtomExPlayerTag*, const char*, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

unsigned int CriStub::criAtomExPlayer_Prepare(CriAtomExPlayerTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criAtomExPlayer_SetAisacControlById(CriAtomExPlayerTag*, unsigned int, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_Resume(CriAtomExPlayerTag*, CriAtomExResumeModeTag)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_Update(CriAtomExPlayerTag*, unsigned int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::unmount(unsigned int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::remount(unsigned int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetVoicePoolIdentifier(CriAtomExPlayerTag*, unsigned int)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetDspParameter(CriAtomExPlayerTag*, int, float)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

int CriStub::criAtomExAcb_GetWaveformInfoByName(CriAtomExAcbTag*, const char*, CriAtomExWaveformInfoTag*)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
	return 0;
}

void CriStub::criAtomExPlayer_Destroy(CriAtomExPlayerTag* player)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

CriAtomExPlayerStatusTag CriStub::criAtomExPlayer_GetStatus(CriAtomExPlayerTag* player)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return CriAtomExPlayerStatusTag();
}

struct CriAtomExAcbTag* CriStub::criAtomExAcb_LoadAcbData(void* acb_data, int acb_data_size, CriFsBinderHnObjTag* awb_binder, const char* awb_path, void* work, int work_size)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return nullptr;
}

int CriStub::criAtomExAcb_CalculateWorkSizeForLoadAcbData(void* acb_data, int acb_data_size, CriFsBinderHnObjTag* awb_binder, const char* awb_path)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
	return 0;
}

void CriStub::criAtomExAcb_Release(CriAtomExAcbTag* acb_hn)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_AttachAisac(CriAtomExPlayerTag* player, const char* global_aisac_name)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_DetachAisac(CriAtomExPlayerTag* player, const char* global_aisac_name)
{
	OutputDebugStringA(__FUNCTION__ "\n"); __debugbreak();
}

void CriStub::criAtomExPlayer_SetCueName(CriAtomExPlayerTag* player, CriAtomExAcbTag* acb_hn, const char* cue_name)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}

void CriStub::criAtomExPlayer_SetVolume(CriAtomExPlayerTag* player, float volume)
{
	OutputDebugStringA(__FUNCTION__ "\n"); //__debugbreak();
}
