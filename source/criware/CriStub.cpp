#include "CriStub.h"

// Stub implementation for icri.h interfaces
CriAtomExPlayerTag* CriStub::criAtomExPlayer_Create(CriAtomExPlayerConfigTag*, void*, int)
{
	return nullptr;
}

void CriStub::gap1()
{
}

void CriStub::gap2()
{
}

void CriStub::gap3()
{
}

void CriStub::gap4()
{
}

void CriStub::gap5()
{
}

void CriStub::gap6()
{
}

void CriStub::gap7()
{
}

void CriStub::gap8()
{
}

void CriStub::gap9()
{
}

int CriStub::criAtomExPlayer_CalculateWorkSize(CriAtomExPlayerConfigTag*)
{
	return 0;
}

unsigned int CriStub::criAtomExPlayer_Start(CriAtomExPlayerTag*)
{
	return 0;
}

void CriStub::criAtomExPlayer_Stop(CriAtomExPlayerTag*)
{
}

void CriStub::criAtomExPlayer_StopWithoutReleaseTime(CriAtomExPlayerTag*)
{
}

void CriStub::criAtomExPlayer_ResetParameters(CriAtomExPlayerTag*)
{
}

void CriStub::criAtomExPlayer_UpdateAll(CriAtomExPlayerTag*)
{
}

void CriStub::criAtomExPlayer_SetPitch(CriAtomExPlayerTag*, float)
{
}

void CriStub::criAtomExPlayer_SetPan3dAngle(CriAtomExPlayerTag*, float)
{
}

void CriStub::criAtomExPlayer_Pause(CriAtomExPlayerTag*, int)
{
}

void CriStub::criAtomExPlayer_SetAisacControlByName(CriAtomExPlayerTag*, const char*, float)
{
}

int CriStub::criAtomExPlayer_IsPaused(CriAtomExPlayerTag*)
{
	return 0;
}

void CriStub::criAtom_ExecuteMain()
{
}

void CriStub::criAtomExPlayer_SetData(CriAtomExPlayerTag*, void*, int)
{
}

void CriStub::criAtomExPlayer_SetFormat(CriAtomExPlayerTag*, unsigned int)
{
}

void CriStub::criAtomExPlayer_SetNumChannels(CriAtomExPlayerTag*, int)
{
}

void CriStub::criAtomExPlayer_SetSamplingRate(CriAtomExPlayerTag*, int)
{
}

void CriStub::criAtomExPlayer_SetFile(CriAtomExPlayerTag*, CriFsBinderHnObjTag*, const char*)
{
}

void CriStub::criAtomExPlayer_LimitLoopCount(CriAtomExPlayerTag*, int)
{
}

void CriStub::criAtomExPlayer_SetVoicePriority(CriAtomExPlayerTag*, int)
{
}

int CriStub::criManaPlayer_CalculateHandleWorkSize()
{
	return 0;
}

CriManaPlayerTag* CriStub::criManaPlayer_Create(void*, int)
{
	return nullptr;
}

void CriStub::criManaPlayer_Destroy(CriManaPlayerTag*)
{
}

void CriStub::criManaPlayer_SetFile(CriManaPlayerTag*, CriFsBinderHnObjTag*, const char*)
{
}

void CriStub::criManaPlayer_Start(CriManaPlayerTag*)
{
}

void CriStub::criManaPlayer_Stop(CriManaPlayerTag*)
{
}

void CriStub::criManaPlayer_StopAndWaitCompletion(CriManaPlayerTag*)
{
}

void CriStub::criManaPlayer_Pause(CriManaPlayerTag*, int)
{
}

int CriStub::criManaPlayer_IsPaused(CriManaPlayerTag*)
{
	return 0;
}

void CriStub::criManaPlayer_GetTime(CriManaPlayerTag*, unsigned __int64*, unsigned __int64*)
{
}

CriManaPlayerStatus CriStub::criManaPlayer_GetStatus(CriManaPlayerTag*)
{
	return CriManaPlayerStatus();
}

int CriStub::criManaPlayer_ReferFrame(CriManaPlayerTag*, CriManaFrameInfo*)
{
	return 0;
}

int CriStub::criManaPlayer_IsFrameOnTime(CriManaPlayerTag*, CriManaFrameInfo*)
{
	return 0;
}

