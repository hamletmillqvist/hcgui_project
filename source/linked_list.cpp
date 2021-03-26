#include "linked_list.h"
#include <stdio.h>
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

/* =========================== NODE ITERATOR =========================== */

NodeIterator::NodeIterator(LINKED_NODE *start)
    : p_current(nullptr), counter(0)
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

/* =========================== LINKED LIST =========================== */

LinkedList::LinkedList()
    : p_tail(nullptr), p_head(nullptr), counter(0)
{
}

LinkedList::~LinkedList()
{
    while (this->counter > 0)
    {
        removeNode(p_head);
    }
}

bool LinkedList::addNode(LINKED_NODE *p_node)
{
    // Argument is not null
    if (p_node != nullptr)
    {
        // If list is empty
        if (counter == 0)
        {
            p_tail = p_node;
            p_head = p_node;
            true;
        }

        // otherwise, does not already exist in the list
        if (!containsNode(p_node))
        {
            p_tail->p_Next = p_node;
            return true;
        }
    }

    return false;
}

bool LinkedList::emplaceNode(void *p_object)
{
    // Argument not null
    if (p_object != nullptr)
    {
        LINKED_NODE *p_node = new LINKED_NODE(p_object);
        addNode(p_node);
    }
}

bool LinkedList::removeNode(LINKED_NODE *p_node)
{
    // Argument not null
    if (p_node != nullptr)
    {
        bool done = false;
        for (NodeIterator iterator = getIterator(); !done && iterator.hasNext(); iterator.forward())
        {
            LINKED_NODE *node = iterator.getNode();
            if (node->p_Next == p_node)
            {
                node->p_Next = p_node->p_Next;
                p_node->p_Next = nullptr;
                p_node->p_Object = nullptr;
                delete p_node;

                done = true;
            }
        }
    }
    return false;
}

LINKED_NODE *LinkedList::getHead()
{
    return this->p_head;
}

LINKED_NODE *LinkedList::getTail()
{
    return this->p_tail;
}

NodeIterator LinkedList::getIterator() const
{
    return NodeIterator(this->p_head);
}

bool LinkedList::containsNode(LINKED_NODE *p_node) const
{
    for (NodeIterator iterator = getIterator(); iterator.hasNext(); iterator.forward())
    {
        if (iterator.getNode() == p_node)
        {
            return true;
        }
    }
    return false;
}

uint32_t LinkedList::getCount() const
{
    return this->counter;
}

void LinkedList::printList() const
{
    printf("Size: %d {\n", counter);

    for (NodeIterator iterator = getIterator(); iterator.hasNext(); iterator.forward())
    {
        LINKED_NODE *p_node = iterator.getNode();
        printf("(Adr: %p, Obj: %p, Nxt: )\n", p_node, p_node->p_Object, p_node->p_Next);
    }
    printf("}\n");
}
