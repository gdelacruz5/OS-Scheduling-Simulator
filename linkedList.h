#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node
{
   int value;
   struct node *next;
} ListNode;

ListNode* addHead(ListNode *list, int value);

ListNode* addTail(ListNode *list, int value);

ListNode* deleteNode(ListNode *list, int index);

void printList(ListNode *list);

#endif

