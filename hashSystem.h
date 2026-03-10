#ifndef HASHSYSTEM_H
#define HASHSYSTEM_H

#include <unordered_map>
#include <vector>
#include "data.h"

class HashSystem
{
private:
      unordered_map<string, vector<Data>> table;

public:
      void insert(Data d);
      bool isDuplicate(Data d);
      void showDuplicates();
      void saveStatistics(int totalData, double executionTime);
      int countUnique();
      int countDuplicate();
};
#endif