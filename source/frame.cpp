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

    hcgui::DrawingArea Frame::makeDrawable()
    {
        // TODO : Cache last calculated drawable and only recalculate on change?

        COORD size = {this->rect.Right, this->rect.Bottom};
        uint16_t lenght = size.X * size.Y;
        CHAR_INFO *p_buffer = (CHAR_INFO *)malloc(sizeof(CHAR_INFO) * lenght);

        CHAR_INFO c;

        // We use seperate for-loops independently for title and background, instead of using a single for-loop with IF-statements inside.
        // This is actually faster.

        // Draw titlebar
        uint8_t titleLenght = strlen(this->title), counter = 0;
        c.Attributes = this->titlebarAttributes;
        for (int16_t y = 0; y < FRAME_TITLEBAR_HEIGHT; y++)
        {
            for (int16_t x = 0; x < size.X; x++)
            {
                c.Char.UnicodeChar = counter < titleLenght ? this->title[counter++] : ' '; // TODO : Still thinking of better way to solve this, preferably without conditions inside loop
                p_buffer[y * size.X + x] = c;
            }
        }

        // Draw frame background
        c.Char.UnicodeChar = ' ';
        c.Attributes = this->backgroundAttribues;
        for (int16_t y = FRAME_TITLEBAR_HEIGHT; y < size.Y; y++)
        {
            for (int16_t x = 0; x < size.X; x++)
            {
                p_buffer[y * size.X + x] = c;
            }
        }

        // Assemble DrawingArea structure and return it
        hcgui::DrawingArea drawing;
        drawing.BufferCoords = size;
        drawing.BufferLenght = lenght;
        drawing.p_Buffer = p_buffer;
        drawing.WindowSize = this->rect;
        return drawing;
    }
}
