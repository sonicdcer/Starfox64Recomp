#include "patches.h"
#include "sf64audio_provisional.h"
#include "audioseq_cmd.h"
#include "audiothread_cmd.h"

#if (DEBUG_AUDIO_LOCALIZATION == 1)
#if DEBUG_JP_AUDIO == 1
#include "audio_jp/audio_bank_jp.c"
#include "audio_jp/audio_seq_jp.c"
#include "audio_jp/audio_table_jp.c"
#include "audio_jp/audiotables_jp.c"
#define audio_seq_localized audio_seq_jp
#define audio_bank_localized audio_bank_jp
#define audio_table_localized audio_table_jp
#define gAudioSpecs_localized gAudioSpecs
#define gSeqTableInit_localized gSeqTableInit_jp
#define gSoundFontTableInit_localized gSoundFontTableInit_jp
#define gSampleBankTableInit_localized gSampleBankTableInit_jp
#define gSeqFontTableInit_localized gSeqFontTableInit_jp
#endif

#if DEBUG_US_AUDIO == 1
#include "audio_us/audio_bank_us.c"
#include "audio_us/audio_seq_us.c"
#include "audio_us/audio_table_us.c"
#include "audio_us/audiotables_us.c"
#define audio_seq_localized audio_seq_us
#define audio_bank_localized audio_bank_us
#define audio_table_localized audio_table_us
#define gAudioSpecs_localized gAudioSpecs
#define gSeqTableInit_localized gSeqTableInit
#define gSoundFontTableInit_localized gSoundFontTableInit
#define gSampleBankTableInit_localized gSampleBankTableInit
#define gSeqFontTableInit_localized gSeqFontTableInit
#endif

#if DEBUG_EU_AUDIO == 1
#define VERSION_EU
#if 0 // Original EU ROM
#include "audio_eu/audio_bank_eu.c"
#include "audio_eu/audio_seq_eu.c"
#include "audio_eu/audio_table_eu.c"
#else // SPANISH MOD
#include "audio_eu/audio_bank_eu_mod.c"
#include "audio_eu/audio_seq_eu_mod.c"
#include "audio_eu/audio_table_eu_mod.c"
#endif
#include "audio_eu/audiotables_eu.c"
#include "audio_eu/audiospecs_eu.c"
#define audio_seq_localized audio_seq_eu
#define audio_bank_localized audio_bank_eu
#define audio_table_localized audio_table_eu
#define gSeqTableInit_localized gSeqTableInit_eu
#define gAudioSpecs_localized gAudioSpecs_eu
#define gSoundFontTableInit_localized gSoundFontTableInit_eu
#define gSampleBankTableInit_localized gSampleBankTableInit_eu
#define gSeqFontTableInit_localized gSeqFontTableInit_eu
#endif
#endif

extern s32 gAudioHeapSize;
u8 gVoiceLanguage = 0;
extern unsigned char audio_seq_us[];
extern unsigned char audio_bank_us[];
extern unsigned char audio_table_us[];

extern AudioTable gSeqTableInit_jp;
extern AudioTable gSampleBankTableInit_jp;
extern s32 gSoundFontTableInit_jp[];
extern s32 gSeqFontTableInit_jp[];
extern SfxBankEntry sSfxBanks[5][20];

#define SEGMENT_ROM_START(segment) (segment##(void*) 0x00000000)

void AudioLoad_InitTable(AudioTable* table, u8* romAddr, u16 unkMediumParam);
s32 AudioHeap_ResetStep(void);

#if DEBUG_AUDIO_LOCALIZATION == 1
RECOMP_PATCH void AudioLoad_Init(void) __attribute__((optnone)) {
    s32 pad[14];
    s32 i;
    s32 j;
    s32 numFonts;
    s32 dwordsLeft;
    u64* clearContext;
    void* ramAddr;

    gAudioResetTimer = 0;

    for (i = 0; i < gAudioHeapSize / 8; i++) {
        *((u64*) gAudioHeap + i) = 0;
    }

    clearContext = gAudioContextStart;
    dwordsLeft = ((u32) gAudioContextEnd - (u32) gAudioContextStart) / 8;
    for (; dwordsLeft >= 0; dwordsLeft--) {
        *clearContext++ = 0;
    }

    gMaxTempoTvTypeFactors = 1000 * 1.00278f / 60;
    gRefreshRate = 60;

    AudioThread_Init();

    for (i = 0; i < 3; i++) {
        gAiBuffLengths[i] = 0xA0;
    }

    gAudioTaskCountQ = 0;
    gAudioTaskIndexQ = 0;
    gCurAiBuffIndex = 0;
    gAudioSoundMode = SOUNDMODE_STEREO;
    gAudioCurTask = NULL;
    gAudioRspTasks[0].task.t.data_size = 0;
    gAudioRspTasks[1].task.t.data_size = 0;

    osCreateMesgQueue(&gSyncDmaQueue, gSyncDmaMsg, 1);
    osCreateMesgQueue(&gCurAudioFrameDmaQueue, gCurAudioFrameDmaMsg, 64);
    osCreateMesgQueue(&gExternalLoadQueue, gExternalLoadMsg, 16);
    osCreateMesgQueue(&gPreloadSampleQueue, gPreloadSampleMsg, 16);

    gCurAudioFrameDmaCount = 0;
    gSampleDmaCount = 0;

    AudioHeap_InitMainPools(gInitPoolSize);

    for (i = 0; i < 3; i++) {
        gAiBuffers[i] = AudioHeap_Alloc(&gInitPool, AIBUF_SIZE);
        for (j = 0; j < AIBUF_LEN; j++) {
            gAiBuffers[i][j] = 0;
        }
    }

    gAudioSpecId = AUDIOSPEC_CO;
    gAudioResetStep = 1;
    AudioHeap_ResetStep();

    gSequenceTable = (AudioTable*) &gSeqTableInit_localized;
    gSoundFontTable = (AudioTable*) &gSoundFontTableInit_localized;
    gSampleBankTable = (AudioTable*) &gSampleBankTableInit_localized;
    gSeqFontTable = (u8*) gSeqFontTableInit_localized;

    gNumSequences = gSequenceTable->base.numEntries;

    // @recomp: use audio tables from data
    recomp_printf("gSequenceTable->base.numEntries: %d \n", gSequenceTable->base.numEntries);
    AudioLoad_InitTable(gSequenceTable, (u8*) audio_seq_localized, gSequenceMedium);
    recomp_printf("gSoundFontTable->base.numEntries: %d \n", gSoundFontTable->base.numEntries);
    AudioLoad_InitTable(gSoundFontTable, (u8*) audio_bank_localized, gSoundFontMedium);
    recomp_printf("gSampleBankTable->base.numEntries: %d \n", gSampleBankTable->base.numEntries);
    AudioLoad_InitTable(gSampleBankTable, (u8*) audio_table_localized, gSampleBankMedium);

    numFonts = gSoundFontTable->base.numEntries;

    gSoundFontList = AudioHeap_Alloc(&gInitPool, numFonts * sizeof(SoundFont));

    for (i = 0; i < numFonts; i++) {
        gSoundFontList[i].sampleBankId1 = (gSoundFontTable->entries[i].shortData1 >> 8) & 0xFF;
        gSoundFontList[i].sampleBankId2 = gSoundFontTable->entries[i].shortData1 & 0xFF;
        gSoundFontList[i].numInstruments = (gSoundFontTable->entries[i].shortData2 >> 8) & 0xFF;
        gSoundFontList[i].numDrums = gSoundFontTable->entries[i].shortData2 & 0xFF;
    }

    ramAddr = AudioHeap_Alloc(&gInitPool, gPermanentPoolSize);
    if (ramAddr == NULL) {
        gPermanentPoolSize = 0;
    }

    AudioHeap_InitPool(&gPermanentPool.pool, ramAddr, gPermanentPoolSize);
    AudioSeq_InitSequencePlayers();
}

RECOMP_PATCH void AudioLoad_InitTable(AudioTable* table, u8* romAddr, u16 unkMediumParam) {
    s32 i;

    table->base.unkMediumParam = unkMediumParam;
    table->base.romAddr = (uintptr_t) romAddr;

    for (i = 0; i < table->base.numEntries; i++) {
        if ((table->entries[i].size != 0) && (table->entries[i].medium == MEDIUM_CART)) {
            table->entries[i].romAddr += (uintptr_t) romAddr;
            recomp_printf("table->entries[i].romAddr: %x \n", table->entries[i].romAddr);
        }
    }
}
#endif

#if DEBUG_EU_AUDIO == 1 && DEBUG_AUDIO_LOCALIZATION == 1

#include "audio_eu\ast_radio_mod.c"

s32 func_radio_800BA7BC(u16* msg, s32 priority);

u16* Message_PtrFromId_SPA(u16 msgId) {
    s32 i;
    MsgLookup* lookup = gMsgLookup_SPA;

    while (lookup->msgId != -1) {
        if (lookup->msgId == msgId) {
            return lookup->msgPtr;
        }
        lookup++;
    }
    return NULL;
}

RECOMP_PATCH void Radio_PlayMessage(u16* msg, RadioCharacterId character) {
    TeamId teamId;
    s32 pad;
    s32 priority;

    gRadioMsgId = Message_IdFromPtr(msg);

    msg = Message_PtrFromId_SPA(gRadioMsgId); // This should get the replacement text pointer

    switch (msg[0]) {
        default:
        case MSGCHAR_PRI0:
            priority = 0;
            break;
        case MSGCHAR_PRI1:
            priority = 1;
            break;
        case MSGCHAR_PRI2:
            priority = 2;
            break;
        case MSGCHAR_PRI3:
            priority = 3;
            break;
    }

    if (gGameState == GSTATE_PLAY) {
        if ((character == RCID_FALCO) || (character == RCID_SLIPPY) || (character == RCID_PEPPY)) {
            if (character == RCID_FALCO) {
                teamId = TEAM_ID_FALCO;
            }
            if (character == RCID_SLIPPY) {
                teamId = TEAM_ID_SLIPPY;
            }
            if (character == RCID_PEPPY) {
                teamId = TEAM_ID_PEPPY;
            }
            if ((gTeamShields[teamId] <= 0) && (gTeamShields[teamId] != -2)) {
                return;
            }
        }
    }

    if ((gRadioState != 0) && (func_radio_800BA7BC(msg, priority) == 1)) {
        return;
    }

    gRadioMsgPri = priority;
    gRadioMsgRadioId = character;

    if (gExpertMode && ((character == RCID_FOX) || (character == RCID_FOX_RED))) {
        gRadioMsgRadioId = RCID_FOX_EXPERT;
    }

    if (gCurrentLevel == LEVEL_VENOM_2) {
        switch (character) {
            case RCID_WOLF:
                gRadioMsgRadioId = RCID_WOLF_2;
                break;
            case RCID_PIGMA:
                gRadioMsgRadioId = RCID_PIGMA_2;
                break;
            case RCID_LEON:
                gRadioMsgRadioId = RCID_LEON_2;
                break;
            case RCID_ANDREW:
                gRadioMsgRadioId = RCID_ANDREW_2;
                break;
        }
    }

    gRadioMsg = msg;
    gRadioState = 100;

    switch (gGameState) {
        case GSTATE_TITLE:
        case GSTATE_ENDING:
            gRadioPrintPosY = 176;
            gRadioPrintPosX = 85;
            gRadioTextBoxPosX = 80.0f;
            gRadioTextBoxPosY = 174.0f;
            gRadioTextBoxScaleX = 4.63f;
            gRadioPortraitPosX = 32.0f;
            gRadioPortraitPosY = 174.0f;
            break;

        case GSTATE_PLAY:
            gRadioPrintPosY = 180;
            gRadioPrintPosX = 79;
            gRadioTextBoxPosX = 74.0f;
            gRadioTextBoxPosY = 178.0f;
            gRadioTextBoxScaleX = 4.53f;
            gRadioPortraitPosX = 26.0f;
            gRadioPortraitPosY = 178.0f;
            break;
    }

//    gRadioMsgId = Message_IdFromPtr(msg);
    Audio_PlayVoice(gRadioMsgId);
}

