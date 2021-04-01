#pragma once
#include <Windows.h>

namespace hcgui
{
    // Identification for different kinds of events.
    enum EventType
    {
        // =============== Input events ===============

        // =============== Frame events ===============

        FrameResized,

        // =============== Instance events ===============

        BufferAreaResized,

        // =============== CONSTANT, THIS IT NOT EVENT TYPE ===============

        // Amount of available event types.
        EVENT_TYPE_COUNT,
    };

    // Contains information about a certain invoked event. Use EventInfo.EventType to determine the what event information to read.
    struct EventInfo
    {
        hcgui::EventType EventType;

        // Contains all event data associated with the event type.
        union
        {
            struct
            {
                SMALL_RECT old_rect;
                SMALL_RECT new_rect;
            } frameResized;
            struct
            {
                COORD previousSize;
                COORD newSize;
            } bufferAreaResized;
        } Data;

        bool InternalOnly = false;
    };

#ifndef EVENT_HANDLE
#define EVENT_HANDLE EventInfo *
#endif

    // Returns an event handle with the appropriate EventType id.
    EVENT_HANDLE createEvent(EventType type);

    // Contains information about a certain event subscriber and the callback method to invoke.
    struct EVENT_SUBSCRIBER
    {
        DWORD ID;
        bool (*CALLBACK_ADDR)(hcgui::EventInfo);
    };
}
