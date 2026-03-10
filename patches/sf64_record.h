#ifndef SF64_RECORD_H
#define SF64_RECORD_H

typedef struct Record {
    u8 vis;
    u16 frame;
} Record;

extern long gWarpzoneCsFrameCount;
extern long gA6GorgonCsFrameCount;

extern Record gWarpzoneCsRecord[19];
extern Record gA6GorgonCsRecord[12];

void UpdateVisPerFrameFromRecording(Record* record, s32 maxFrames, long* frameCounter);

#endif