RECOMP_PATCH void Message_DisplayScrollingText(Gfx** gfxPtr, u16* msgPtr, s32 xPos, s32 yPos, s32 yRangeHi, s32 yRangeLo, s32 len) {
    s32 x = xPos;
    s32 y = yPos;
    s32 i;

    msgPtr = Message_PtrFromId_SPA(Message_IdFromPtr(msgPtr)); // This should get the replacement text pointer

    gDPSetTextureLUT((*gfxPtr)++, G_TT_RGBA16);
    gDPLoadTLUT((*gfxPtr)++, 64, 256, gTextCharPalettes);

    for (i = 0; msgPtr[i] != 0 && i < len; i++) {
        switch (msgPtr[i]) {
            case MSGCHAR_NWL:
                x = xPos;
                y += 15;
                break;

            case MSGCHAR_QSP:
                x += 2;
                break;

            case MSGCHAR_HSP:
                x += 3;
                break;

            case MSGCHAR_SPC:
                x += 7;
                break;

            default:
                if ((yRangeLo < y) && (y < yRangeHi)) {
                    Message_DisplayChar(gfxPtr, msgPtr[i], x, y);
                }
                x += 7;
                break;

            case MSGCHAR_NP2:
            case MSGCHAR_NP3:
            case MSGCHAR_NP4:
            case MSGCHAR_NP5:
            case MSGCHAR_NP6:
            case MSGCHAR_NP7:
            case MSGCHAR_PRI0:
            case MSGCHAR_PRI1:
            case MSGCHAR_PRI2:
            case MSGCHAR_PRI3:
            case MSGCHAR_NXT:
                break;
        }
    }
}
#endif

#if DEBUG_EU_AUDIO == 1 && DEBUG_AUDIO_LOCALIZATION == 1
f32 Audio_GetSfxFalloff(u8 bankId, u8 entryIndex);
s8 Audio_GetSfxReverb(u8 bankId, u8 entryIndex, u8 channelId);
f32 Audio_GetSfxFreqMod(u8 bankId, u8 entryIndex);
s8 Audio_GetSfxPan(f32 xPos, f32 zPos, u8 mode);

extern SfxChannelState sSfxChannelState[16];
;
extern Modulation sSfxVolumeMods[5];
extern u8 sSfxLayout;

RECOMP_PATCH void Audio_SetSfxProperties(u8 bankId, u8 entryIndex, u8 channelId) {
    f32 volumeMod = 1.0f;
    s8 reverb = 0;
    f32 freqMod = 1.0f;
    s8 pan = 64;
    SfxBankEntry* entry = &sSfxBanks[bankId][entryIndex];

    switch (bankId) {
        case SFX_BANK_PLAYER:
        case SFX_BANK_1:
        case SFX_BANK_2:
        case SFX_BANK_3:
            if (entry->state == 2) {
                AUDIOCMD_CHANNEL_SET_IO(SEQ_PLAYER_SFX, channelId, 1, gLevelType);
            }
            if ((entry->sfxId & SFX_FLAG_18) && (*entry->zPos > 0.0f)) {
                f32 yScaled = *entry->yPos / 2.5f;

                entry->distance = SQ(*entry->xPos) + SQ(yScaled);
            }
            entry->distance = sqrtf(entry->distance);
            volumeMod = Audio_GetSfxFalloff(bankId, entryIndex) * *entry->volMod * sSfxVolumeMods[bankId].value;
            reverb = Audio_GetSfxReverb(bankId, entryIndex, channelId);
            freqMod = Audio_GetSfxFreqMod(bankId, entryIndex) * *entry->freqMod;
            if (!((bankId == SFX_BANK_PLAYER) && ((-200.0f < *entry->zPos) && (*entry->zPos < 200.0f)) &&
                  (sSfxLayout != SFX_LAYOUT_VS))) {
                pan = Audio_GetSfxPan(*entry->xPos, *entry->zPos, entry->token);
            }
            break;
        case SFX_BANK_SYSTEM:
#ifdef VERSION_EU
            if (entry->state == 2) {
                AUDIOCMD_CHANNEL_SET_IO(SEQ_PLAYER_SFX, channelId, 1, gVoiceLanguage);
            }
#endif
            if (sSfxLayout == SFX_LAYOUT_VS) {
                if (entry->token != 4) {
                    pan = (entry->token & 1) * 127;
                }
            }
            break;
    }
    if (volumeMod != sSfxChannelState[channelId].volMod) {
        AUDIOCMD_CHANNEL_SET_VOL_SCALE(SEQ_PLAYER_SFX, channelId, volumeMod);
        sSfxChannelState[channelId].volMod = volumeMod;
    }
    if (reverb != sSfxChannelState[channelId].reverb) {
        AUDIOCMD_CHANNEL_SET_REVERB_VOLUME(SEQ_PLAYER_SFX, channelId, reverb);
        sSfxChannelState[channelId].reverb = reverb;
    }
    if (freqMod != sSfxChannelState[channelId].freqMod) {
        AUDIOCMD_CHANNEL_SET_FREQ_SCALE(SEQ_PLAYER_SFX, channelId, freqMod);
        sSfxChannelState[channelId].freqMod = freqMod;
    }
    if (pan != sSfxChannelState[channelId].pan) {
        AUDIOCMD_CHANNEL_SET_PAN(SEQ_PLAYER_SFX, channelId, pan);
        sSfxChannelState[channelId].pan = pan;
    }
}
#endif

#if DEBUG_EU_AUDIO == 1 && DEBUG_AUDIO_LOCALIZATION == 1
void Audio_ResetVoicesAndPlayers(void);
void Audio_ResetSfxChannelState(void);
void Audio_ResetActiveSequencesAndVolume(void);
void Audio_ResetSfx(void);
void Audio_StartSequence(u8 seqPlayId, u8 seqId, u8 seqArgs, u16 fadeInTime);
void AudioThread_QueueCmdS32(u32 opArgs, u32 val);

RECOMP_PATCH void Audio_InitSounds(void) {
    Audio_ResetVoicesAndPlayers();
    Audio_ResetSfxChannelState();
    Audio_ResetActiveSequencesAndVolume();
    Audio_ResetSfx();
#ifdef VERSION_EU
    AUDIOCMD_GLOBAL_SYNC_LOAD_SEQ_PARTS(0x42 /*NA_BGM_VO_LYLAT*/, 0);
#endif
    Audio_StartSequence(SEQ_PLAYER_VOICE, NA_BGM_VO, -1, 1);
    Audio_StartSequence(SEQ_PLAYER_SFX, NA_BGM_SE, -1, 10);
}
#endif

#if DEBUG_EU_AUDIO == 1 && DEBUG_AUDIO_LOCALIZATION == 1
void Audio_LoadAquasSequence(void);
void Audio_LoadInstruments(void);
void AudioThread_ScheduleProcessCmds(void);
void Audio_RestoreVolumeSettings(u8 audioType);

extern u8 sAudioSpecId;

RECOMP_PATCH void Audio_RestartSeqPlayers(void) {
    s32 pad1;
    s32 pad2;
    u16 fadeIn = 1;

#ifdef VERSION_EU
    if (gVoiceLanguage == 0) {
        Audio_StartSequence(SEQ_PLAYER_VOICE, NA_BGM_VO, -1, 1);
    } else {
        Audio_StartSequence(SEQ_PLAYER_VOICE, 66 /*NA_BGM_VO_LYLAT*/, -1, 1);
    }
#else
    Audio_StartSequence(SEQ_PLAYER_VOICE, NA_BGM_VO, -1, 1);
#endif

    if (sAudioSpecId == AUDIOSPEC_AQ) {
        fadeIn = 360;
    } else if (sAudioSpecId < AUDIOSPEC_TITLE) {
        fadeIn = 90;
    }
    Audio_StartSequence(SEQ_PLAYER_SFX, NA_BGM_SE, -1, fadeIn);
    Audio_LoadInstruments();
    Audio_LoadAquasSequence();
    SEQCMD_SET_SEQPLAYER_VOLUME(SEQ_PLAYER_SFX, 0, 127);
    SEQCMD_SET_SEQPLAYER_VOLUME(SEQ_PLAYER_VOICE, 0, 127);
    AudioThread_ScheduleProcessCmds();
    AUDIOCMD_GLOBAL_STOP_AUDIOCMDS();
    AUDIOCMD_GLOBAL_STOP_AUDIOCMDS();
    AUDIOCMD_GLOBAL_STOP_AUDIOCMDS();
    Audio_RestoreVolumeSettings(AUDIO_TYPE_MUSIC);
    Audio_RestoreVolumeSettings(AUDIO_TYPE_SFX);
    Audio_RestoreVolumeSettings(AUDIO_TYPE_VOICE);
}
#endif

#if DEBUG_EU_AUDIO == 1 && DEBUG_AUDIO_LOCALIZATION == 1
void AudioHeap_InitSessionPools(AudioSessionPoolSplit* split);
void AudioHeap_InitCachePools(AudioCachePoolSplit* split);
void AudioHeap_InitPersistentPoolsAndCaches(AudioCommonPoolSplit* split);
void AudioHeap_InitTemporaryPoolsAndCaches(AudioCommonPoolSplit* split);
void AudioHeap_InitSampleCaches(u32 persistentSampleCacheSize, u32 temporarySampleCacheSize);
void AudioHeap_ResetLoadStatus(void);

#define osAiSetFrequency osAiSetFrequency_recomp
#define osWritebackDCacheAll osWritebackDCacheAll_recomp
void* AudioHeap_AllocZeroed(AudioAllocPool* pool, u32 size);
void osWritebackDCacheAll(void);
s32 osAiSetFrequency(u32 freq);

