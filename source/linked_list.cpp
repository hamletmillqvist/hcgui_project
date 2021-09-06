#include "linked_list.h"
#include <stdio.h>

LinkedList::LinkedList()
    : p_last(nullptr), p_first(nullptr), elementCount(0)
{
}

LinkedList::~LinkedList()
{
    while (this->elementCount > 0)
    {
        removeNode(p_first);
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
            // Place node at the start and update both "first" and "last" pointers to use the new node.
            p_last = p_node;
            p_first = p_node;
            elementCount++;
            return true;
        }

        // otherwise, does not already exist in the list
        else if (!containsNode(p_node))
        {
            // Place new node after the last one, then move the "last"-pointer to the new node.
            p_last->p_Next = p_node;
            p_last = p_node;
            elementCount++;
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
        // Wrap object pointer inside a new linked node.
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
        // If target node is the first node
        if (p_first = p_node)
        {
            p_first = nullptr;

            // If it is also the last node
            if (p_last == p_node)
            {
                p_last = nullptr;
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
                    // If next node is the target
                    if (current_node->p_Next == p_node)
                    {
                        // If next node is not the last node, then hook current node to the one after next.
                        if (p_last != p_node)
                        {
                            // Link Node before target to the one after the target. Steps are as follows:
                            // (A) -> (B) -> (C)
                            // (A) & (B) -> C
                            current_node->p_Next = p_node->p_Next;
                        }

                        // Otherwise, if the target node is the last node.
                        if (p_last == p_node)
                        {
                            // Move the last-node-pointer to the current node instead, then set current.next to be null
                            p_last = current_node;
                            current_node->p_Next = nullptr;
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
    return this->p_first;
}

LINKED_NODE *LinkedList::getLast()
{
    return this->p_last;
}

NodeIterator LinkedList::getIterator() const
{
    return NodeIterator(this->p_first);
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
    return elementCount;
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
