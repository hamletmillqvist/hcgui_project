#include "hcgui/frame.h"

namespace hcgui
{
    Frame::Frame(FRAME_HANDLE p_parent, const char *p_title)
        : parent(p_parent), title(p_title), rect(FRAME_DEFAULT_RECT), backgroundAttribues(FRAME_DEFAULT_BACKGROUND_ATTRIBUTES), titlebarAttributes(FRAME_DEFAULT_TITLEBAR_ATTRIBUTES)
    {
    }

    Frame::~Frame()
    {
    }

    void Frame::setTitle(const char *p_title)
    {
        this->title = p_title;
    }

    const char *Frame::getTitle() const
    {
        return this->title;
    }

    SMALL_RECT Frame::getRect() const
    {
        return this->rect;
    }

    void Frame::setRect(SMALL_RECT new_rect)
    {
        // Prepare event to be triggered on next polling
        EVENT_HANDLE eh = createEvent(EventType::FrameResized);
        eh->Data.frameResized = {this->rect, new_rect};
        scheduleEvent(eh);

        this->rect = new_rect;
    }
}