RECOMP_PATCH void AudioHeap_Init(void) {
    s32 i;
    s32 j;
    AudioSpec* spec = &((AudioSpec*)gAudioSpecs_localized)[gAudioSpecId];
    ReverbSettings* settings;
    SynthesisReverb* reverb;
    s16* ramAddr;
    u32 persistentSize;
    u32 temporarySize;
    u32 cachePoolSize;
    u32 miscPoolSize;

    gSampleDmaCount = 0;
    gAudioBufferParams.samplingFrequency = spec->samplingFrequency;
    gAudioBufferParams.aiSamplingFrequency = osAiSetFrequency(gAudioBufferParams.samplingFrequency);
    gAudioBufferParams.samplesPerFrameTarget = ALIGN16(gAudioBufferParams.samplingFrequency / gRefreshRate);

    gAudioBufferParams.minAiBufferLength = gAudioBufferParams.samplesPerFrameTarget - 0x10;
    gAudioBufferParams.maxAiBufferLength = gAudioBufferParams.samplesPerFrameTarget + 0x10;

    gAudioBufferParams.ticksPerUpdate = ((gAudioBufferParams.samplesPerFrameTarget + 0x10) / 192) + 1;
    gAudioBufferParams.samplesPerTick =
        (gAudioBufferParams.samplesPerFrameTarget / gAudioBufferParams.ticksPerUpdate) & ~7;
    gAudioBufferParams.samplesPerTickMax = gAudioBufferParams.samplesPerTick + 8;
    gAudioBufferParams.samplesPerTickMin = gAudioBufferParams.samplesPerTick - 8;
    gAudioBufferParams.resampleRate = 32000.0f / (s32) gAudioBufferParams.samplingFrequency;
    gAudioBufferParams.ticksPerUpdateInvScaled = (3.0f / 2560.0f) / gAudioBufferParams.ticksPerUpdate;
    gAudioBufferParams.ticksPerUpdateInv = 1.0f / gAudioBufferParams.ticksPerUpdate;
    gNumNotes = spec->numNotes;
    D_8014C1B0 = spec->unk_14;
    gMaxTempo = (u16) ((gAudioBufferParams.ticksPerUpdate * 2880000.0f / gSeqTicksPerBeat) / gMaxTempoTvTypeFactors);
    gAudioBufferParams.numBuffers = spec->numBuffers;
    gAudioBufferParams.samplesPerFrameTarget *= gAudioBufferParams.numBuffers;
    gAudioBufferParams.maxAiBufferLength *= gAudioBufferParams.numBuffers;
    gAudioBufferParams.minAiBufferLength *= gAudioBufferParams.numBuffers;
    gAudioBufferParams.ticksPerUpdate *= gAudioBufferParams.numBuffers;

    // #ifdef VERSION_US
    if (gAudioBufferParams.numBuffers >= 2) {
        gAudioBufferParams.maxAiBufferLength -= 0x10;
    }
    // #endif

    gMaxAudioCmds = (gNumNotes * 20 * gAudioBufferParams.ticksPerUpdate) + (spec->numReverbs * 32) + 480;
    persistentSize = spec->persistentSeqCacheSize + spec->persistentFontCacheSize +
                     spec->persistentSampleBankCacheSize + spec->persistentSampleCacheSize + 0x10;
    temporarySize = spec->temporarySeqCacheSize + spec->temporaryFontCacheSize + spec->temporarySampleBankCacheSize +
                    spec->temporarySampleCacheSize + 0x10;
    cachePoolSize = persistentSize + temporarySize;
    miscPoolSize = gSessionPool.size - cachePoolSize - 0x100;
    gSessionPoolSplit.miscPoolSize = miscPoolSize;
    gSessionPoolSplit.cachePoolSize = cachePoolSize;
    AudioHeap_InitSessionPools(&gSessionPoolSplit);
    gCachePoolSplit.persistentCommonPoolSize = persistentSize;
    gCachePoolSplit.temporaryCommonPoolSize = temporarySize;
    AudioHeap_InitCachePools(&gCachePoolSplit);
    gPersistentCommonPoolSplit.seqCacheSize = spec->persistentSeqCacheSize;
    gPersistentCommonPoolSplit.fontCacheSize = spec->persistentFontCacheSize;
    gPersistentCommonPoolSplit.sampleBankCacheSize = spec->persistentSampleBankCacheSize;
    AudioHeap_InitPersistentPoolsAndCaches(&gPersistentCommonPoolSplit);
    gTemporaryCommonPoolSplit.seqCacheSize = spec->temporarySeqCacheSize;
    gTemporaryCommonPoolSplit.fontCacheSize = spec->temporaryFontCacheSize;
    gTemporaryCommonPoolSplit.sampleBankCacheSize = spec->temporarySampleBankCacheSize;
    AudioHeap_InitTemporaryPoolsAndCaches(&gTemporaryCommonPoolSplit);
    AudioHeap_InitSampleCaches(spec->persistentSampleCacheSize, spec->temporarySampleCacheSize);
    AudioHeap_ResetLoadStatus();
    gNotes = AudioHeap_AllocZeroed(&gMiscPool, gNumNotes * sizeof(Note));
    Audio_NoteInitAll();
    Audio_InitNoteFreeList();
    gNoteSubsEu = AudioHeap_AllocZeroed(&gMiscPool, gAudioBufferParams.ticksPerUpdate * gNumNotes * sizeof(NoteSubEu));

    for (i = 0; i != 2; i++) {
        gAbiCmdBuffs[i] = AudioHeap_AllocZeroed(&gMiscPool, gMaxAudioCmds * 8);
    }
    for (i = 0; i < ARRAY_COUNT(gSynthReverbs); i++) {
        gSynthReverbs[i].useReverb = 0;
    }

    gNumSynthReverbs = spec->numReverbs;

    for (i = 0; i < gNumSynthReverbs; i++) {
        settings = &spec->reverbSettings[i];
        reverb = &gSynthReverbs[i];
        reverb->downsampleRate = settings->downsampleRate;
        reverb->windowSize = settings->windowSize * 64;
        reverb->decayRatio = settings->decayRatio;
        reverb->leakRtL = settings->leakRtL;
        reverb->leakLtR = settings->leakLtR;
        reverb->useReverb = 8;

        reverb->leftRingBuf = AudioHeap_AllocZeroed(&gMiscPool, reverb->windowSize * 2);
        reverb->rightRingBuf = AudioHeap_AllocZeroed(&gMiscPool, reverb->windowSize * 2);
        reverb->nextRingBufPos = 0;
        reverb->unk_20 = 0;
        reverb->curFrame = 0;

        reverb->bufSizePerChan = reverb->windowSize;
        reverb->framesToIgnore = 2;

        // recomp_printf("reverb->downsampleRate: %d:\n", reverb->downsampleRate);
        // recomp_printf("reverb->windowSize: %d:\n", reverb->windowSize);
        // recomp_printf("reverb->decayRatio: %d:\n", reverb->decayRatio);
        // recomp_printf("reverb->leakRtL: %d:\n", reverb->leakRtL);
        // recomp_printf("reverb->leakLtR: %d:\n", reverb->leakLtR);
        // recomp_printf("reverb->useReverb: %d:\n", reverb->useReverb);
// 
        // recomp_printf("reverb->leftRingBuf: %x:\n", reverb->leftRingBuf);
        // recomp_printf("reverb->rightRingBuf: %x:\n", reverb->rightRingBuf);
        // recomp_printf("reverb->nextRingBufPos: %d:\n", reverb->nextRingBufPos);
        // recomp_printf("reverb->unk_20: %d:\n", reverb->unk_20);
        // recomp_printf("reverb->curFrame: %d:\n", reverb->curFrame);
        // recomp_printf("reverb->bufSizePerChan: %d:\n", reverb->bufSizePerChan);
        // recomp_printf("reverb->framesToIgnore: %d:\n \n", reverb->framesToIgnore);

        if (reverb->downsampleRate != 1) {
            reverb->resampleFlags = 1;
            reverb->unk_0A = (0x8000 / reverb->downsampleRate);
            reverb->unk_30 = AudioHeap_AllocZeroed(&gMiscPool, 0x20);
            reverb->unk_34 = AudioHeap_AllocZeroed(&gMiscPool, 0x20);
            reverb->unk_38 = AudioHeap_AllocZeroed(&gMiscPool, 0x20);
            reverb->unk_3C = AudioHeap_AllocZeroed(&gMiscPool, 0x20);
            for (j = 0; j < gAudioBufferParams.ticksPerUpdate; j++) {
                ramAddr = AudioHeap_AllocZeroed(&gMiscPool, 0x300);
                reverb->items[0][j].toDownsampleLeft = ramAddr;
                reverb->items[0][j].toDownsampleRight = ramAddr + 192;
                ramAddr = AudioHeap_AllocZeroed(&gMiscPool, 0x300);
                reverb->items[1][j].toDownsampleLeft = ramAddr;
                reverb->items[1][j].toDownsampleRight = ramAddr + 192;
            }
        }
    }
    AudioLoad_InitSampleDmaBuffers(gNumNotes);
    gPreloadSampleStackTop = 0;
    D_8014C1B4 = 0x1000;
    osWritebackDCacheAll();
}
#endif

#define osCartRomInit osCartRomInit_recomp
#define osEPiStartDma osEPiStartDma_recomp
OSPiHandle* osCartRomInit(void);
s32 osEPiStartDma(OSPiHandle*, OSIoMesg*, s32);

#if DEBUG_AUDIO_LOCALIZATION == 1
RECOMP_PATCH s32 AudioLoad_Dma(OSIoMesg* mesg, u32 priority, s32 direction, u32 devAddr, void* ramAddr, u32 size,
                               OSMesgQueue* retQueue, s32 medium, const char* dmaType) {
    OSPiHandle* handle;

    /*
        switch (medium) {
            case MEDIUM_CART:
                handle = osCartRomInit();
                break;
            case MEDIUM_DISK_DRIVE:
                handle = osDriveRomInit();
                break;
            default:
                return 0;
        }
    */

    if (size % 16) {
        size = ALIGN16(size);
    }

    /*
    mesg->hdr.pri = priority;
    mesg->hdr.retQueue = retQueue;
    mesg->dramAddr = ramAddr;
    mesg->devAddr = devAddr;
    mesg->size = size;

    handle->transferInfo.cmdType = 2;
    osEPiStartDma(handle, mesg, direction);
    */

    // @recomp: Copy from data to RAM and respond the message.
    memcpy2(ramAddr, (u32*) devAddr, size);
    osSendMesg(retQueue, (OSMesg) 1, OS_MESG_NOBLOCK);

    return 0;
}
#endif

// Fix UB
// Shouldn't be needed, but i'm leaving this patch here just in case.
#if 0
void AudioLoad_RelocateFont(s32 fontId, u32 fontBaseAddr, void* relocData);
void AudioLoad_SyncDmaUnkMedium(u32 devAddr, u8* ramAddr, u32 size, s32 unkMediumParam);
void AudioLoad_SyncDma(u32 devAddr, u8* ramAddr, u32 size, s32 medium);
AudioAsyncLoad* AudioLoad_StartAsyncLoad(u32 devAddr, u8* ramAddr, u32 size, s32 medium, s32 nChunks,
                                         OSMesgQueue* retQueue, u32 retMesg);

