#include "linked_list.h"
#include <stdio.h>

LinkedList::LinkedList()
    : p_tail(nullptr), p_head(nullptr), elementCount(0)
{
}

LinkedList::~LinkedList()
{
    while (this->elementCount > 0)
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
        if (elementCount == 0)
        {
            p_tail = p_node;
            p_head = p_node;
            return true;
            elementCount++;
        }

        // otherwise, does not already exist in the list
        if (!containsNode(p_node))
        {
            p_tail->p_Next = p_node;
            return true;
            elementCount++;
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
        return true;
    }
    return false;
}

bool LinkedList::removeNode(LINKED_NODE *p_node)
{
    // Argument not null and list is not empty
    if (p_node != nullptr && elementCount > 0)
    {
        // If target node is the head
        if (p_head = p_node)
        {
            p_head = nullptr;

            // If it is also the tail
            if (p_tail == p_node)
            {
                p_tail = nullptr;
            }

            delete p_node;
            elementCount--;
            return true;
        }
        else
        {
            bool done = false;
            for (NodeIterator iterator = getIterator(); !done && iterator.hasNext(); iterator.forward())
            {
                LINKED_NODE *current_node = iterator.getNode();
                if (current_node != nullptr)
                {
                    // Next node is the target
                    if (current_node->p_Next == p_node)
                    {
                        // Link Node before target to the one after the target.
                        current_node->p_Next = p_node->p_Next;

                        // If the target node is the tail, move tail backwards one step
                        if (p_tail == p_node)
                        {
                            current_node = p_tail;
                        }

                        // Clean up original node data to more easily perform error checking
                        p_node->p_Next = nullptr;
                        p_node->p_Object = nullptr;
                        delete p_node;

                        // End the loop and decrement the elementCount
                        elementCount--;
                        done = true;
                    }
                }
            }
            return done;
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
    return this->elementCount;
}

#ifdef DEBUG_BUILD
void LinkedList::printList() const
{
    printf("Size: %d {\n", this->elementCount);
    // Prevents infinite loop
    if (this->elementCount > 0)
    {
        for (NodeIterator iterator = getIterator(); iterator.hasNext(); iterator.forward())
        {
            LINKED_NODE *p_node = iterator.getNode();
            printf("(Adr: %p, Obj: %p, Nxt: )\n", p_node, p_node->p_Object, p_node->p_Next);
        }
    }
    printf("}\n");
}
#else
void LinkedList::printList() const
{
    return; // removes warning for empty method
}
#endif
