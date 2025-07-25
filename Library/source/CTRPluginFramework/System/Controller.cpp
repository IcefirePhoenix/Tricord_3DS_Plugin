#include "types.h"
#include "3ds.h"
#include "CTRPluginFramework/System/Controller.hpp"

namespace CTRPluginFramework
{
    // Reference: https://3dbrew.org/wiki/HID_Shared_Memory

    u32     Controller::_keysDown = 0;
    u32     Controller::_keysHeld = 0;
    u32     Controller::_keysReleased = 0;

    extern "C" vu32* hidSharedMem;

    shortVector  Controller::GetCirclePadPosition()
    {
        circlePosition cPos;

        hidCircleRead(&cPos);
        return (shortVector(cPos.dx, cPos.dy));
    }

    // TODO: a better injection, touch input injection being not good enough
    void    Controller::InjectTouch(u16 posX, u16 posY)
    {
        touchPosition tpos = {.px = posX, .py = posY};

        for (int i = 0; i < 8; i++)
        {
            int j = 42 + 8 + i * 2;
            *(touchPosition *)(&hidSharedMem[j]) = tpos;
            hidSharedMem[j + 1] = 1;
        }

        u64 tick = svcGetSystemTick();
        hidSharedMem[42] = 0;
        *((u64*)&hidSharedMem[42]) = tick;
    }

    void    Controller::InjectKey(u32 key)
    {
        for (int i = 0; i < 8; i++)
        {
            // PAD state arrays start at 0x28 (10 * sizeof(u32) = 40 in dec)
            // each array entry is 0x10 (i * 4 * sizeof(u32) = i * 0x10)
            int j = 10 + i * 4;
            *(u32 *)(&hidSharedMem[j]) |= key;
        }
    }

    void    Controller::ClearKeys(void)
    {
        for (int i = 0; i < 8; i++)
        {
            int j = 10 + i * 4;
            *(u32 *)(&hidSharedMem[j]) = 0x0;

            // (unused for now, keeping for reference)
            // emulate button releases:
            // adding 2 to reach entry offset 0x8 (2 * sizeof(u32) = 0x8)
            // 0x00000FFF to set bits 0-11 (all input except GPIO inverted + C-Pad)
            // *(u32 *)(&hidSharedMem[j + 2]) = 0x00000FFF;
        }
    }

    void    Controller::Update(void)
    {
        hidScanInput();
        _keysDown = hidKeysDown();
        _keysHeld = hidKeysHeld();
        _keysReleased = hidKeysUp();
    }
}