extern s32 D_80146D80;
RECOMP_PATCH s32 AudioLoad_RelocateFontAndPreloadSamples(s32 fontId, u32 fontDataAddr, SampleBankRelocInfo* relocData,
                                                         s32 isAsync) {
    s32 i;
    Sample* sample;
    u8* sampleRamAddr;
    s32 size;
    s32 pad;
    u32 nChunks;
    s32 inProgress;

    inProgress = false;
    if (gPreloadSampleStackTop != 0) {
        inProgress = true;
    } else {
        D_80146D80 = 0;
    }

    gNumUsedSamples = 0;
    AudioLoad_RelocateFont(fontId, fontDataAddr, relocData);
    size = 0;

    for (i = 0; i < gNumUsedSamples; i++) {
        size += ALIGN16(gUsedSamples[i]->size);
    }

    for (i = 0; i < gNumUsedSamples; i++) {
        if (gPreloadSampleStackTop == 120) {
            break;
        }
        sample = gUsedSamples[i];

        sampleRamAddr = NULL;

        //! @bug Those are assignments, not equality checks.
        switch (isAsync) {
            case AUDIOLOAD_SYNC:
                if (sample->medium == relocData->medium1) {
                    sampleRamAddr = AudioHeap_AllocPersistentSampleCache(sample->size, relocData->sampleBankId1,
                                                                         (u32) sample->sampleAddr, sample->medium);
                } else if (sample->medium == relocData->medium2) {
                    sampleRamAddr = AudioHeap_AllocPersistentSampleCache(sample->size, relocData->sampleBankId2,
                                                                         (u32) sample->sampleAddr, sample->medium);
                }
                break;

            case AUDIOLOAD_ASYNC:
                if (sample->medium == relocData->medium1) {
                    sampleRamAddr = AudioHeap_AllocTemporarySampleCache(sample->size, relocData->sampleBankId1,
                                                                        (u32) sample->sampleAddr, sample->medium);
                } else if (sample->medium == relocData->medium2) {
                    sampleRamAddr = AudioHeap_AllocTemporarySampleCache(sample->size, relocData->sampleBankId2,
                                                                        (u32) sample->sampleAddr, sample->medium);
                }
                break;
        }

        if (sampleRamAddr == NULL) {
            continue;
        }

        switch (isAsync) {
            case AUDIOLOAD_SYNC:
                if (sample->medium == MEDIUM_UNK) {
                    AudioLoad_SyncDmaUnkMedium((u32) sample->sampleAddr, sampleRamAddr, sample->size,
                                               gSampleBankTable->base.unkMediumParam);
                    sample->sampleAddr = sampleRamAddr;
                    sample->medium = MEDIUM_RAM;
                } else {
                    AudioLoad_SyncDma((u32) sample->sampleAddr, sampleRamAddr, sample->size, sample->medium);
                    sample->sampleAddr = sampleRamAddr;
                    sample->medium = MEDIUM_RAM;
                }
                break;

            case AUDIOLOAD_ASYNC:
                size = gPreloadSampleStackTop;
                gPreloadSampleStack[size].sample = sample;
                gPreloadSampleStack[size].ramAddr = sampleRamAddr;
                gPreloadSampleStack[size].encodedInfo = (size << 24) | 0xFFFFFF;
                gPreloadSampleStack[size].isFree = 0;
                gPreloadSampleStack[size].endAndMediumKey =
                    (uintptr_t) sample->sampleAddr + sample->size + sample->medium;
                gPreloadSampleStackTop++;
                break;
        }
    }

    gNumUsedSamples = 0;

    if ((gPreloadSampleStackTop != 0) && !inProgress) {
        sample = gPreloadSampleStack[gPreloadSampleStackTop - 1].sample;
        nChunks = (sample->size / 0x1000) + 1;
        AudioLoad_StartAsyncLoad((u32) sample->sampleAddr, gPreloadSampleStack[gPreloadSampleStackTop - 1].ramAddr,
                                 sample->size, sample->medium, nChunks, &gPreloadSampleQueue,
                                 gPreloadSampleStack[gPreloadSampleStackTop - 1].encodedInfo);
    }
}
#endif

#if 0
void func_80009AAC(s32 updateIndex);
void func_8001678C(s32 arg0);
Acmd* func_8000A25C(s16* aiBuf, s32 aiBufLen, Acmd* aList, s32 updateIndex);
void func_800080C0(s32 sampleCount, s32 itemIndex, s32 reverbIndex);

RECOMP_PATCH Acmd* func_80009B64(Acmd* aList, s32* cmdCount, s16* aiBufStart, s32 aiBufLen) {
    Acmd* aCmdPtr;
    s16* aiBufPtr;
    s32 chunkLen;
    s32 i;
    s32 j;

    aCmdPtr = aList;
    for (i = gAudioBufferParams.ticksPerUpdate; i > 0; i--) {
        func_8001678C(i - 1);
        func_80009AAC(gAudioBufferParams.ticksPerUpdate - i);
    }

    aiBufPtr = aiBufStart;

    recomp_printf("ticksPerUpdate: %d\n", gAudioBufferParams.ticksPerUpdate);
    for (i = gAudioBufferParams.ticksPerUpdate; i > 0; i--) {
        if (i == 1) {
            chunkLen = aiBufLen;
        } else if ((aiBufLen / i) >= gAudioBufferParams.samplesPerTickMax) {
            chunkLen = gAudioBufferParams.samplesPerTickMax;
        } else if (gAudioBufferParams.samplesPerTickMin >= (aiBufLen / i)) {
            chunkLen = gAudioBufferParams.samplesPerTickMin;
        } else {
            chunkLen = gAudioBufferParams.samplesPerTick;
        }

        for (j = 0; j < gNumSynthReverbs; j++) {
            if (gSynthReverbs[j].useReverb) {
                func_800080C0(chunkLen, gAudioBufferParams.ticksPerUpdate - i, j);
            }
        }
        static int timessynthreverbs = 0;
        if (timessynthreverbs < 10) {
            recomp_printf("chunkLen: %x\n", chunkLen);
        }
        timessynthreverbs++;
        aCmdPtr = func_8000A25C((s16*) aiBufPtr, chunkLen, aCmdPtr, gAudioBufferParams.ticksPerUpdate - i);
        aiBufLen -= chunkLen;
        aiBufPtr += chunkLen * 2;
    }

    for (j = 0; j < gNumSynthReverbs; j++) {
        if (gSynthReverbs[j].framesToIgnore != 0) {
            gSynthReverbs[j].framesToIgnore--;
        }
        gSynthReverbs[j].curFrame ^= 1;
    }

    *cmdCount = aCmdPtr - aList;

    return aCmdPtr;
}
#endif

#if 0
Acmd* func_8000B3F0(Acmd* aList, NoteSubEu* noteSub, NoteSynthesisState* synthState, s32 numSamplesToLoad);
u8* func_800097A8(Sample* sample, s32 length, u32 flags, UnkStruct_800097A8* arg3);
void func_80009A2C(s32 updateIndex, s32 noteIndex);
Acmd* func_8000B480(Acmd* aList, NoteSynthesisState* synthState, s32 size, u16 pitch, u16 inpDmem, u32 resampleFlags);
Acmd* func_8000B51C(Acmd* aList, NoteSubEu* noteSub, NoteSynthesisState* synthState, s32 aiBufLen, u16 dmemSrc,
                    s32 delaySide, s32 flags);
Acmd* func_8000B98C(Acmd* aList, NoteSubEu* noteSub, NoteSynthesisState* synthState, s32 size, s32 flags,
                    s32 delaySide);


