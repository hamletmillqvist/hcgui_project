#include "hcgui/event_handler.h"
#include <stdio.h>

namespace hcgui
{
    hcgui::EventHandler::EventHandler(hcgui::EventType event_type)
    {
        this->eventType = event_type;
    }

    hcgui::EventHandler::~EventHandler()
    {
        // Delete all subscribers (nodes delete themselves when LinkedList is destroyed)
        this->subscribers.~LinkedList();
    }

    uint32_t hcgui::EventHandler::addSubscriber(bool (*callback_addr)(hcgui::EventInfo))
    {
        this->lastSubscriberId++;

        hcgui::EVENT_SUBSCRIBER *subscriber = new hcgui::EVENT_SUBSCRIBER();
        subscriber->ID = lastSubscriberId;
        subscriber->CALLBACK_ADDR = callback_addr;

        LINKED_NODE *node = new LINKED_NODE(subscriber);

        this->subscribers.addNode(node);

        return subscriber->ID;
    }

    void hcgui::EventHandler::removeSubscriber(uint32_t subscriber_id)
    {
        for (NodeIterator iterator = this->subscribers.getIterator(); iterator.hasNext(); iterator.forward())
        {
            hcgui::EVENT_SUBSCRIBER *obj = (hcgui::EVENT_SUBSCRIBER *)iterator.getObject();

            if (obj->ID == subscriber_id)
            {
                delete obj;
                this->subscribers.removeNode(iterator.getNode());
                return;
            }
        }
    }

    void hcgui::EventHandler::triggerEvent(hcgui::EventInfo event_info)
    {
        bool event_pass_forward = true;
        NodeIterator iterator = this->subscribers.getIterator();

        for (NodeIterator iterator = subscribers.getIterator(); event_pass_forward && iterator.hasNext(); iterator.forward())
        {
            LINKED_NODE *current_node = iterator.getNode();
            hcgui::EVENT_SUBSCRIBER *p_subscriber = (hcgui::EVENT_SUBSCRIBER *)current_node->p_Object;
            event_pass_forward = p_subscriber->CALLBACK_ADDR(event_info);
        }
    }
}
