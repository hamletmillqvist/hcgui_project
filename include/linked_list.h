#pragma once
#include "node_iterator.h"

class LinkedList // Linked list utilizing the LINKED_NODE structure.
{
private:
    LINKED_NODE *p_head,
        *p_tail;

    uint32_t elementCount;

public:
    LinkedList();
    // Delete all linked nodes (not their content, that is up to user)
    ~LinkedList();

    bool addNode(LINKED_NODE *p_node);
    bool emplaceNode(void *p_object);
    bool removeNode(LINKED_NODE *p_node);
    bool containsNode(LINKED_NODE *p_node) const;

    // Returns the address of the first node in the list.
    LINKED_NODE *getHead();

    // Returns the address of the last node in the list.
    LINKED_NODE *getTail();

    // Returns an iterator of the linked list
    NodeIterator getIterator() const;

    uint32_t getCount() const;

    void printList() const;

private:
};
