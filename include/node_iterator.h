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
    uint32_t counter;
    LINKED_NODE *p_current;

public:
    NodeIterator(LINKED_NODE *start);

    void *getObject();
    LINKED_NODE *getNode();

    // Sets current-pointer to the next node or nullptr if no more. Returns TRUE if next one is NOT nullptr.
    bool forward();
    bool hasNext() const;
};
