#include "patches.h"

#if 1
void Gfx_Texture_UV_Scroll(Gfx* displayList, u16 width, u16 height, s32 mode, bool segmented) {
    u32 ult = 0;
    u32 lrt = 0;
    u32 lrs = 0;
    u32 uls = 0;
    Gfx* cmd = NULL;

    if (displayList == NULL) {
        return;
    }

    if (segmented) {
        cmd = SEGMENTED_TO_VIRTUAL(displayList);
    } else {
        cmd = displayList;
    }

    switch (mode) {
        case 0: // UP
            ult = cmd->words.w0 & 0xFFF;
            lrt = cmd->words.w1 & 0xFFF;

            ult = (ult - 4) & ((height * 4) - 1);
            lrt = (ult + ((height * 4) - 1)) & 0x0FFF;

            cmd->words.w0 = (G_SETTILESIZE << 24) | ult;
            cmd->words.w1 = (cmd->words.w1 & 0x0707F000) | lrt;
            break;

        case 1: // DOWN
            ult = cmd->words.w0 & 0xFFF;
            lrt = cmd->words.w1 & 0xFFF;

            ult = (ult + 4) & ((height * 4) - 1);
            lrt = (ult + ((height * 4) - 1)) & 0x0FFF;

            cmd->words.w0 = (G_SETTILESIZE << 24) | ult;
            cmd->words.w1 = (cmd->words.w1 & 0x0707F000) | lrt;
            break;

        case 2: // RIGHT
            uls = (cmd->words.w0 >> 12) & 0xFFF;
            lrs = (cmd->words.w1 >> 12) & 0xFFF;

            uls = (uls + 4) & ((width * 4) - 1);
            lrs = (uls + ((width * 4) - 1)) & 0x0FFF;

            cmd->words.w0 = (G_SETTILESIZE << 24) | (uls << 12);
            cmd->words.w1 = (cmd->words.w1 & 0x0700007F) | (lrs << 12);
            break;

        case 3: // LEFT
            uls = (cmd->words.w0 >> 12) & 0xFFF;
            lrs = (cmd->words.w1 >> 12) & 0xFFF;

            uls = (uls - 4) & ((width * 4) - 1);
            lrs = (uls + ((width * 4) - 1)) & 0x0FFF;

            cmd->words.w0 = (G_SETTILESIZE << 24) | (uls << 12);
            cmd->words.w1 = (cmd->words.w1 & 0x0700007F) | (lrs << 12);
            break;

        default: // INVALID MODE
            break;
    }
}
#endif