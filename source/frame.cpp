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

        COORD size = {this->rect.Right + 1, this->rect.Bottom + 1};
        uint16_t lenght = size.X * size.Y;
        CHAR_INFO *p_buffer = (CHAR_INFO *)malloc(sizeof(CHAR_INFO) * lenght);

        CHAR_INFO c;
        c.Attributes = 0;
        c.Char.UnicodeChar = '\n';
        for (uint32_t i = 0; i < lenght; i++)
            p_buffer[i] = c;

        // We use seperate for-loops independently for title and background, instead of using a single for-loop with IF-statements inside.
        // This is actually faster.

        // Draw frame background
        c.Attributes = this->backgroundAttribues;
        c.Char.UnicodeChar = ' ';

        for (int16_t y = 0; y < size.Y - 1; y++)
        {
            for (int16_t x = 0; x < size.X - 1; x++)
            {
                p_buffer[y * size.X + x] = c;
            }
        }

        // Apply title text
        size_t title_lenght = strlen(this->title);
        for (int16_t x = 1; x < size.X && x - 1 < title_lenght; x++)
        {
            c.Char.UnicodeChar = this->title[x - 1];
            p_buffer[x] = c;
        }

        // If shadows are applicable, draw them
        c.Attributes = FRAME_DEFAULT_SHADOW_ATTRIBUTES;
        c.Char.UnicodeChar = ' ';
        const short right = size.X - 1;
        for (int16_t y = 1; y < size.Y; y++)
        {
            p_buffer[y * size.X + right] = c;
        }
        for (int16_t x = 1; x < size.X - 1; x++)
        {
            p_buffer[(size.Y - 1) * size.X + x] = c;
        }

        // Assemble DrawingArea structure and return it
        hcgui::DrawingArea drawing;
        drawing.BufferCoords = size;
        drawing.BufferLenght = lenght;
        drawing.p_Buffer = p_buffer;
        drawing.WindowSize = this->rect;
        drawing.WindowSize.Right = size.X;
        drawing.WindowSize.Bottom = size.Y;
        return drawing;
    }
}