RECOMP_PATCH Acmd* func_8000A700(s32 noteIndex, NoteSubEu* noteSub, NoteSynthesisState* synthState, s16* aiBuf, s32 aiBufLen,
                    Acmd* aList, s32 updateIndex) {
    s32 pad11C;
    s32 pad118;
    s32 pad114;
    Sample* bookSample;  // sp110
    AdpcmLoop* loopInfo; // sp10C
    void* currentBook;   // sp108
    s32 pad104;
    s32 pad100;
    s32 noteFinished;           // spFC
    u32 restart;                // spF8
    s32 flags;                  // spF4
    u16 resampleRateFixedPoint; // spF2
    s32 nSamplesToLoad;         // spEC
    s32 padE8;
    s32 padE4;
    s32 padE0;
    s32 skipBytes; // spDC
    s32 padD8;
    s32 padD4;
    s32 padD0;
    u32 sampleAddr; // spCC
    s32 padC8;
    s32 samplesLenAdjusted;     // spC4
    s32 nAdpcmSamplesProcessed; // spC0
    u32 endPos;                 // spBC
    s32 nSamplesToProcess;      // spB8
    s32 padB4;
    s32 padB0;
    s32 padAC;
    s32 padA8;
    s32 padA4;
    s32 nTrailingSamplesToIgnore; // spA0
    s32 pad9C;
    s32 pad98;
    s32 pad94;
    s32 frameSize; // sp90
    s32 pad8C;
    s32 skipInitialSamples; // sp88
    s32 sampleDmaStart;     // sp84
    s32 pad80;
    s32 nParts;  // sp7C
    s32 curPart; // sp78
    s32 nSamplesInThisIteration;
    s32 sampleDataStartPad;
    s32 resampledTempLen;                    // sp6C
    u16 noteSamplesDmemAddrBeforeResampling; // sp6A
    s32 samplesRemaining;
    s32 frameIndex;
    s32 sampleDataOffset;
    Note* note; // sp58
    u16 sp56;   // sp56

    s32 nSamplesInFirstFrame;
    s32 delaySide;
    s32 nSamplesToDecode;
    s32 nFramesToDecode;
    s32 nFirstFrameSamplesToIgnore;
    s32 s5;
    u32 sampleslenFixedPoint;
    u8* sampleData;
    s32 temp;
    s32 temp2;
    u32 nEntries;
    s32 aligned;
    s32 align2;
    u16 addr;

    currentBook = NULL;
    note = &gNotes[noteIndex];
    flags = 0;
    if (noteSub->bitField0.needsInit == 1) {
        flags = 1;
        synthState->restart = 0;
        synthState->samplePosInt = 0;
        synthState->samplePosFrac = 0;
        synthState->curVolLeft = 0;
        synthState->curVolRight = 0;
        synthState->prevHaasEffectLeftDelaySize = 0;
        synthState->prevHaasEffectRightDelaySize = 0;
        synthState->numParts = 0;
        note->noteSubEu.bitField0.finished = 0;
    }
    resampleRateFixedPoint = noteSub->resampleRate;
    nParts = noteSub->bitField1.hasTwoParts + 1;
    sampleslenFixedPoint = (resampleRateFixedPoint * aiBufLen * 2) + synthState->samplePosFrac;
    nSamplesToLoad = (sampleslenFixedPoint) >> 0x10;
    synthState->samplePosFrac = sampleslenFixedPoint & 0xFFFF;
    if ((synthState->numParts == 1) && (nParts == 2)) {
        nSamplesToLoad += 2;
        sp56 = 2;
    } else if ((synthState->numParts == 2) && (nParts == 1)) {
        nSamplesToLoad -= 4;
        sp56 = 4;
    } else {
        sp56 = 0;
    }
    synthState->numParts = nParts;
    if (noteSub->bitField1.isSyntheticWave) {
        aList = func_8000B3F0(aList, noteSub, synthState, nSamplesToLoad);
        noteSamplesDmemAddrBeforeResampling = (synthState->samplePosInt * 2) + 0x5F0;
        synthState->samplePosInt += nSamplesToLoad;
    } else {
        bookSample = *(Sample**) noteSub->waveSampleAddr;
        loopInfo = bookSample->loop;
        endPos = loopInfo->end;
        sampleAddr = (uintptr_t)bookSample->sampleAddr;
        resampledTempLen = 0;

        for (curPart = 0; curPart < nParts; curPart++) {
            nAdpcmSamplesProcessed = 0;
            s5 = 0;
            if (nParts == 1) {
                samplesLenAdjusted = nSamplesToLoad;
            } else if (nSamplesToLoad & 1) {
                samplesLenAdjusted = (nSamplesToLoad & ~1) + (curPart * 2);
            } else {
                samplesLenAdjusted = nSamplesToLoad;
            }
            if ((bookSample->codec == 0) && (currentBook != bookSample->book->book)) {
                switch (noteSub->bitField1.bookOffset) {
                    case 1:
                        currentBook = &gD_800DD200[1];
                        break;
                    case 2:
                        currentBook = &gD_800DD200[2];
                        break;
                    default:
                    case 3:
                        currentBook = bookSample->book->book;
                        break;
                }

                nEntries = 16 * bookSample->book->order * bookSample->book->numPredictors;
                aLoadADPCM(aList++, nEntries, OS_K0_TO_PHYSICAL(currentBook));
            }
            while (nAdpcmSamplesProcessed != samplesLenAdjusted) {
                restart = 0;
                noteFinished = 0;
                samplesRemaining = endPos - synthState->samplePosInt;
                nSamplesToProcess = samplesLenAdjusted - nAdpcmSamplesProcessed;

                nFirstFrameSamplesToIgnore = synthState->samplePosInt & 0xF;

                if ((nFirstFrameSamplesToIgnore == 0) && !synthState->restart) {
                    nFirstFrameSamplesToIgnore = 0x10;
                }

                nSamplesInFirstFrame = 0x10 - nFirstFrameSamplesToIgnore;
                if (nSamplesToProcess < samplesRemaining) {
                    nFramesToDecode = (nSamplesToProcess - nSamplesInFirstFrame + 0xF) / 16;
                    nSamplesToDecode = nFramesToDecode * 0x10;
                    // if(1) {}
                    nTrailingSamplesToIgnore = (nSamplesInFirstFrame + nSamplesToDecode) - nSamplesToProcess;
                } else {
                    nSamplesToDecode = samplesRemaining - nSamplesInFirstFrame;
                    nTrailingSamplesToIgnore = 0;
                    if (nSamplesToDecode <= 0) {
                        nSamplesToDecode = 0;
                        nSamplesInFirstFrame = samplesRemaining;
                    }
                    nFramesToDecode = (nSamplesToDecode + 0xF) / 16;
                    if (loopInfo->count != 0) {
                        restart = 1;
                    } else {
                        noteFinished = 1;
                    }
                }
                switch (bookSample->codec) {
                    case 0:
                        frameSize = 9;
                        skipInitialSamples = 0x10;
                        sampleDmaStart = 0;
                        break;
                    case 1:
                        frameSize = 0x10;
                        skipInitialSamples = 0x10;
                        sampleDmaStart = 0;
                        break;
                    case 2:
                        temp =
                            (uintptr_t)func_800097A8(bookSample, samplesLenAdjusted, flags, &synthState->synthesisBuffers->unk_40);
                        aLoadBuffer(aList++, OS_K0_TO_PHYSICAL(temp), 0x5F0, (samplesLenAdjusted + 0x10) * 2);
                        s5 = samplesLenAdjusted;
                        nAdpcmSamplesProcessed = samplesLenAdjusted;
                        skipBytes = 0;
                        goto skip;
                }
                aligned = ALIGN16(nFramesToDecode * frameSize + 0x10);
                addr = 0x990 - aligned;
                if (nFramesToDecode != 0) {
                    frameIndex = (synthState->samplePosInt + skipInitialSamples - nFirstFrameSamplesToIgnore) / 16;
                    sampleDataOffset = frameIndex * frameSize;
                    if (bookSample->medium == 0) {
                        sampleData = (u8*)sampleDmaStart + sampleDataOffset + sampleAddr;
                    } else {
                        sampleData = AudioLoad_DmaSampleData(sampleDmaStart + sampleDataOffset + sampleAddr, aligned,
                                                             flags, &synthState->sampleDmaIndex, bookSample->medium);
                    }
                    // if (1){}
                    sampleDataStartPad = (u32) sampleData & 0xF;
                    aLoadBuffer(aList++, OS_K0_TO_PHYSICAL(sampleData - sampleDataStartPad), addr, aligned);
                } else {
                    nSamplesToDecode = 0;
                    sampleDataStartPad = 0;
                }
                if (synthState->restart) {
                    aSetLoop(aList++, OS_K0_TO_PHYSICAL(bookSample->loop->predictorState));
                    flags = 2;
                    synthState->restart = 0;
                }
                nSamplesInThisIteration = nSamplesToDecode + nSamplesInFirstFrame - nTrailingSamplesToIgnore;

                if (nAdpcmSamplesProcessed == 0) {
                    switch (bookSample->codec) {
                        case 0:
                            aSetBuffer(aList++, 0, addr + sampleDataStartPad, 0x5F0, nSamplesToDecode * 2);
                            aADPCMdec(aList++, flags, OS_K0_TO_PHYSICAL(synthState->synthesisBuffers));
                            break;
                        case 1:
                            aSetBuffer(aList++, 0, addr + sampleDataStartPad, 0x5F0, nSamplesToDecode * 2);
                            aS8Dec(aList++, flags, OS_K0_TO_PHYSICAL(synthState->synthesisBuffers));
                            break;
                    }

                    skipBytes = nFirstFrameSamplesToIgnore * 2;
                } else {
                    // if (1) {}
                    align2 = ALIGN16(s5 + 0x10);
                    switch (bookSample->codec) {
                        case 0:
                            aSetBuffer(aList++, 0, addr + sampleDataStartPad, align2 + 0x5F0, nSamplesToDecode * 2);
                            aADPCMdec(aList++, flags, OS_K0_TO_PHYSICAL(synthState->synthesisBuffers));
                            break;
                        case 1:
                            aSetBuffer(aList++, 0, addr + sampleDataStartPad, align2 + 0x5F0, nSamplesToDecode * 2);
                            aS8Dec(aList++, flags, OS_K0_TO_PHYSICAL(synthState->synthesisBuffers));
                            break;
                    }
                    aDMEMMove(aList++, (align2 + (nFirstFrameSamplesToIgnore * 2) + 0x5F0), s5 + 0x5F0,
                              nSamplesInThisIteration * 2);
                }
                nAdpcmSamplesProcessed += nSamplesInThisIteration;
                switch (flags) {
                    case 1:
                        skipBytes = 0x20;
                        s5 = (nSamplesToDecode + 0x10) * 2;
                        break;
                    case 2:
                        // if(1) {}
                        s5 = nSamplesInThisIteration * 2 + s5;
                        break;
                    default:
                        if (s5 != 0) {
                            s5 = nSamplesInThisIteration * 2 + s5;
                        } else {
                            // if (1) { }
                            s5 = (nFirstFrameSamplesToIgnore + nSamplesInThisIteration) * 2;
                        }
                        break;
                }
            skip:
                flags = 0;
                // goto dummy_label_147574; dummy_label_147574: ;
                if (noteFinished) {
                    aClearBuffer(aList++, s5 + 0x5F0, (samplesLenAdjusted - nAdpcmSamplesProcessed) * 2);
                    noteSub->bitField0.finished = 1;
                    note->noteSubEu.bitField0.finished = 1;
                    func_80009A2C(updateIndex, noteIndex);
                    break;
                }
                if (restart) {
                    synthState->restart = 1;
                    synthState->samplePosInt = loopInfo->start;
                } else {
                    synthState->samplePosInt += nSamplesToProcess;
                }
            }
            switch (nParts) {
                case 1:
                    noteSamplesDmemAddrBeforeResampling = skipBytes + 0x5F0;
                    break;
                case 2:
                    switch (curPart) {
                        case 0:
                            aInterl(aList++, skipBytes + 0x5F0, 0x470, ALIGN8(samplesLenAdjusted / 2));
                            resampledTempLen = samplesLenAdjusted;
                            noteSamplesDmemAddrBeforeResampling = 0x470;
                            if (noteSub->bitField0.finished) {
                                aClearBuffer(aList++, resampledTempLen + noteSamplesDmemAddrBeforeResampling,
                                             samplesLenAdjusted + 0x10);
                            }
                            break;
                        case 1:
                            aInterl(aList++, skipBytes + 0x5F0, resampledTempLen + 0x470,
                                    ALIGN8(samplesLenAdjusted / 2));
                            break;
                    }
                    break;
            }

            if (noteSub->bitField0.finished) {
                break;
            }
        }
    }

    flags = 0;
    if (noteSub->bitField0.needsInit == 1) {
        flags = 1;
        noteSub->bitField0.needsInit = 0;
    }
    flags = sp56 | flags;
    aList = func_8000B480(aList, synthState, aiBufLen * 2, resampleRateFixedPoint, noteSamplesDmemAddrBeforeResampling,
                          flags);
    if (flags & 1) {
        flags = 1;
    }
    if (noteSub->bitField1.bookOffset == 3) {
        aUnkCmd19(aList++, 0, aiBufLen * 2, 0x450, 0x450);
    }

    temp2 = noteSub->gain;
    if (temp2 != 0) {
        if (temp2 < 0x10) {
            temp2 = 0x10;
        }
        aHiLoGain(aList++, temp2, (aiBufLen + 0x10) * 2, 0x450, 0);
    }
    if ((noteSub->leftDelaySize != 0) || (synthState->prevHaasEffectLeftDelaySize != 0)) {
        delaySide = 1;
    } else if ((noteSub->rightDelaySize != 0) || (synthState->prevHaasEffectRightDelaySize != 0)) {
        delaySide = 2;
    } else {
        delaySide = 0;
    }

    aList = func_8000B51C(aList, noteSub, synthState, aiBufLen, 0x450, delaySide, flags);
    // if(restart) {}
    if (noteSub->bitField0.usesHeadsetPanEffects) {
        if (!(flags & 1)) {
            flags = 0;
        }
        aList = func_8000B98C(aList, noteSub, synthState, aiBufLen * 2, flags, delaySide);
    }

    return aList;
}
#endif

#if 0
typedef enum OverlayCalls {
    /*  90 */ OVLCALL_FO_CS_COMPLETE = 90,
    /*  91 */ OVLCALL_BO_BASE_UPDATE,
    /*  92 */ OVLCALL_BO_BASE_DRAW,
    /*  93 */ OVLCALL_BO_BASE_SHIELD_UPDATE,
    /*  94 */ OVLCALL_BO_BASE_SHIELD_DRAW,
    /*  95 */ OVLCALL_BO_SHIELD_REACTOR_UPDATE,
    /*  96 */ OVLCALL_BO_SHIELD_REACTOR_DRAW,
    /*  97 */ OVLCALL_BO_LASER_CANNON_UPDATE,
    /*  98 */ OVLCALL_BO_LASER_CANNON_DRAW,
    /* 103 */ OVLCALL_TITLE_UPDATE = 103,
    /* 104 */ OVLCALL_TITLE_DRAW,
    /* 105 */ OVLCALL_MAP_UPDATE,
    /* 106 */ OVLCALL_MAP_DRAW,
    /* 107 */ OVLCALL_OPTION_UPDATE,
    /* 108 */ OVLCALL_OPTION_DRAW,
    /* 109 */ OVLCALL_GAME_OVER_UPDATE,
    /* 110 */ OVLCALL_UNKMAP_DRAW,
} OverlayCalls;

