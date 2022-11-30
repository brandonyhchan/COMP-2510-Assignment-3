//
// Created by Princeton Dychinco on 2022-11-27.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "a3.h"

void frontBackSplit(nodeptr source, nodeptr* frontRef, nodeptr* backRef) {
  nodeptr fast;
  nodeptr slow;
  slow = source;
  fast = source -> next;

  while (fast != NULL) {
    fast = fast -> next;
    if (fast != NULL) {
      slow = slow -> next;
      fast = fast -> next;
    }
  }

  *frontRef = source;
  *backRef = slow -> next;
  slow -> next = NULL;
}

nodeptr sortedMerge(nodeptr a, nodeptr b) {
  nodeptr result = NULL;

  if(a == NULL) {
    return b;
  } else if (b == NULL) {
    return a;
  }

  if(a -> base <= b -> base) {
    result = a;
    result -> next = sortedMerge(a->next, b);
  } else {
    result = b;
    result -> next = sortedMerge(a, b-> next);
  }
  return result;
}

void mergeSort(nodeptr* headRef) {
  nodeptr head = *headRef;
  nodeptr a;
  nodeptr b;

  if((head == NULL) || head->next == NULL) {
    return;
  }

  frontBackSplit(head, &a, &b);
  mergeSort(&a);
  mergeSort(&b);

  *headRef = sortedMerge(a,b);
}

llptr createLinkedList() {
  llptr linkList = malloc(sizeof(llptr));
  linkList -> head = NULL;
  linkList -> tail = NULL;
  linkList -> numOfNodes = 0;
  return linkList;
}

void addToLinkedList(llptr linkList, nodeptr node) {
  if (linkList->head == NULL) {
    linkList -> head = node;
    linkList -> tail = node;
    linkList -> numOfNodes = 1;
  } else {
    linkList -> tail -> next = node;
    linkList -> tail = node;
    linkList -> numOfNodes++;
  }
}

void saveString(nodeptr node, char* str) {
  for(int i = 0; i < strlen(str); i++) {
    node -> identifier[i] = str[i];
  }
}

nodeptr createNode(char** str) {
  nodeptr node = (nodeptr) malloc(sizeof(struct Node));
  strcpy(node->identifier, str[0]);
  node -> base = atoi(str[1]);
  node -> limit = atoi(str[2]);
  node -> next = NULL;
  return node;
}

void readFile(FILE* fptr, llptr linkList) {
  char buffer[100];
  while(fgets(buffer, 100, fptr) != NULL) {
    char** splitString = (char**)malloc(sizeof(char*) * 10);
    char* token;
    int counter = 0;
    token = strtok(buffer, " ");
    while(token != NULL) {
      splitString[counter] = (char*)malloc(sizeof(char) * 10);
      splitString[counter] = token;
      counter++;
      token = strtok(NULL," ");
    }
    addToLinkedList(linkList, createNode(splitString));
  }
//  mergeSort(&(linkList -> head));
}

void printMemory(llptr linkList) {
  nodeptr node = linkList -> head;
  int counter = 1;
  while(node != NULL) {
    if(strcmp(node->identifier, "H") == 0) {
      printf("Node %d: %s (Hole), base=%d, limit=%d\n",counter, node->identifier, node->base, node->limit);
    } else {
      printf("Node %d: %s, base=%d, limit=%d\n",counter, node->identifier, node->base, node->limit);
    }
    counter++;
    node = node -> next;
  }
}

void mergeHoles(llptr linkList) {
  printf("2 segfault");
  nodeptr node = linkList -> head;
  int counter = 3;
  while(node != NULL) {
    counter++;
    nodeptr nextNode = node -> next;
    if(strcmp(node -> identifier, "H") == 0 && strcmp(nextNode -> identifier, "H") == 0) {
      node -> limit += nextNode -> limit;
      node -> next = nextNode -> next;
    } else {
      node = node -> next;
    }
  }
}

/**
 * combines the non-process hole nodes into one node and places it at the end of the linkedlist
 * @param linkList data structure with the process data
 */
void compactMemory(llptr linkList) {
    nodeptr node = linkList -> head;
    nodeptr prevProcessNode = NULL;
    int limitCounter = 0;

    while(node != NULL) {
        if(strcmp(node -> identifier, "H") == 0) {
            limitCounter += node -> limit;
            node = node -> next;
        } else {
            if(prevProcessNode != NULL) {
                prevProcessNode->next = node;
                node->base = prevProcessNode->base + prevProcessNode->limit;
            } else {
                node->base = 0;
            }
            prevProcessNode = node;
            linkList->tail = node;
            node = node -> next;
        }
    }
    nodeptr newHole = (nodeptr) malloc(sizeof(struct Node));
    strcpy(newHole->identifier, "H");
    newHole->base = linkList->tail->base + linkList->tail->limit;
    newHole->limit = limitCounter;
    newHole->next = NULL;
    addToLinkedList(linkList, newHole);
}

int main() {
  int input;
  int fileNotRead = 1;
  llptr linkList = createLinkedList();
  do{
    printf("1. load an input file\n2. merge holes\n3. compact memory\n4. print memory view\n5. exit the program\n");
    scanf("%d", &input);
    switch(input) {
      case 1: {
        if(fileNotRead) {
          fileNotRead = 0;
          printf("1. Type the file name: ");
          char* fileName = malloc(sizeof(char) * 20);
          scanf("%s", fileName);
          FILE* fptr = fopen(fileName, "r");
          readFile(fptr, linkList);
          printf("operation successful\n");
          break;
        } else {
          printf("ERROR: You have already read a file. Please exit the program to read another file.\n");
          break;
        }
      }
      case 2: {
        printf("1 segfault\n");
        mergeHoles(linkList);
        printf("operation successful\n");
        break;
      }
      case 3: {
        printf("compacting memory");
        compactMemory(linkList);
        printf("operation successful\n");
        break;
      }
      case 4: {
        printMemory(linkList);
        break;
      }
      case 5: {
        printf("good bye");
        break;
      }
    }
  }while(input !=5);
  return 0;
}