#include "node_iterator.h"

LINKED_NODE::LINKED_NODE()
{
    p_Object = nullptr;
    p_Next = nullptr;
}

LINKED_NODE::LINKED_NODE(void *p_obj)
{
    p_Object = p_obj;
    p_Next = nullptr;
}

NodeIterator::NodeIterator(LINKED_NODE *start)
    : p_current(start), counter(0)
{
}

LINKED_NODE *NodeIterator::getNode()
{
    return p_current;
}

void *NodeIterator::getObject()
{
    return p_current != nullptr ? p_current->p_Object : nullptr;
}

bool NodeIterator::hasNext() const
{
    // Current is not null
    if (p_current != nullptr)
    {
        // If started counting and next exists OR we're still on counter 0
        if (counter == 0 || (counter > 0 && p_current->p_Next != nullptr))
        {
            return true;
        }
    }
    return false;
}

bool NodeIterator::forward()
{
    // If not already at end
    if (p_current != nullptr)
    {
        if (counter > 0)
        {
            p_current = p_current->p_Next;
        }
        counter++;
    }
    return false;
}
