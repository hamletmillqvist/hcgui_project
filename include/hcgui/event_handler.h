#pragma once
#include "hcgui_events.h"
#include "linked_list.h"

namespace hcgui
{
    // Contains a linked list of all event subscribers and the type of events it handles.
    class EventHandler
    {
    private:
        hcgui::EventType eventType;
        LinkedList subscribers;
        uint32_t lastSubscriberId = 0;

    public:
        EventHandler(hcgui::EventType event_type);
        ~EventHandler();
        uint32_t addSubscriber(bool (*CALLBACK_ADDR)(hcgui::EVENT_INFO));
        void removeSubscriber(uint32_t subscriber_id);

        void triggerEvent(hcgui::EVENT_INFO event_info);
    };
}
