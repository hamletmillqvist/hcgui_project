#pragma once
#include <Windows.h>

namespace hcgui
{
    enum EventType
    {
        // Input events

        KeyPress,
        MousePress,
        MouseMovement,

        // Window events

        WindowClosed,
        WindowMoved,
        WindowResized,

        // Instance events

        BufferAreaResized,

        // Amount of available event types.
        EVENT_TYPE_COUNT,
    };

    // Contains information about a certain invoked event. Use EVENT_INFO.EventType to determine the what event information to read.
    struct EVENT_INFO
    {
        hcgui::EventType EventType;

        union
        {
            struct
            {
                COORD previousPosition;
                COORD newPosition;
            } mouseMoved;

            struct
            {
                COORD previousPostion;
                COORD newPosition;
            } windowMoved;

            struct
            {
                SMALL_RECT previousRect;
                SMALL_RECT newRect;
            } windowResized;

            struct
            {
                COORD previousSize;
                COORD newSize;
            } bufferAreaResized;
        };

        bool InternalOnly = false;
    };

    // Contains information about a certain event subscriber and the callback method to invoke.
    struct EVENT_SUBSCRIBER
    {
        DWORD ID;
        bool (*CALLBACK_ADDR)(hcgui::EVENT_INFO);
    };
}