void Game_SetGameState(void);
void Game_InitStandbyDL(Gfx** dList);
void Game_InitMasterDL(Gfx **dList);
void Game_SetScene(void);
bool Game_ChangeScene(void);
void Game_InitViewport(Gfx **dList, u8 camCount, u8 camIndex);
void Game_Draw(s32 playerNum);
extern u8 aNintendoLogoTex[];
extern s32 sVsCameraULx[];
extern s32 sVsCameraLRx[];
extern s32 sVsCameraULy[];
extern s32 sVsCameraLRy[];

#define uint64_t u64

// uint64_t __udivdi3(uint64_t n_hi, uint64_t n_lo, uint64_t d) {
//     if (d == 0) {
//         // Division by zero; handle as appropriate.
//         return 0;
//     }
// 
//     uint64_t quotient = 0;
//     uint64_t remainder = n_hi; // Start remainder with high 64-bits of dividend
// 
//     for (int i = 63; i >= 0; i--) {
//         // Shift remainder and bring down the next bit from the low part of dividend
//         remainder = (remainder << 1) | ((n_lo >> i) & 1);
// 
//         if (remainder >= d) {
//             remainder -= d;
//             quotient |= (uint64_t)1 << i;
//         }
//     }
// 
//     return quotient;
// }

RECOMP_PATCH void Game_Update(void) {
    s32 i;
    u8 partialFill;
    u8 soundMode;

    Game_SetGameState();

    if (gGameStandby) {
        Game_InitStandbyDL(&gUnkDisp1);
        gGameStandby = false;
        return;
    }

    Game_InitMasterDL(&gUnkDisp1);
    Game_SetScene();

    if (Game_ChangeScene() != true) {
        Lib_InitPerspective(&gUnkDisp1);
        Game_InitViewport(&gUnkDisp1, gCamCount, 0);

        if (gNextGameStateTimer != 0) {
            gNextGameStateTimer--;
        }

        switch (gGameState) {
            case GSTATE_BOOT:
                gNextGameStateTimer = 2;
                gGameState++;
                break;

            case GSTATE_BOOT_WAIT:
                if (gNextGameStateTimer == 0) {
                    gGameState++;
                }
                break;

            case GSTATE_SHOW_LOGO:
                RCP_SetupDL(&gMasterDisp, SETUPDL_76);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 0], 128, 16, 100.0f, 86.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 1], 128, 16, 100.0f, 102.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 2], 128, 16, 100.0f, 118.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 3], 128, 16, 100.0f, 134.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 4], 128, 10, 100.0f, 150.0f, 1.0f, 1.0f);
                gGameState++;
                break;

            case GSTATE_CHECK_SAVE:
                if (Save_Read() != 0) {
// #ifdef AVOID_UB
//                     gSaveFile.save = gDefaultSave;
//                     gSaveFile.backup = gDefaultSave;
// #else
//                     gSaveFile = *((SaveFile*) &gDefaultSave);
// #endif
                    memcpy2(&gSaveFile, &gDefaultSave, sizeof(SaveFile));
                    Save_Write();
                }
                gGameState++;
                Timer_CreateTask(((((u64)((1000) * 1000LL)*(OS_CLOCK_RATE/15625LL))/(1000000LL/15625LL))), Timer_Increment, (s32*) &gGameState, 1);
                /* fallthrough */
            case GSTATE_LOGO_WAIT:
                RCP_SetupDL(&gMasterDisp, SETUPDL_76);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 0], 128, 16, 100.0f, 86.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 1], 128, 16, 100.0f, 102.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 2], 128, 16, 100.0f, 118.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 3], 128, 16, 100.0f, 134.0f, 1.0f, 1.0f);
                Lib_TextureRect_IA8(&gMasterDisp, &aNintendoLogoTex[128 * 16 * 4], 128, 10, 100.0f, 150.0f, 1.0f, 1.0f);
                break;

            case GSTATE_START:
                gGameState = GSTATE_INIT;
                gSceneId = SCENE_TITLE;
                gSceneSetup = 0;
                break;

            case GSTATE_INIT:
                gGameState = GSTATE_TITLE;
                gTitleState = 1;
                gClearPlayerInfo = true;
                Memory_FreeAll();
                Play_ClearObjectData();
                gCamCount = 1;
                gLifeCount[0] = 2;
                gPathProgress = 0.0f;
                D_hud_8016170C = gCsFrameCount = gShowLevelClearStatusScreen = gLevelStartStatusScreenTimer =
                    gLevelClearScreenTimer = gVsMatchState = gVersusMode = gTitleState = gStarCount = gMapState =
                        gPlayState = gOptionMenuStatus = gDrawMode = gShowBossHealth = gShowHud = gBgColor =
                            gFillScreenAlpha = 0;
                gNextGameState = D_ctx_80177C94 = D_ctx_80177CAC = D_ctx_80177CB4 = D_ctx_80177CBC = D_ctx_80177CC4 =
                    D_ctx_80177C9C = D_ctx_80177CA4 = D_play_80161A5C = gLastGameState = GSTATE_NONE;
#ifdef MODS_BOOT_STATE
                gNextGameState = MODS_BOOT_STATE;
#endif
                for (i = 0; i < 4; i++) {
                    gBoostButton[i] = L_CBUTTONS;
                    gBrakeButton[i] = D_CBUTTONS;
                    gShootButton[i] = A_BUTTON;
                    gBombButton[i] = B_BUTTON;

                    D_ctx_80177958[i] = 1.0f;

                    D_ctx_80177858[i] = 3;
                    gPlayerForms[i] = FORM_ARWING;
                    gShowReticles[i] = true;
                    gPlayerGlareAlphas[i] = 0;
                }

                gVersusStage = 0;
                gVsPointsToWin = 3;
                gBlurAlpha = 255;

                for (i = 0; i < ARRAY_COUNT(gLeveLClearStatus); i++) {
                    gLeveLClearStatus[i] = 0;
                }

                gExpertMode = false;
                gOptionSoundMode = gSaveFile.save.data.soundMode;

                switch (gOptionSoundMode) {
                    case OPTIONSOUND_STEREO:
                        soundMode = SOUNDMODE_STEREO;
                        break;
                    case OPTIONSOUND_MONO:
                        soundMode = SOUNDMODE_MONO;
                        break;
                    case OPTIONSOUND_HEADSET:
                        soundMode = SOUNDMODE_HEADSET;
                        break;
                    default:
                        gOptionSoundMode = OPTIONSOUND_STEREO;
                        soundMode = SOUNDMODE_STEREO;
                        break;
                }

                SEQCMD_SET_SOUND_MODE(soundMode);
                gVolumeSettings[AUDIO_TYPE_MUSIC] = gSaveFile.save.data.musicVolume;
                gVolumeSettings[AUDIO_TYPE_VOICE] = gSaveFile.save.data.voiceVolume;
                gVolumeSettings[AUDIO_TYPE_SFX] = gSaveFile.save.data.sfxVolume;

                if (gVolumeSettings[AUDIO_TYPE_MUSIC] > 99) {
                    gVolumeSettings[AUDIO_TYPE_MUSIC] = 99;
                }
                if (gVolumeSettings[AUDIO_TYPE_VOICE] > 99) {
                    gVolumeSettings[AUDIO_TYPE_VOICE] = 99;
                }
                if (gVolumeSettings[AUDIO_TYPE_SFX] > 99) {
                    gVolumeSettings[AUDIO_TYPE_SFX] = 99;
                }
                Audio_SetVolume(AUDIO_TYPE_MUSIC, gVolumeSettings[AUDIO_TYPE_MUSIC]);
                Audio_SetVolume(AUDIO_TYPE_VOICE, gVolumeSettings[AUDIO_TYPE_VOICE]);
                Audio_SetVolume(AUDIO_TYPE_SFX, gVolumeSettings[AUDIO_TYPE_SFX]);
                break;

            case GSTATE_TITLE:
                OvlMenu_CallFunction(OVLCALL_TITLE_UPDATE, NULL);
                break;
            case GSTATE_MENU:
                OvlMenu_CallFunction(OVLCALL_OPTION_UPDATE, NULL);
                break;
            case GSTATE_MAP:
                Map_Main();
                break;
            case GSTATE_VS_INIT:
                Versus_StartMatch();
                break;
            case GSTATE_PLAY:
                Play_Main();
                break;
            case GSTATE_GAME_OVER:
                OvlMenu_CallFunction(OVLCALL_GAME_OVER_UPDATE, NULL);
                break;
            case GSTATE_ENDING:
                gDrawMode = DRAW_ENDING;
                Ending_Main();
                break;
            default:
                break;
        }

        Game_Draw(0);

        if (gCamCount == 2) {
            Game_InitViewport(&gMasterDisp, gCamCount, 1);
            Game_Draw(1);

            gDPPipeSync(gMasterDisp++);
            gDPSetScissor(gMasterDisp++, G_SC_NON_INTERLACE, SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_WIDTH - SCREEN_MARGIN,
                          SCREEN_HEIGHT - SCREEN_MARGIN);
        } else if ((gCamCount == 4) && (gDrawMode != DRAW_NONE)) {
            Game_InitViewport(&gMasterDisp, gCamCount, 3);
            Game_Draw(3);

            Game_InitViewport(&gMasterDisp, gCamCount, 2);
            Game_Draw(2);

            Game_InitViewport(&gMasterDisp, gCamCount, 1);
            Game_Draw(1);

            gDPPipeSync(gMasterDisp++);
            gDPSetScissor(gMasterDisp++, G_SC_NON_INTERLACE, SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_WIDTH - SCREEN_MARGIN,
                          SCREEN_HEIGHT - SCREEN_MARGIN);
            gDPSetColorDither(gMasterDisp++, G_CD_NOISE);
            gDPSetAlphaDither(gMasterDisp++, G_AD_NOISE);
            gDPSetCycleType(gMasterDisp++, G_CYC_1CYCLE);
            gDPSetCombineMode(gMasterDisp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
            gDPSetRenderMode(gMasterDisp++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 0, 0, 0, 0);
            gDPFillRectangle(gMasterDisp++, SCREEN_WIDTH / 2 - 2 - 1, SCREEN_MARGIN, SCREEN_WIDTH / 2 + 2,
                             SCREEN_HEIGHT - SCREEN_MARGIN);
            gDPFillRectangle(gMasterDisp++, SCREEN_MARGIN, SCREEN_HEIGHT / 2 - 2 - 1, SCREEN_WIDTH - SCREEN_MARGIN,
                             SCREEN_HEIGHT / 2 + 2);

            if (gLevelType == LEVELTYPE_PLANET) {
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 0, 0, 0, 255);
            } else {
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 100, 100, 255, 255);
            }

            gDPFillRectangle(gMasterDisp++, SCREEN_WIDTH / 2 - 1 - 1, SCREEN_MARGIN, SCREEN_WIDTH / 2 + 1,
                             SCREEN_HEIGHT - SCREEN_MARGIN);
            gDPFillRectangle(gMasterDisp++, SCREEN_MARGIN, SCREEN_HEIGHT / 2 - 1 - 1, SCREEN_WIDTH - SCREEN_MARGIN,
                             SCREEN_HEIGHT / 2 + 1);

            HUD_8008CB8C();
        }

        partialFill = false;

        if (gCamCount == 1) {
            Graphics_FillRectangle(&gMasterDisp, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, gPlayerGlareReds[0],
                                   gPlayerGlareGreens[0], gPlayerGlareBlues[0], gPlayerGlareAlphas[0]);
            if ((gDrawMode == DRAW_PLAY) || (gDrawMode == DRAW_ENDING)) {
                Radio_Draw();
                if (gShowHud) {
                    HUD_Draw();
                    HUD_EdgeArrows_Update();
                }
                HUD_DrawBossHealth();
            }
        } else {
            for (i = 0; i < gCamCount; i++) {
                if (gPlayer[i].dmgEffectTimer != 0) {

                    Graphics_FillRectangle(&gMasterDisp, sVsCameraULx[i], sVsCameraULy[i], sVsCameraLRx[i],
                                           sVsCameraLRy[i], gFillScreenRed, gFillScreenGreen, gFillScreenBlue,
                                           gFillScreenAlpha);
                    partialFill = true;
                } else {
                    Graphics_FillRectangle(&gMasterDisp, sVsCameraULx[i], sVsCameraULy[i], sVsCameraLRx[i],
                                           sVsCameraLRy[i], gPlayerGlareReds[i], gPlayerGlareGreens[i],
                                           gPlayerGlareBlues[i], gPlayerGlareAlphas[i]);
                }
            }
        }

        Background_dummy_80040CDC();
        HUD_DrawStatusScreens();
        AllRange_DrawCountdown();

        if ((gGameState == GSTATE_PLAY) && gVersusMode) {
            Versus_Draw();
        }

        Wipe_Draw(WIPE_CIRCULAR, gCircleWipeFrame);

        if (!partialFill) {
            Graphics_FillRectangle(&gMasterDisp, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, gFillScreenRed,
                                   gFillScreenGreen, gFillScreenBlue, gFillScreenAlpha);
        }
        Audio_dummy_80016A50();
