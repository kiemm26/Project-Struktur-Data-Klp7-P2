#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "data.h"

struct Node
{
      Data data;
      bool isDuplicate;
      Node *next;
};

class LinkedList
{
private:
      Node *head;
      int duplicateMode;

public:
      LinkedList();
      void insert(Data d);
      Node *searchByID(string id);
      Node *searchByName(string name);
      Node *searchByIDAndName(string id, string name);
      bool existsContent(string content);
      void detectDuplicateByContent();
      void detectDuplicateByMetadata();
      void detectDuplicateByFullData();
      void resetDuplicate();
      void printAll();
      void printDuplicates();
      void showStatistics(long insertTime, long searchTime, long deleteTime, long showTime, long detectTime);
      void deleteByID(string id);
      int count();
      int countDuplicate();
};

#endif