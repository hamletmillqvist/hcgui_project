#include "hcgui/hcgui_events.h"

namespace hcgui
{
    EVENT_HANDLE createEvent(EventType type)
    {
        EVENT_HANDLE handle = new EventInfo();
        handle->EventType = EventType::FrameResized;

        return handle;
    }
}