#if MODS_RAM_MOD == 1
        RamMod_Update();
#endif
#if MODS_FPS_COUNTER == 1
        Play_RenderFps();
#endif
#if MODS_SPAWNER == 1
        Spawner();
#endif
    }
}
#endif

#if 0
RECOMP_PATCH Acmd* func_800098DC(Acmd* aList, u16 dmem, u16 startPos, s32 size, s32 reverbIndex) {
    recomp_printf("LOAD gSynthReverbs[reverbIndex].leftRingBuf[startPos]: %08x\n dmem; %x\n size: %d\n",
                  OS_K0_TO_PHYSICAL(&gSynthReverbs[reverbIndex].leftRingBuf[startPos]), dmem, size);

    aLoadBuffer(aList++, OS_K0_TO_PHYSICAL(&gSynthReverbs[reverbIndex].leftRingBuf[startPos]), dmem, size);
    aLoadBuffer(aList++, OS_K0_TO_PHYSICAL(&gSynthReverbs[reverbIndex].rightRingBuf[startPos]), dmem + DMEM_1CH_SIZE,
                size);
    return aList;
}
#endif

#if 0
RECOMP_PATCH Acmd* func_80009984(Acmd* aList, u16 dmem, u16 startPos, s32 size, s32 reverbIndex) {
    recomp_printf("SAVE gSynthReverbs[reverbIndex].leftRingBuf[startPos]: %08x\n dmem; %x\n size: %d\n",
                  OS_K0_TO_PHYSICAL(&gSynthReverbs[reverbIndex].leftRingBuf[startPos]), dmem, size);

    aSaveBuffer(aList++, dmem, OS_K0_TO_PHYSICAL(&gSynthReverbs[reverbIndex].leftRingBuf[startPos]), size);
    aSaveBuffer(aList++, dmem + DMEM_1CH_SIZE, OS_K0_TO_PHYSICAL(&gSynthReverbs[reverbIndex].rightRingBuf[startPos]),
                size);
    return aList;
}
#endif

#if 0
// func_80009D78
#define DMEM_COMPRESSED_ADPCM_DATA 0x990
#define DMEM_LEFT_CH 0x990
#define DMEM_RIGHT_CH 0xB10
#define DMEM_HAAS_TEMP 0x650
#define DMEM_TEMP 0x450
#define DMEM_UNCOMPRESSED_NOTE 0x5F0
#define DMEM_WET_LEFT_CH 0xC90
#define DMEM_WET_RIGHT_CH 0xE10 // = DMEM_WET_LEFT_CH + DMEM_1CH_SIZE
#define SAMPLE_SIZE sizeof(s16)

// AudioSynth_LoadRingBufferPart
Acmd* func_800098DC(Acmd* aList, u16 dmem, u16 startPos, s32 size, s32 reverbIndex);

RECOMP_PATCH Acmd* func_80009D78(Acmd* aList, s32 aiBufLen, s16 reverbIndex, s16 updateIndex) {
    ReverbRingBufferItem* sp64 = &gSynthReverbs[reverbIndex].items[gSynthReverbs[reverbIndex].curFrame][updateIndex];
    s16 sp62;
    s16 sp60;
    static int segaCounter = 0;

    aClearBuffer(aList++, DMEM_WET_LEFT_CH, DMEM_2CH_SIZE);

    if (gSynthReverbs[reverbIndex].downsampleRate == 1) {
        if (segaCounter < 10) {
            // recomp_printf("sp64->startPos: %x\n sp64->lengthA: %x\n", sp64->startPos, sp64->lengthA);
        }
        segaCounter++;
        aList = func_800098DC(aList, DMEM_WET_LEFT_CH, sp64->startPos, sp64->lengthA, reverbIndex);
        if (sp64->lengthB != 0) {
            aList = func_800098DC(aList, sp64->lengthA + DMEM_WET_LEFT_CH, 0, sp64->lengthB, reverbIndex);
        }
        aAddMixer(aList++, 0x300, DMEM_WET_LEFT_CH, DMEM_LEFT_CH, 0);
        aMix(aList++, 0x30, gSynthReverbs[reverbIndex].decayRatio + 0x8000, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH);
    } else {
        sp62 = (sp64->startPos & 7) * 2;
        sp60 = ALIGN16(sp62 + sp64->lengthA);
        aList = func_800098DC(aList, 0x470, sp64->startPos - (sp62 / 2), DMEM_1CH_SIZE, reverbIndex);
        if (sp64->lengthB != 0) {
            aList = func_800098DC(aList, sp60 + 0x470, 0, DMEM_1CH_SIZE - sp60, reverbIndex);
        }
        aSetBuffer(aList++, 0, sp62 + 0x470, DMEM_WET_LEFT_CH, aiBufLen * 2);
        aResample(aList++, gSynthReverbs[reverbIndex].resampleFlags, gSynthReverbs[reverbIndex].unk_0A,
                  OS_K0_TO_PHYSICAL(gSynthReverbs[reverbIndex].unk_30));
        aSetBuffer(aList++, 0, sp62 + DMEM_UNCOMPRESSED_NOTE, DMEM_WET_RIGHT_CH, aiBufLen * 2);
        aResample(aList++, gSynthReverbs[reverbIndex].resampleFlags, gSynthReverbs[reverbIndex].unk_0A,
                  OS_K0_TO_PHYSICAL(gSynthReverbs[reverbIndex].unk_34));
        aAddMixer(aList++, 0x300, DMEM_WET_LEFT_CH, DMEM_LEFT_CH, 0);
        aMix(aList++, 0x30, gSynthReverbs[reverbIndex].decayRatio + 0x8000, DMEM_WET_LEFT_CH, DMEM_WET_LEFT_CH);
    }

    if ((gSynthReverbs[reverbIndex].leakRtL != 0) || (gSynthReverbs[reverbIndex].leakLtR != 0)) {
        aDMEMMove(aList++, DMEM_WET_LEFT_CH, 0x470, DMEM_1CH_SIZE);
        aMix(aList++, DMEM_1CH_SIZE >> 4, gSynthReverbs[reverbIndex].leakRtL, DMEM_WET_RIGHT_CH, DMEM_WET_LEFT_CH);
        aMix(aList++, DMEM_1CH_SIZE >> 4, gSynthReverbs[reverbIndex].leakLtR, 0x470, DMEM_WET_RIGHT_CH);
    }
    return aList;
}
#endif

#if 0
void AudioSynth_InitNextRingBuf(s32 sampleCount, s32 itemIndex, s32 reverbIndex);
Acmd* AudioSynth_DoOneAudioUpdate(s16* aiBuf, s32 aiBufLen, Acmd* aList, s32 updateIndex);
void AudioSynth_SyncSampleStates(s32 updateIndex);
void func_8001678C(s32 arg0);

RECOMP_PATCH Acmd* func_80009B64(Acmd* aList, s32* cmdCount, s16* aiBufStart, s32 aiBufLen) {
    Acmd* aCmdPtr;
    s16* aiBufPtr;
    s32 chunkLen;
    s32 i;
    s32 j;

    aCmdPtr = aList;
    for (i = gAudioBufferParams.ticksPerUpdate; i > 0; i--) {
        func_8001678C(i - 1);
        AudioSynth_SyncSampleStates(gAudioBufferParams.ticksPerUpdate - i);
    }

    aiBufPtr = aiBufStart;
    for (i = gAudioBufferParams.ticksPerUpdate; i > 0; i--) {
        if (i == 1) {
            chunkLen = aiBufLen;
        } else if ((aiBufLen / i) >= gAudioBufferParams.samplesPerTickMax) {
            chunkLen = gAudioBufferParams.samplesPerTickMax;
        } else if (gAudioBufferParams.samplesPerTickMin >= (aiBufLen / i)) {
            chunkLen = gAudioBufferParams.samplesPerTickMin;
        } else {
            chunkLen = gAudioBufferParams.samplesPerTick;
        }

        for (j = 0; j < gNumSynthReverbs; j++) {
            if (gSynthReverbs[j].useReverb) {
                AudioSynth_InitNextRingBuf(chunkLen, gAudioBufferParams.ticksPerUpdate - i, j);
            }
        }
            static int timessynthreverbs = 0;
        if (timessynthreverbs < 10) {
            recomp_printf("chunkLen: %x\n", chunkLen);
        }
        timessynthreverbs++;

        aCmdPtr =
            AudioSynth_DoOneAudioUpdate((s16*) aiBufPtr, chunkLen, aCmdPtr, gAudioBufferParams.ticksPerUpdate - i);
        aiBufLen -= chunkLen;
        aiBufPtr += chunkLen * 2;
    }

    for (j = 0; j < gNumSynthReverbs; j++) {
        if (gSynthReverbs[j].framesToIgnore != 0) {
            gSynthReverbs[j].framesToIgnore--;
        }
        gSynthReverbs[j].curFrame ^= 1;
    }

    *cmdCount = aCmdPtr -n aList;

    return aCmdPtr;
}
#endif

