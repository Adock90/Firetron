/*This is the kernel stub of Firetron to check everything is working
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "boot.h"

void kmain(krnl_params* params)
{
	UINT32 *frame_buffer = NULL;
    UINT32 x_res = 0;
    UINT32 y_res = 0;

    frame_buffer = (UINT32 *)params->gop.FrameBufferBase;
    x_res = params->gop.Info->PixelsPerScanLine;
    y_res = params->gop.Info->VerticalResolution;

    for (UINT32 y = 0; y < y_res; y++)
    {
        for (UINT32 x = 0; x < x_res; x++)
        {
            frame_buffer[x + y * x_res] = 0xFFCC2222;
        }
    }

        for (UINT32 y = 0; y < y_res/50; y++)
    {
        for (UINT32 x = 0; x < x_res/50; x++)
        {
            frame_buffer[x + y * x_res] = 0xFFCC2222;
        }
    }
}
