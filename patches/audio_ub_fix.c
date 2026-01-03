#include "patches.h"
#include "sf64audio_provisional.h"

#define RECOMP_EXPORT __attribute__((section(".recomp_export")))
#define RECOMP_PATCH __attribute__((section(".recomp_patch")))
#define RECOMP_FORCE_PATCH __attribute__((section(".recomp_force_patch")))

void AudioHeap_UnapplySampleCache(SampleCacheEntry* entry, Sample* sample);

SPTask* sNewAudioTasks_recomp[2];

// @recomp Fix undefined behaviour in audio, present in the original game
RECOMP_PATCH void AudioHeap_DiscardSampleCaches(void) {
    s32 fontId;
    s32 i;
    s32 numFonts = gSoundFontTable->base.numEntries;
    s32 pad;
    s32 sampleBankId2;
    s32 sampleBankId1;
    s32 instId;
    s32 drumId;
    Drum* drum;
    Instrument* instrument;
    SampleCacheEntry* entry;

    entry = gPersistentSampleCache.entries; // @recomp

    for (fontId = 0; fontId < numFonts; fontId++) {
        sampleBankId1 = gSoundFontList[fontId].sampleBankId1;
        sampleBankId2 = gSoundFontList[fontId].sampleBankId2;
        if (((sampleBankId1 != SAMPLES_NONE_U) && (entry->sampleBankId == sampleBankId1)) ||
            ((sampleBankId2 != SAMPLES_NONE) && (entry->sampleBankId == sampleBankId2)) ||
            (entry->sampleBankId == SAMPLES_SFX)) {
            if ((AudioHeap_SearchCaches(FONT_TABLE, CACHE_PERMANENT, fontId) != NULL) &&
                ((gFontLoadStatus[fontId] > 1) != 0)) {
                for (i = 0; i < gPersistentSampleCache.numEntries; i++) {
                    entry = &gPersistentSampleCache.entries[i];
                    for (instId = 0; instId < gSoundFontList[fontId].numInstruments; instId++) {
                        instrument = Audio_GetInstrument(fontId, instId);
                        if (instrument != NULL) {
                            if (instrument->normalRangeLo != 0) {
                                AudioHeap_UnapplySampleCache(entry, instrument->lowPitchTunedSample.sample);
                            }
                            if (instrument->normalRangeHi != 0x7F) {
                                AudioHeap_UnapplySampleCache(entry, instrument->highPitchTunedSample.sample);
                            }
                            AudioHeap_UnapplySampleCache(entry, instrument->normalPitchTunedSample.sample);
                        }
                    }
                    for (drumId = 0; drumId < gSoundFontList[fontId].numDrums; drumId++) {
                        drum = Audio_GetDrum(fontId, drumId);
                        if (drum != NULL) {
                            AudioHeap_UnapplySampleCache(entry, drum->tunedSample.sample);
                        }
                    }
                }
            }
        }
    }
}

RECOMP_PATCH void Main_Initialize(void) {
    u8 i;

    gVIsPerFrame = 0;
    gSysFrameCount = 0;
    gStartNMI = false;
    gStopTasks = false;
    gFillScreenColor = 0;
    gFillScreen = false;
    gCurrentTask = NULL;

    for (i = 0; i < ARRAY_COUNT(sAudioTasks); i += 1) {
        sAudioTasks[i] = NULL;
    }
    for (i = 0; i < ARRAY_COUNT(sGfxTasks); i += 1) {
        sGfxTasks[i] = NULL;
    }
    for (i = 0; i < ARRAY_COUNT(sNewAudioTasks_recomp); i += 1) {
        sNewAudioTasks_recomp[i] = NULL;
    }
    for (i = 0; i < ARRAY_COUNT(sNewGfxTasks); i += 1) {
        sNewGfxTasks[i] = NULL;
    }
}

RECOMP_PATCH void Main_GetNewTasks(void) {
    u8 i;
    SPTask** audioTask;
    SPTask** gfxTask;
    SPTask** newAudioTask;
    SPTask** newGfxTask;
    OSMesg spTaskMsg;
    SPTask* newTask;

    newAudioTask = sNewAudioTasks_recomp;
    newGfxTask = sNewGfxTasks;
    for (i = 0; i < ARRAY_COUNT(sNewAudioTasks_recomp); i += 1) {
        *(newAudioTask++) = NULL;
    }
    for (i = 0; i < ARRAY_COUNT(sNewGfxTasks); i += 1) {
        *(newGfxTask++) = NULL;
    }

    newAudioTask = sNewAudioTasks_recomp;
    newGfxTask = sNewGfxTasks;
    while (MQ_GET_MESG(&gTaskMesgQueue, &spTaskMsg)) {
        newTask = (SPTask*) spTaskMsg;
        newTask->state = SPTASK_STATE_NOT_STARTED;

        switch (newTask->task.t.type) {
            case M_AUDTASK:
                *(newAudioTask++) = newTask;
                break;
            case M_GFXTASK:
                *(newGfxTask++) = newTask;
                break;
        }
    }
    newAudioTask = sNewAudioTasks_recomp;
    newGfxTask = sNewGfxTasks;
    audioTask = sAudioTasks;
    gfxTask = sGfxTasks;

    for (i = 0; i < ARRAY_COUNT(sAudioTasks); i += 1, audioTask++) {
        if (*audioTask == NULL) {
            break;
        }
    }
    for ((void) i; i < ARRAY_COUNT(sAudioTasks); i += 1) {
        *(audioTask++) = *(newAudioTask++);
    }

    for (i = 0; i < ARRAY_COUNT(sGfxTasks); i += 1, gfxTask++) {
        if (*gfxTask == NULL) {
            break;
        }
    }
    for ((void) i; i < ARRAY_COUNT(sGfxTasks); i += 1) {
        *(gfxTask++) = *(newGfxTask++);
    }
}