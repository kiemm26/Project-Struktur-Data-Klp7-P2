#include "hashSystem.h"
#include <iostream>
#include <fstream>
string generateKey(Data d)
{
      return d.content + "_" + to_string(d.size);
}

void HashSystem::insert(Data d)
{
      string key = generateKey(d);
      table[key].push_back(d);
}

bool HashSystem::isDuplicate(Data d)
{
      string key = generateKey(d);
      if (table[key].size() > 1)
            return true;
      return false;
}

void HashSystem::showDuplicates()
{

      ofstream out("output/duplicates.txt");
      if (!out.is_open())
      {
            cout << "Gagal membuka file output\n";
            return;
      }

      int groupCount = 0;
      for (auto &pair : table)
      {
            if (pair.second.size() > 1)
            {
                  groupCount++;
                  out << "Duplicate group " << groupCount << ":\n";
                  for (auto &d : pair.second)
                  {
                        out << d.id << " " << d.name << "\n";
                  }
                  out << "\n";
            }
      }

      out.close();
      cout << "Duplicate list disimpan di output/duplicates.txt\n";
}

int HashSystem::countUnique()
{
      int uniqueCount = 0;
      for (auto &pair : table)
      {
            if (pair.second.size() == 1)
                  uniqueCount++;
      }
      return uniqueCount;
}

int HashSystem::countDuplicate()
{
      int dup = 0;
      for (auto &pair : table)
      {
            if (pair.second.size() > 1)
                  dup += pair.second.size();
      }
      return dup;
}

void HashSystem::saveStatistics(int totalData, double executionTime)
{
      ofstream out("output/statistics.txt");
      if (!out.is_open())
      {
            cout << "Gagal membuka file statistik\n";
            return;
      }

      out << "Total Data: " << totalData << "\n";
      out << "Unique Data: " << countUnique() << "\n";
      out << "Duplicate Data: " << countDuplicate() << "\n";
      out << "Execution Time: " << executionTime << " seconds\n";

      out.close();
      cout << "Statistik disimpan di output/statistics.txt\n";
}