/*
   This file has been generated by IDA.
   It contains local type definitions from
   the type library 'vf5fs-pxd-w64-retail'
*/

struct CriAtomExPlayerConfigTag;
struct CriManaFrameInfo;
struct CriManaTextureBuffersYUV;
struct CriManaPlaybackBasicWorkConfig;
struct CriManaPlaybackExWorkConfig;
struct CriAtomExStandardVoicePoolConfigTag;
struct CriAtomExWaveformInfoTag;

/* 720 */
enum CriManaPlayerStatus
{
  CRIMANAPLAYER_STATUS_STOP = 0x0,
  CRIMANAPLAYER_STATUS_DECHDR = 0x1,
  CRIMANAPLAYER_STATUS_WAIT_PREP = 0x2,
  CRIMANAPLAYER_STATUS_PREP = 0x3,
  CRIMANAPLAYER_STATUS_READY = 0x4,
  CRIMANAPLAYER_STATUS_PLAYING = 0x5,
  CRIMANAPLAYER_STATUS_PLAYEND = 0x6,
  CRIMANAPLAYER_STATUS_ERROR = 0x7,
  CRIMANAPLAYER_STATUS_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 723 */
enum CriAtomExResumeModeTag
{
  CRIATOMEX_RESUME_ALL_PLAYBACK = 0x0,
  CRIATOMEX_RESUME_PAUSED_PLAYBACK = 0x1,
  CRIATOMEX_RESUME_PREPARED_PLAYBACK = 0x2,
  CRIATOMEX_RESUME_MODE_RESERVED = 0x3,
  CRIATOMEX_RESUME_MODE_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 724 */
enum CriAtomExVoiceAllocationMethodTag
{
  CRIATOMEX_ALLOCATE_VOICE_ONCE = 0x0,
  CRIATOMEX_RETRY_VOICE_ALLOCATION = 0x1,
  CRIATOMEX_VOICE_ALLOCATION_METHOD_IS_4BYTE = 0x7FFFFFFF,
};

/* 5574 */
const struct CriAtomExPlayerConfigTag
{
  CriAtomExVoiceAllocationMethodTag voice_allocation_method;
  int max_path_strings;
  int max_path;
  char max_aisacs;
  int updates_time;
  int enable_audio_synced_timer;
};

/* 5576 */
struct CriManaImageBufferInfo
{
  char *imageptr;
  unsigned int bufsize;
  unsigned int line_pitch;
  unsigned int line_size;
  unsigned int num_lines;
};

/* 728 */
enum CriManaAlphaType
{
  CRIMANA_COMPO_OPAQ = 0x0,
  CRIMANA_COMPO_ALPHFULL = 0x1,
  CRIMANA_COMPO_ALPH3STEP = 0x2,
  CRIMANA_COMPO_ALPH32BIT = 0x3,
  CRIMANA_COMPO_ALPH1BIT = 0x4,
  CRIMANA_COMPO_ALPH2BIT = 0x5,
  CRIMANA_COMPO_ALPH3BIT = 0x6,
  CRIMANA_COMPO_ALPH4BIT = 0x7,
  CRIMANA_COMPO_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 729 */
enum CriManaReferFrameResult
{
  CRIMANA_REFER_RESULT_OK = 0x0,
  CRIMANA_REFER_RESULT_SHORT_INPUT = 0x1,
  CRIMANA_REFER_RESULT_SHORT_CPUTIME = 0x2,
  CRIMANA_REFER_RESULT_NO_MORE_KEEP = 0x3,
  CRIMANA_REFER_RESULT_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 730 */
enum CriManaColorSpaceConversionType
{
  CRIMANA_COLORSPACE_CONVERSION_TYPE_ITU_R_BT601_LIMITED = 0x0,
  CRIMANA_COLORSPACE_CONVERSION_TYPE_ITU_R_BT601_FULLRANGE = 0x1,
  CRIMANA_COLORSPACE_CONVERSION_TYPE_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 5577 */
struct __declspec(align(8)) CriManaFrameInfo
{
  int frame_no;
  int frame_no_per_file;
  unsigned int width;
  unsigned int height;
  unsigned int disp_width;
  unsigned int disp_height;
  unsigned int framerate;
  unsigned int framerate_n;
  unsigned int framerate_d;
  unsigned int total_frames_per_file;
  unsigned __int64 time;
  unsigned __int64 time_per_file;
  unsigned __int64 tunit;
  unsigned int cnt_concatenated_movie;
  int num_images;
  CriManaImageBufferInfo image_info[4];
  int csc_flag;
  CriManaAlphaType alpha_type;
  CriManaReferFrameResult ref_result;
  void *details_ptr[2];
  CriManaColorSpaceConversionType color_conv;
  unsigned int cnt_skipped_frames;
  unsigned int reserved[1];
};

/* 5578 */
struct CriManaTextureBuffer
{
  char *imagebuf;
  unsigned int bufsize;
  unsigned int pitch;
};

/* 5579 */
struct CriManaTextureBuffersYUV
{
  CriManaTextureBuffer y_plane;
  CriManaTextureBuffer u_plane;
  CriManaTextureBuffer v_plane;
  CriManaTextureBuffer a_plane;
};

/* 5580 */
struct CriManaPlaybackCommonParams
{
  int readbuf_size_byte;
  unsigned int max_audio_tracks;
};

/* 726 */
enum CriManaVideoCodecType
{
  CRIMANA_VIDEO_CODEC_UNKNOWN = 0x0,
  CRIMANA_VIDEO_CODEC_SOFDEC_PRIME = 0x1,
  CRIMANA_VIDEO_CODEC_H264 = 0x5,
  CRIMANA_VIDEO_CODEC_VP9 = 0x9,
  CRIMANA_VIDEO_CODEC_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 5581 */
struct CriManaPlaybackVideoParams
{
  int video_flag;
  int max_width;
  int max_height;
  int num_frame_pools;
  CriManaVideoCodecType codec_type;
  int capacity_of_picsize;
  int framerate_n;
  int framerate_d;
};

/* 727 */
enum CriManaAudioCodecType
{
  CRIMANA_AUDIO_CODEC_UNKNOWN = 0x0,
  CRIMANA_AUDIO_CODEC_ADX = 0x2,
  CRIMANA_AUDIO_CODEC_HCA = 0x4,
  CRIMANA_AUDIO_CODEC_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 5582 */
struct CriManaPlaybackAudioParams
{
  int audio_flag;
  unsigned int sampling_rate;
  unsigned int num_channels;
  int output_buffer_samples;
  CriManaAudioCodecType codec_type;
};

/* 5583 */
struct CriManaPlaybackSubtitleParams
{
  int subtitle_flag;
  int max_datasize;
};

/* 5584 */
struct CriManaPlaybackBasicWorkConfig
{
  CriManaPlaybackCommonParams common_params;
  CriManaPlaybackVideoParams video_params;
  CriManaPlaybackAudioParams main_audio_params;
  CriManaPlaybackSubtitleParams subtitle_params;
};

/* 5585 */
struct CriManaPlaybackAlphaParams
{
  int alpha_flag;
  CriManaAlphaType alpha_type;
  int max_width;
  int max_height;
  CriManaVideoCodecType codec_type;
  int capacity_of_picsize;
};

/* 5586 */
struct CriManaPlaybackExWorkConfig
{
  CriManaPlaybackAudioParams sub_audio_params;
  CriManaPlaybackAudioParams extra_audio_params;
  CriManaPlaybackAlphaParams alpha_params;
};

/* 718 */
enum CriAtomSoundRendererTypeTag
{
  CRIATOM_SOUND_RENDERER_NATIVE = 0x1,
  CRIATOM_SOUND_RENDERER_ASR = 0x2,
  CRIATOM_SOUND_RENDERER_HAPTIC = 0x3,
  CRIATOM_SOUND_RENDERER_HW1 = 0x1,
  CRIATOM_SOUND_RENDERER_HW2 = 0x5,
  CRIATOM_SOUND_RENDERER_HW3 = 0x9,
  CRIATOM_SOUND_RENDERER_HW4 = 0xD,
  CRIATOM_SOUND_RENDERER_ANY = 0x0,
  CRIATOM_SOUND_RENDERER_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF,
};

/* 5587 */
struct CriAtomStandardPlayerConfigTag
{
  int max_channels;
  int max_sampling_rate;
  int streaming_flag;
  CriAtomSoundRendererTypeTag sound_renderer_type;
  int decode_latency;
};

/* 5588 */
const struct CriAtomExStandardVoicePoolConfigTag
{
  unsigned int identifier;
  int num_voices;
  CriAtomStandardPlayerConfigTag player_config;
};

/* 5589 */
struct CriAtomExWaveformInfoTag
{
  int wave_id;
  unsigned int format;
  int sampling_rate;
  int num_channels;
  __int64 num_samples;
  int streaming_flag;
  unsigned int reserved[1];
};

class __declspec(novtable) icri
{
public:
	virtual struct CriAtomExPlayerTag *criAtomExPlayer_Create(CriAtomExPlayerConfigTag *, void *, int) = 0;
	// These are private or never used, as IDA defined them as a "gap"
	virtual void gap1() = 0;
	virtual void gap2() = 0;
	virtual void gap3() = 0;
	virtual void gap4() = 0;
	virtual void gap5() = 0;
	virtual void gap6() = 0;
	virtual void gap7() = 0;
	virtual void gap8() = 0;
	virtual void gap9() = 0;
	virtual int criAtomExPlayer_CalculateWorkSize(CriAtomExPlayerConfigTag *) = 0;
	virtual unsigned int criAtomExPlayer_Start(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtomExPlayer_Stop(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtomExPlayer_StopWithoutReleaseTime(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtomExPlayer_ResetParameters(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtomExPlayer_UpdateAll(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtomExPlayer_SetPitch(struct CriAtomExPlayerTag *, float) = 0;
	virtual void criAtomExPlayer_SetPan3dAngle(struct CriAtomExPlayerTag *, float) = 0;
	virtual void criAtomExPlayer_Pause(struct CriAtomExPlayerTag *, int) = 0;
	virtual void criAtomExPlayer_SetAisacControlByName(struct CriAtomExPlayerTag *, const char *, float) = 0;
	virtual int criAtomExPlayer_IsPaused(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtom_ExecuteMain() = 0;
	virtual void criAtomExPlayer_SetData(struct CriAtomExPlayerTag *, void *, int) = 0;
	virtual void criAtomExPlayer_SetFormat(struct CriAtomExPlayerTag *, unsigned int) = 0;
	virtual void criAtomExPlayer_SetNumChannels(struct CriAtomExPlayerTag *, int) = 0;
	virtual void criAtomExPlayer_SetSamplingRate(struct CriAtomExPlayerTag *, int) = 0;
	virtual void criAtomExPlayer_SetFile(struct CriAtomExPlayerTag *, struct CriFsBinderHnObjTag *, const char *) = 0;
	virtual void criAtomExPlayer_LimitLoopCount(struct CriAtomExPlayerTag *, int) = 0;
	virtual void criAtomExPlayer_SetVoicePriority(struct CriAtomExPlayerTag *, int) = 0;
	virtual int criManaPlayer_CalculateHandleWorkSize() = 0;
	virtual struct CriManaPlayerTag* criManaPlayer_Create(void *, int) = 0;
	virtual void criManaPlayer_Destroy(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_SetFile(struct CriManaPlayerTag *, struct CriFsBinderHnObjTag *, const char *) = 0;
	virtual void criManaPlayer_Start(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_Stop(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_StopAndWaitCompletion(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_Pause(struct CriManaPlayerTag *, int) = 0;
	virtual int criManaPlayer_IsPaused(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_GetTime(struct CriManaPlayerTag *, unsigned __int64 *, unsigned __int64 *) = 0;
	virtual CriManaPlayerStatus criManaPlayer_GetStatus(struct CriManaPlayerTag *) = 0;
	virtual int criManaPlayer_ReferFrame(struct CriManaPlayerTag *, CriManaFrameInfo *) = 0;
	virtual int criManaPlayer_IsFrameOnTime(struct CriManaPlayerTag *, CriManaFrameInfo *) = 0;
	virtual void criManaPlayer_CopyFrameToBuffersYUV(struct CriManaPlayerTag *, CriManaFrameInfo *, CriManaTextureBuffersYUV *) = 0;
	virtual void criManaPlayer_DiscardFrame(struct CriManaPlayerTag *, CriManaFrameInfo *) = 0;
	virtual float criManaPlayer_GetVolume(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_SetVolume(struct CriManaPlayerTag *, float) = 0;
	virtual int criManaPlayer_GetPlaybackWorkParam(struct CriManaPlayerTag *, CriManaPlaybackBasicWorkConfig *, CriManaPlaybackExWorkConfig *) = 0;
	virtual int criManaPlayer_CalculatePlaybackWorkSize(struct CriManaPlayerTag *, CriManaPlaybackBasicWorkConfig *, CriManaPlaybackExWorkConfig *) = 0;
	virtual void criManaPlayer_SetPlaybackWork(struct CriManaPlayerTag *, CriManaPlaybackBasicWorkConfig *, CriManaPlaybackExWorkConfig *, void *, int) = 0;
	virtual void criManaPlayer_FreePlaybackWork(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_Prepare(struct CriManaPlayerTag *) = 0;
	virtual void criManaPlayer_DecodeHeader(struct CriManaPlayerTag *) = 0;
	virtual void criMana_SyncMasterTimer() = 0;
	virtual void criMana_ExecuteMain() = 0;
	virtual ~icri() { };
	virtual void *alloc(size_t size, size_t align) = 0;
	virtual void free(void *) = 0;
	virtual int criAtomEx_CalculateWorkSizeForRegisterAcfData(void *, int) = 0;
	virtual void criAtomEx_RegisterAcfData(void *, int, void *, int) = 0;
	virtual void criAtomEx_UnregisterAcf() = 0;
	virtual int criAtomEx_CalculateWorkSizeForDspBusSetting(const char *) = 0;
	virtual int criAtomEx_CalculateWorkSizeForDspBusSettingFromAcfData(void *, int, const char *) = 0;
	virtual void criAtomEx_AttachDspBusSetting(const char *, void *, int) = 0;
	virtual struct CriAtomExVoicePoolTag *criAtomExVoicePool_AllocateStandardVoicePool(CriAtomExStandardVoicePoolConfigTag *, void *, int) = 0;
	virtual void criAtomExVoicePool_Free(struct CriAtomExVoicePoolTag *) = 0;
	virtual void criAtomDbas_Destroy(int) = 0;
	virtual void criAtomEx_DetachDspBusSetting() = 0;
	virtual void criAtomEx_ExecuteMain() = 0;
	virtual void criAtomExPlayer_SetBusSendLevelByName(struct CriAtomExPlayerTag *, const char *, float) = 0;
	virtual void criAtomExPlayer_SetBusSendLevelOffsetByName(struct CriAtomExPlayerTag *, const char *, float) = 0;
	virtual unsigned int criAtomExPlayer_Prepare(struct CriAtomExPlayerTag *) = 0;
	virtual void criAtomExPlayer_SetAisacControlById(struct CriAtomExPlayerTag *, unsigned int, float) = 0;
	virtual void criAtomExPlayer_Resume(struct CriAtomExPlayerTag *, CriAtomExResumeModeTag) = 0;
	virtual void criAtomExPlayer_Update(struct CriAtomExPlayerTag *, unsigned int) = 0;
	virtual void unmount(unsigned int) = 0;
	virtual void remount(unsigned int) = 0;
	virtual void criAtomExPlayer_SetVoicePoolIdentifier(struct CriAtomExPlayerTag *, unsigned int) = 0;
	virtual void criAtomExPlayer_SetDspParameter(struct CriAtomExPlayerTag *, int, float) = 0;
	virtual int criAtomExAcb_GetWaveformInfoByName(struct CriAtomExAcbTag *, const char *, CriAtomExWaveformInfoTag *) = 0;
};