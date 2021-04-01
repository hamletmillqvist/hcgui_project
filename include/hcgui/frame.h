#pragma once

#include "hcgui_internals.h"
#include "hcgui_internals.h"

namespace hcgui
{
    class Frame;

#ifndef FRAME_HANDLE
#define FRAME_HANDLE hcgui::Frame *
#endif

    const SMALL_RECT FRAME_DEFAULT_RECT{0, 0, 10, 10};
    const uint16_t FRAME_DEFAULT_BACKGROUND_ATTRIBUTES = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY,
                   FRAME_DEFAULT_TITLEBAR_ATTRIBUTES = BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_INTENSITY;

    class Frame
    {
    private:
        const char *title;
        FRAME_HANDLE parent;
        SMALL_RECT rect;
        uint16_t backgroundAttribues, titlebarAttributes;

    public:
        Frame(FRAME_HANDLE p_parent, const char *p_title);
        ~Frame();

        void setTitle(const char *p_title);
        const char *getTitle() const;

        SMALL_RECT getRect() const;
        void setRect(SMALL_RECT new_rect);
    };
}
