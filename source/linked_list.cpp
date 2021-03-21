#include "linked_list.h"

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
{
    this->p_current = start;
}

LINKED_NODE *NodeIterator::getNode()
{
    return p_current;
}

void *NodeIterator::getObject()
{
    if (p_current != nullptr)
    {
        return p_current->p_Object;
    }
    else
    {
        return nullptr;
    }
}

bool NodeIterator::next()
{
    if (p_current != nullptr)
    {
        p_current = p_current->p_Next;
        counter++;

        if (p_current != nullptr)
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

/* =========================== LINKED LIST =========================== */

LinkedList::LinkedList() {}

LinkedList::~LinkedList()
{
    while (this->counter > 0)
    {
        removeNode(p_head);
    }
}

bool LinkedList::addNode(LINKED_NODE *node)
{
    // Argument is not null
    if (node != nullptr)
    {
        // voidail exists, add onto tail
        if (this->p_tail != nullptr)
        {
            this->p_tail->p_Next = node;
        }
        // Set new node as tail and head
        else
        {
            if (!findNode(node))
            {
                this->p_head = node;
                this->p_tail = node;
            }
        }
        counter++;
        return true;
    }
    return false;
}

bool LinkedList::emplaceNode(void *p_object)
{
    if (p_object != nullptr)
    {
        LINKED_NODE *node = new LINKED_NODE(p_object);
        return this->addNode(node);
    }
    else
    {
        return false;
    }
}

bool LinkedList::removeNode(LINKED_NODE *p_node)
{
    bool shouldDelete = false;

    // Argument is not null
    if (p_node != nullptr)
    {
        // Is only element in list
        if (p_tail == p_head && p_tail == p_node)
        {
            shouldDelete = true;
            p_tail = nullptr;
            p_head = nullptr;
        }
        // Is the head
        else if (p_head == p_node)
        {
            shouldDelete = true;
            p_head = p_head->p_Next;
        }
        // We need to search the list
        else
        {
            LINKED_NODE *current = p_head;

            while (!shouldDelete && current != nullptr)
            {
                // We found the node
                if (current->p_Next == p_node)
                {
                    current->p_Next = current->p_Next->p_Next;
                    shouldDelete = true;
                }
                else
                {
                    current = current->p_Next;
                }
            }
        }

        if (shouldDelete)
        {
            delete p_node;
            counter--;
            return true;
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

bool LinkedList::findNode(LINKED_NODE *p_node) const
{
    for (NodeIterator iterator = getIterator(); iterator.next();)
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
