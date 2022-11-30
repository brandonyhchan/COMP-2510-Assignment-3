//
// Created by Princeton Dychinco on 2022-11-27.
//

#ifndef A3_A3_H
#define A3_A3_H

typedef struct Node* nodeptr;
typedef struct LL* llptr;

struct Node {
  char identifier[4];
  int base;
  int limit;
  nodeptr next;
};

struct LL {
  nodeptr head;
  nodeptr tail;
  int numOfNodes;
};

#endif //A3_A3_H
