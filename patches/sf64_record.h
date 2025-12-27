#ifndef SF64_RECORD_H
#define SF64_RECORD_H

typedef struct Record {
    u8 vis;
    u16 frame;
} Record;

extern int gWarpzoneCsFrameCount;
extern Record gWarpzoneCsRecord[19];

void UpdateVisPerFrameFromRecording_Warpzone(Record* record, s32 maxFrames);

#endif
