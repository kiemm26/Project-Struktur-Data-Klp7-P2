#include <iostream>
#include <vector>

#include "data.h"
#include "linkedList.h"
#include "fileHandler.h"

using namespace std;

int main()
{

      LinkedList list;
      vector<Data> dataset = readCSV("dataset/dataset.csv");

      cout << "Dataset loaded : " << dataset.size() << endl;

      for (Data d : dataset)
      {
            list.insert(d);
      }

      cout << "Data inserted to LinkedList" << endl;
      cout << "Total data in system : " << list.count() << endl;
      cout << endl;
      cout << "Sample data : " << endl;

      list.printAll();

      return 0;
}