/*
RECOMP_PATCH f32 func_80013708(Portamento* portamento) {
    u32 temp;
    f32 temp2;

    portamento->cur += portamento->speed;
    temp = portamento->cur;
    if (temp > 127) {
        temp = 127;
    }
    temp2 = 1.0f + ((gBendPitchOneOctaveFrequencies[128 + temp] - 1.0f) * portamento->extent);
    recomp_printf("temp2: %f\n", temp2);
    return temp2;
}
*/
AudioTable *AudioLoad_GetLoadTable(s32 tableType);void *AudioLoad_SearchCaches(s32 tableType, s32 id);void AudioLoad_SyncDma(u32 devAddr, u8 *ramAddr, u32 size, s32 medium);
void AudioLoad_SyncDmaUnkMedium(u32 devAddr, u8 *ramAddr, u32 size, s32 diskParam);
void* AudioLoad_SyncLoad(u32 tableType, u32 id, s32* didAllocate);s32 AudioLoad_GetLoadTableIndex(s32 tableType, u32 entryId);
Instrument *AudioPlayback_GetInstrumentInner(s32, s32);void Audio_SetSequenceFade(u8 seqPlayId, u8 fadeModId, u8 fadeMod, u8 fadeTime);
s32 AudioLoad_SyncLoadSample(Sample *sample, s32 fontId);Sample *AudioLoad_GetFontSample(s32 fontId, s32 instId);
extern AudioSlowLoadBuffer gSlowLoads;extern u8 sSetNextVoiceId;extern u8 sMuteBgmForVoice;extern u32 sNextVoiceId;
#define MEDIUM_DISK 1

#if 0
RECOMP_PATCH void AudioLoad_InitTable(AudioTable* table, u8* romAddr, u16 unkMediumParam) {
    s32 i;

    table->base.unkMediumParam = unkMediumParam;
    table->base.romAddr = (uintptr_t) romAddr;

    recomp_printf("Table(addr): %x\n", (u32)table);
    recomp_printf("Entries(dec): %d\n", table->base.numEntries);

    for (i = 0; i < table->base.numEntries; i++) {
        if ((table->entries[i].size != 0) && (table->entries[i].medium == MEDIUM_CART)) {
            table->entries[i].romAddr += (uintptr_t) romAddr;
            recomp_printf("table->entries[i].romAddr: %x \n", table->entries[i].romAddr);
        }
    }
}
#endif

#if 0
RECOMP_PATCH
void Audio_UpdateVoice(void) {
    s8 voiceBank;
    u16 voiceId;
    u8 voiceIdHi;
    u8 voiceIdLo;

    if (sSetNextVoiceId) {
        voiceBank = sNextVoiceId / 1000;
        voiceId = sNextVoiceId % 1000;
        voiceIdHi = voiceId / 256;
        voiceIdLo = voiceId % 256;
        recomp_printf("Audio_UpdateVoice();\n");
        // recomp_printf("%x\n", ((((AUDIOCMD_OP_CHANNEL_SET_IO) &0xFF) << 0x18) | ((((SEQ_PLAYER_VOICE)) &0xFF) << 0x10) | ((((15)) &0xFF) << 0x8) | ((((0)) &0xFF) << 0)), (1));
        // 0x6030f00
        AUDIOCMD_CHANNEL_SET_IO(SEQ_PLAYER_VOICE, 15, 0, 1);
        // 0x6030f04
        AUDIOCMD_CHANNEL_SET_IO(SEQ_PLAYER_VOICE, 15, 4, voiceBank);
        recomp_printf("voiceBank: %d ", voiceBank);

        recomp_printf("voiceId: %d ", voiceId);

        // 0x6030f05
        AUDIOCMD_CHANNEL_SET_IO(SEQ_PLAYER_VOICE, 15, 5, voiceIdHi);
        recomp_printf("voiceIdHi: %d ", voiceIdHi);
        // 0x6030f06
        AUDIOCMD_CHANNEL_SET_IO(SEQ_PLAYER_VOICE, 15, 6, voiceIdLo);
        recomp_printf("voiceIdLo: %d\n", voiceIdLo);
        sSetNextVoiceId = false;
    } else if ((sMuteBgmForVoice) && (Audio_GetCurrentVoice() == 0)) {
        Audio_SetSequenceFade(SEQ_PLAYER_BGM, 2, 127, 15);
        sMuteBgmForVoice = false;
    }
}
#endif
#if 0
RECOMP_PATCH
// Original name: VoiceLoad
s32 AudioLoad_SlowLoadSample(s32 fontId, u8 instId, s8* status) {
    Sample* sample;
    AudioSlowLoad* slowLoad;

    recomp_printf("fontId: %d, instId: %d, status: %d\n", fontId, instId, *status);

    sample = AudioLoad_GetFontSample(fontId, instId);
    if (sample == NULL) {
        *status = SLOW_LOAD_STATUS_0;
        return -1;
    }
    if (sample->medium == MEDIUM_RAM) {
        *status = SLOW_LOAD_STATUS_2;
        return 0;
    }

    slowLoad = &gSlowLoads.slowLoad[gSlowLoads.unk_00];
    if (slowLoad->state == SLOW_LOAD_DONE) {
        slowLoad->state = SLOW_LOAD_WAITING;
    }

    slowLoad->sample = *sample;
    slowLoad->status = status;

    slowLoad->curRamAddr =
        AudioHeap_AllocTemporarySampleCache(sample->size, fontId, (s32)sample->sampleAddr, sample->medium);

    if (slowLoad->curRamAddr == NULL) {
        if ((sample->medium == MEDIUM_DISK) || (sample->codec == 2)) {
            *status = SLOW_LOAD_STATUS_0;
            return -1;
        } else {
            *status = SLOW_LOAD_STATUS_3;
            return -1;
        }
    }

    slowLoad->state = SLOW_LOAD_START;
    slowLoad->bytesRemaining = ALIGN16(sample->size);
    slowLoad->ramAddr = slowLoad->curRamAddr;
    slowLoad->curDevAddr = (u32)sample->sampleAddr;
    slowLoad->medium = sample->medium;
    slowLoad->seqOrFontId = fontId;
    slowLoad->instId = instId;

    gSlowLoads.unk_00 ^= 1;

    return 0;
}
#endif

#if 0
RECOMP_PATCH s32 AudioLoad_SyncLoadInstrument(s32 fontId, s32 instId, s32 drumId) {
    Instrument* instrument;
    Drum* drum;

    recomp_printf("fontId: %d, instId: %d, drumId: %d\n", fontId, instId, drumId);

    if (instId < 0x7F) {
        instrument = Audio_GetInstrument(fontId, instId);
        if (instrument == NULL) {
            return -1;
        }
        if (instrument->normalRangeLo != 0) {
            AudioLoad_SyncLoadSample(instrument->lowPitchTunedSample.sample, fontId);
        }
        AudioLoad_SyncLoadSample(instrument->normalPitchTunedSample.sample, fontId);
        if (instrument->normalRangeHi != 0x7F) {
            AudioLoad_SyncLoadSample(instrument->highPitchTunedSample.sample, fontId);
        }
    } else if (instId == 0x7F) {
        drum = Audio_GetDrum(fontId, drumId);
        if (drum == NULL) {
            return -1;
        }
        AudioLoad_SyncLoadSample(drum->tunedSample.sample, fontId);
        return 0;
    }
}
#endif

#if 0
RECOMP_PATCH 
void* AudioLoad_SyncLoadSampleBank(u32 sampleBankId, s32* outMedium) {
    void* ramAddr;
    AudioTable* sampleBankTable = AudioLoad_GetLoadTable(2);
    s32 cachePolicy;
    s32 noLoad;

    recomp_printf("AudioLoad_SyncLoadSampleBank: sampleBankId: %d\n", sampleBankId);

    sampleBankId = AudioLoad_GetLoadTableIndex(SAMPLE_TABLE, sampleBankId);
    ramAddr = AudioLoad_SearchCaches(2, sampleBankId);
    if (ramAddr != NULL) {
        if (gSampleFontLoadStatus[sampleBankId] != 5) {
            gSampleFontLoadStatus[sampleBankId] = 2;
        }
        *outMedium = MEDIUM_RAM;
        return ramAddr;
    }

    cachePolicy = sampleBankTable->entries[sampleBankId].cachePolicy;
    if (cachePolicy == CACHEPOLICY_4) {
        *outMedium = sampleBankTable->entries[sampleBankId].medium;
        return (void*)sampleBankTable->entries[sampleBankId].romAddr;
    }

    ramAddr = AudioLoad_SyncLoad(2, sampleBankId, &noLoad);
    if (ramAddr != NULL) {
        *outMedium = MEDIUM_RAM;
        return (void*)ramAddr;
    }

    *outMedium = sampleBankTable->entries[sampleBankId].medium;

    return (void*)sampleBankTable->entries[sampleBankId].romAddr;
}
#endif

#if 0
RECOMP_PATCH void* AudioLoad_SyncLoad(u32 tableType, u32 id, s32* didAllocate) {
    u32 size;
    AudioTable* table;
    u8* ramAddr;
    u32 medium;
    s32 loadStatus;
    u32 romAddr;
    s32 pad;
    s32 cachePolicy;
    
    recomp_printf("AudioLoad_SyncLoad: tableType: %d, id: %d, didAllocate %d\n", tableType, id, *didAllocate);

    ramAddr = AudioLoad_SearchCaches(tableType, id);
    if (ramAddr != NULL) {
        loadStatus = 2;
        *didAllocate = false;
    } else {
        table = AudioLoad_GetLoadTable(tableType);
        size = table->entries[id].size;
        size = ALIGN16(size);
        medium = table->entries[id].medium;
        cachePolicy = table->entries[id].cachePolicy;
        romAddr = table->entries[id].romAddr;

        switch (cachePolicy) {
            case CACHEPOLICY_0:
                ramAddr = AudioHeap_AllocPermanent(tableType, id, size);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;

            case CACHEPOLICY_1:
                ramAddr = AudioHeap_AllocCached(tableType, size, CACHE_PERSISTENT, id);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;

            case CACHEPOLICY_2:
                ramAddr = AudioHeap_AllocCached(tableType, size, CACHE_TEMPORARY, id);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;

            case CACHEPOLICY_3:
            case CACHEPOLICY_4:
                ramAddr = AudioHeap_AllocCached(tableType, size, CACHE_EITHER, id);
                if (ramAddr == NULL) {
                    return ramAddr;
                }
                break;
        }

        *didAllocate = true;

        if (medium == 1) {
            AudioLoad_SyncDmaUnkMedium(romAddr, ramAddr, size, table->base.unkMediumParam);
        } else {
            AudioLoad_SyncDma(romAddr, ramAddr, size, medium);
        }
        loadStatus = (cachePolicy == CACHEPOLICY_0) ? 5 : 2;
    }

    switch (tableType) {
        case SEQUENCE_TABLE:
            if (gSeqLoadStatus[id] != 5) {
                gSeqLoadStatus[id] = loadStatus;
            }
            break;

        case FONT_TABLE:
            if (gFontLoadStatus[id] != 5) {
                gFontLoadStatus[id] = loadStatus;
            }
            break;

        case SAMPLE_TABLE:
            if (gSampleFontLoadStatus[id] != 5) {
                gSampleFontLoadStatus[id] = loadStatus;
            }
            break;
    }
    return ramAddr;
}
#endif