void CriStub::criManaPlayer_CopyFrameToBuffersYUV(CriManaPlayerTag*, CriManaFrameInfo*, CriManaTextureBuffersYUV*)
{
}

void CriStub::criManaPlayer_DiscardFrame(CriManaPlayerTag*, CriManaFrameInfo*)
{
}

float CriStub::criManaPlayer_GetVolume(CriManaPlayerTag*)
{
	return 0.0f;
}

void CriStub::criManaPlayer_SetVolume(CriManaPlayerTag*, float)
{
}

int CriStub::criManaPlayer_GetPlaybackWorkParam(CriManaPlayerTag*, CriManaPlaybackBasicWorkConfig*, CriManaPlaybackExWorkConfig*)
{
	return 0;
}

int CriStub::criManaPlayer_CalculatePlaybackWorkSize(CriManaPlayerTag*, CriManaPlaybackBasicWorkConfig*, CriManaPlaybackExWorkConfig*)
{
	return 0;
}

void CriStub::criManaPlayer_SetPlaybackWork(CriManaPlayerTag*, CriManaPlaybackBasicWorkConfig*, CriManaPlaybackExWorkConfig*, void*, int)
{
}

void CriStub::criManaPlayer_FreePlaybackWork(CriManaPlayerTag*)
{
}

void CriStub::criManaPlayer_Prepare(CriManaPlayerTag*)
{
}

void CriStub::criManaPlayer_DecodeHeader(CriManaPlayerTag*)
{
}

void CriStub::criMana_SyncMasterTimer()
{
}

void CriStub::criMana_ExecuteMain()
{
}

void* CriStub::alloc(size_t size, size_t align)
{
	return nullptr;
}

void CriStub::free(void*)
{
}

int CriStub::criAtomEx_CalculateWorkSizeForRegisterAcfData(void*, int)
{
	return 0;
}

void CriStub::criAtomEx_RegisterAcfData(void*, int, void*, int)
{
}

void CriStub::criAtomEx_UnregisterAcf()
{
}

int CriStub::criAtomEx_CalculateWorkSizeForDspBusSetting(const char*)
{
	return 0;
}

int CriStub::criAtomEx_CalculateWorkSizeForDspBusSettingFromAcfData(void*, int, const char*)
{
	return 0;
}

void CriStub::criAtomEx_AttachDspBusSetting(const char*, void*, int)
{
}

CriAtomExVoicePoolTag* CriStub::criAtomExVoicePool_AllocateStandardVoicePool(CriAtomExStandardVoicePoolConfigTag*, void*, int)
{
	return nullptr;
}

void CriStub::criAtomExVoicePool_Free(CriAtomExVoicePoolTag*)
{
}

void CriStub::criAtomDbas_Destroy(int)
{
}

void CriStub::criAtomEx_DetachDspBusSetting()
{
}

void CriStub::criAtomEx_ExecuteMain()
{
}

void CriStub::criAtomExPlayer_SetBusSendLevelByName(CriAtomExPlayerTag*, const char*, float)
{
}

void CriStub::criAtomExPlayer_SetBusSendLevelOffsetByName(CriAtomExPlayerTag*, const char*, float)
{
}

unsigned int CriStub::criAtomExPlayer_Prepare(CriAtomExPlayerTag*)
{
	return 0;
}

void CriStub::criAtomExPlayer_SetAisacControlById(CriAtomExPlayerTag*, unsigned int, float)
{
}

void CriStub::criAtomExPlayer_Resume(CriAtomExPlayerTag*, CriAtomExResumeModeTag)
{
}

void CriStub::criAtomExPlayer_Update(CriAtomExPlayerTag*, unsigned int)
{
}

void CriStub::unmount(unsigned int)
{
}

void CriStub::remount(unsigned int)
{
}

void CriStub::criAtomExPlayer_SetVoicePoolIdentifier(CriAtomExPlayerTag*, unsigned int)
{
}

void CriStub::criAtomExPlayer_SetDspParameter(CriAtomExPlayerTag*, int, float)
{
}

int CriStub::criAtomExAcb_GetWaveformInfoByName(CriAtomExAcbTag*, const char*, CriAtomExWaveformInfoTag*)
{
	return 0;
}
