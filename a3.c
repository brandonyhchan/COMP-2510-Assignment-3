#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "a3.h"

/**
 * File:    a3.c
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
 *      This program opens an interactive menu that allows the user
 *      to select their choice from 5 options. The options available
 *      to the user include: loading an input file, merging adjacent
 *      holes in memory, compacting memory, printing the memory view
 *      and terminating the program.
 */


/**
 * Splits a linkedlist into front and back halves.
 *
 * @param source head of the linkedlist to be split
 * @param frontRef front half of linkedlist
 * @param backRef back half of linkedlist
 */
void frontBackSplit(nodeptr source, nodeptr *frontRef, nodeptr *backRef) {
  nodeptr fast;
  nodeptr slow;
  slow = source;
  fast = source->next;

  while (fast != NULL) {
    fast = fast->next;
    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  *frontRef = source;
  *backRef = slow->next;
  slow->next = NULL;
}

/**
 * Compares two nodes and returns the one with the lower base.
 *
 * @param a first node
 * @param b second node
 * @return pointer to the node with lower base. Returns the other if
 * one of them is null.
 */
nodeptr sortedMerge(nodeptr a, nodeptr b) {
  nodeptr result = NULL;

  if (a == NULL) {
    return b;
  } else if (b == NULL) {
    return a;
  }

  if (a->base <= b->base) {
    result = a;
    result->next = sortedMerge(a->next, b);
  } else {
    result = b;
    result->next = sortedMerge(a, b->next);
  }
  return result;
}

/**
 * Performs a merge sort on a linkedlist.
 *
 * @param headRef head of the linkedlist to be sorted
 */
void mergeSort(nodeptr *headRef) {
  nodeptr head = *headRef;
  nodeptr a;
  nodeptr b;

  if ((head == NULL) || head->next == NULL) {
    return;
  }

  frontBackSplit(head, &a, &b);
  mergeSort(&a);
  mergeSort(&b);

  *headRef = sortedMerge(a, b);
}

/**
 * Creates an empty linkedlist.
 *
 * @return a pointer to the newly created linkedlist
 */
llptr createLinkedList() {
  llptr linkList = malloc(sizeof(llptr));
  linkList->head = NULL;
  linkList->tail = NULL;
  linkList->numOfNodes = 0;
  return linkList;
}

/**
 * Adds a node to a linkedlist.
 *
 * @param linkList to be appended
 * @param node pointer to the node being added
 */
void addToLinkedList(llptr linkList, nodeptr node) {
  if (linkList->head == NULL) {
    linkList->head = node;
    linkList->tail = node;
    linkList->numOfNodes = 1;
  } else {
    linkList->tail->next = node;
    linkList->tail = node;
    linkList->numOfNodes++;
  }
}

/**
 * Creates a node representing a block of memory from a string.
 *
 * @param str from which the node will be created
 * @return pointer to a newly created node
 */
nodeptr createNode(char **str) {
  nodeptr node = (nodeptr) malloc(sizeof(struct Node));
  strcpy(node->identifier, str[0]);
  node->base = atoi(str[1]);
  node->limit = atoi(str[2]);
  node->next = NULL;
  return node;
}

/**
 * Reads an input file. creates nodes for each block of memory,
 * and stores a sorted linkedlist.
 *
 * @param fptr input text file
 * @param linkList storing the sorting blocks of memory
 */
void readFile(FILE *fptr, llptr linkList) {
  char buffer[100];
  while (fgets(buffer, 100, fptr) != NULL) {
    char **splitString = (char **) malloc(sizeof(char *) * 10);
    char *token;
    int counter = 0;
    token = strtok(buffer, " ");
    while (token != NULL) {
      splitString[counter] = (char *) malloc(sizeof(char) * 10);
      splitString[counter] = token;
      counter++;
      token = strtok(NULL, " ");
    }
    addToLinkedList(linkList, createNode(splitString));
  }
  mergeSort(&(linkList->head));
}

/**
 * Prints the view from memory. Blocks of memory are separated by lines.
 *
 * @param linkList containing blocks of memory
 */
void printMemory(llptr linkList) {
  nodeptr node = linkList->head;
  int counter = 1;
  while (node != NULL) {
    if (strcmp(node->identifier, "H") == 0) {
      printf("Node %d: %s (Hole), base=%d, limit=%d\n", counter, node->identifier, node->base, node->limit);
    } else {
      printf("Node %d: %s, base=%d, limit=%d\n", counter, node->identifier, node->base, node->limit);
    }
    counter++;
    node = node->next;
  }
}

/**
 * Merges adjacent holes of free memory to avoid fragmentation.
 *
 * @param linkList containing blocks of memory
 */
void mergeHoles(llptr linkList) {
  nodeptr node = linkList->head;
  int counter = 3;
  while (node->next != NULL) {
    counter++;
    nodeptr nextNode = node->next;
    if (strcmp(node->identifier, "H") == 0 && strcmp(nextNode->identifier, "H") == 0) {
      node->limit += nextNode->limit;
      node->next = nextNode->next;
    } else {
      node = node->next;
    }
  }
}

/**
 * Combines the non-process hole nodes into one node and places it at the end of the linkedlist.
 *
 * @param linkList data structure with the process data
 */
void compactMemory(llptr linkList) {
  nodeptr node = linkList->head;
  nodeptr prevProcessNode = NULL;
  int limitCounter = 0;

  while (node != NULL) {
    if (strcmp(node->identifier, "H") == 0) {
      limitCounter += node->limit;
      node = node->next;
    } else {
      if (prevProcessNode != NULL) {
        prevProcessNode->next = node;
        node->base = prevProcessNode->base + prevProcessNode->limit;
      } else {
        linkList -> head = node;
        node->base = 0;
      }
      prevProcessNode = node;
      linkList->tail = node;
      node = node->next;
    }
  }
  nodeptr newHole = (nodeptr) malloc(sizeof(struct Node));
  strcpy(newHole->identifier, "H");
  newHole->base = linkList->tail->base + linkList->tail->limit;
  newHole->limit = limitCounter;
  newHole->next = NULL;
  addToLinkedList(linkList, newHole);
}

/**
 * Contains conditional logic to execute the commands the user wishes through the menu. Contains
 * multiple options to load an input file, merge holes, compact memory and print the memory view.
 *
 * @param fileNotRead
 * @param linkedList data structure with the process data
 */
void menu(llptr linkedList) {
    int input;
    int fileNotRead = 1;
    do {
        printf("1. load an input file\n2. merge holes\n3. compact memory\n4. print memory view\n5. exit the program\n");
        scanf("%d", &input);
        switch (input) {
            case 1: {
                if (fileNotRead) {
                    fileNotRead = 0;
                    printf("1. Type the file name: ");
                    char *fileName = malloc(sizeof(char) * 20);
                    scanf("%s", fileName);
                    FILE *fptr = fopen(fileName, "r");

                    if (fptr == NULL) {
                        printf("Error: file could not be found.");
                        exit(1);
                    }
                    readFile(fptr, linkedList);
                    printf("operation successful\n");
                    break;
                } else {
                    printf("ERROR: You have already read a file. Please exit the program to read another file.\n");
                    break;
                }
            }
            case 2: {
                mergeHoles(linkedList);
                printf("operation successful\n");
                break;
            }
            case 3: {
                compactMemory(linkedList);
                printf("operation successful\n");
                break;
            }
            case 4: {
                printMemory(linkedList);
                break;
            }
            case 5: {
                printf("good bye");
                break;
            }
            default: {
                break;
            }
        }
    } while (input != 5);
}

/**
 * Main function that controls the option menu. Nested calls are
 * made based on the user's input.
 *
 * @return 0 if execution runs without error
 */
int main() {
  llptr linkList = createLinkedList();

  menu(linkList);
  return 0;
}