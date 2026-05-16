#ifndef HASHSYSTEM_H
#define HASHSYSTEM_H

#include <string>
#include <vector>
#include "data.h"

using namespace std;

struct HashNode
{
      Data data;
      bool isDuplicate;
      HashNode *next; 
};

class HashSystem
{
private:
      static const int TABLE_SIZE = 2003;

      HashNode *table[TABLE_SIZE];
      int totalNodes;
      int duplicateMode;

      int hashFunction(const string &key) const;
      void resetDuplicate();
      int countDuplicate() const;

      void writeGroupContent(ofstream &out, HashNode *anchor, int group);
      void writeGroupMetadata(ofstream &out, HashNode *anchor, int group);
      void writeGroupFull(ofstream &out, HashNode *anchor, int group);

      void printTableHeader() const;
      void printRow(int rowNum, const Data &d) const;

public:
      HashSystem();
      ~HashSystem();

      void insert(Data d);
      HashNode *searchByID(const string &id);
      HashNode *searchByName(const string &name);
      HashNode *searchByIDAndName(const string &id, const string &name);
      void deleteByID(const string &id);

      void printAll();
      void printDuplicates();

      void detectDuplicateByContent();
      void detectDuplicateByMetadata();
      void detectDuplicateByFullData();

      int count() const;
      void showStatistics(long insertTime, long searchTime, long deleteTime, long showTime, long detectTime);
};
#endif