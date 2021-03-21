#pragma once
#include <stdint.h>

struct LINKED_NODE // Node structure for making linked lists. Can be used on its own or in combination with the LinkedList class.
{
    LINKED_NODE();
    LINKED_NODE(void *p_obj);

    void *p_Object;
    LINKED_NODE *p_Next;
};

class NodeIterator
{
private:
    uint32_t counter = 0;
    LINKED_NODE *p_current;

public:
    NodeIterator(LINKED_NODE *start);

    void *getObject();
    LINKED_NODE *getNode();

    // Sets current-pointer to the next node or nullptr if no more. Returns TRUE if next one is NOT nullptr.
    bool next();
};

class LinkedList // Linked list utilizing the LINKED_NODE structure.
{
private:
    LINKED_NODE *p_head = nullptr,
                *p_tail = nullptr;

    uint32_t counter = 0;

public:
    LinkedList();
    // Delete all linked nodes (not their content, that is up to user)
    ~LinkedList();

    bool addNode(LINKED_NODE *p_node);
    bool emplaceNode(void *p_object);
    bool removeNode(LINKED_NODE *p_node);

    // Returns the address of the first node in the list.
    LINKED_NODE *getHead();

    // Returns the address of the last node in the list.
    LINKED_NODE *getTail();

    // Returns an iterator of the linked list
    NodeIterator getIterator() const;

    uint32_t getCount() const;

private:
    bool findNode(LINKED_NODE *p_node) const;
};
