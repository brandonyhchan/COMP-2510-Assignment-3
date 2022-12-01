#ifndef A3_A3_H
#define A3_A3_H

/**
 * Shorthand to point to a node struct in the memory.
 */
typedef struct Node* nodeptr;

/**
 * Shorthand to point to a linked list "object" in the memory
 */
typedef struct LL* llptr;

/**
 * A struct representing a node in the linked list containing the identifier, base limit and a pointer
 * to the next node in the list.
 */
struct Node {
  char identifier[4];
  int base;
  int limit;
  nodeptr next;
};

/**
 * A struct representing a linked list, with pointers pointing to the head and the tail of the list. Also
 * keeps track of the number of nodes in the list.
 */
struct LL {
  nodeptr head;
  nodeptr tail;
  int numOfNodes;
};

#endif //A3_A3_H
