#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "data.h"

struct Node
{
      Data data;
      Node *next;
};

class LinkedList
{
private:
      Node *head;

public:
      LinkedList();
      void insert(Data d);
      Node *searchByID(string id);
      Node *searchByName(string name);
      bool existsContent(string content);
      void printAll();
      int count();
};

#endif