/**
 * File:    a3.h
 *
 * Author: Buck Sin - A00805677
 * Author: Brandon Chan - A01026254
 * Author: Princeton Dychinco - A00849214
 * Author: Stanley Chow - A01266659
 *
 * COMP 2510: Set B
 * Date: Nov 30, 2022
 *
 * Summary of File:
 *
 *      The header file for a3.c. Contains typdefs for pointers to a node and the linked list, and node. Contains
 *      the struct definitions for the nodes in a linked list and the linked list itself.
 */

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
