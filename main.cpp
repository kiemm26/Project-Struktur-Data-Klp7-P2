#include <iostream>
#include <vector>

#include "data.h"
#include "linkedList.h"
#include "fileHandler.h"

using namespace std;

void showMenu()
{
      cout << endl;
      cout << "===== DUPLICATE DETECTION SYSTEM =====" << endl;
      cout << "1. Insert Data" << endl;
      cout << "2. Search Data" << endl;
      cout << "3. Detect Duplicate" << endl;
      cout << "4. Show Data" << endl;
      cout << "5. Delete Data" << endl;
      cout << "6. Statistics" << endl;
      cout << "7. Exit" << endl;
      cout << "Choose menu: ";
}

int main()
{
      LinkedList list;

      long insertTime = 0;
      long searchTime = 0;
      long deleteTime = 0;
      long showTime = 0;
      long detectTime = 0;

      vector<Data> dataset = readCSV("dataset/dataset.csv");

      for (Data d : dataset)
      {
            list.insert(d);
      }
      int choice;

      while (true)
      {
            showMenu();
            cin >> choice;
            // INSERT DATA
            switch (choice)
            {
            case 1:
            {
                  Data d;

                  cout << "ID: ";
                  cin >> d.id;

                  cout << "Name: ";
                  cin >> d.name;

                  cout << "Size (KB): ";
                  cin >> d.size;

                  cout << "Upload date (yyyy-mm-dd): ";
                  cin >> d.upload_date;

                  cout << "Source: ";
                  cin >> d.source;

                  cout << "Content: ";
                  cin >> d.content;

                  auto start = chrono::high_resolution_clock::now();

                  list.insert(d);

                  auto end = chrono::high_resolution_clock::now();
                  insertTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();

                  cout << "Data inserted successfully!" << endl;
                  break;
            }
            // SEARCH DATA
            case 2:
            {
                  int option;

                  cout << endl;
                  cout << "SEARCH DATA" << endl;
                  cout << "1. Search by ID" << endl;
                  cout << "2. Search by Name" << endl;
                  cout << "3. Search by ID and Name" << endl;
                  cout << "Choose option: ";

                  cin >> option;
                  if (option == 1)
                  {
                        string id;
                        cout << "Enter ID: ";
                        cin >> id;

                        auto start = chrono::high_resolution_clock::now();

                        Node *result = list.searchByID(id);

                        if (result != NULL)
                        {
                              cout << result->data.id << " | " << result->data.name << " | " << result->data.size << " | " << result->data.upload_date << " | " << result->data.source << " | " << result->data.content << endl;
                        }
                        else
                        {
                              cout << "Data not found" << endl;
                        }

                        auto end = chrono::high_resolution_clock::now();
                        searchTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                  }

                  else if (option == 2)
                  {
                        string name;
                        cout << "Enter name: ";
                        cin >> name;

                        auto start = chrono::high_resolution_clock::now();
                        Node *result = list.searchByName(name);
                        if (result != NULL)
                        {
                              cout << result->data.id << " | " << result->data.name << " | " << result->data.size << " | " << result->data.upload_date << " | " << result->data.source << " | " << result->data.content << endl;
                        }
                        else
                        {
                              cout << "Data not found" << endl;
                        }

                        auto end = chrono::high_resolution_clock::now();
                        searchTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                  }

                  else if (option == 3)
                  {
                        string id, name;

                        cout << "Enter ID: ";
                        cin >> id;
                        cout << "Enter Name: ";
                        cin >> name;

                        auto start = chrono::high_resolution_clock::now();

                        Node *result = list.searchByIDAndName(id, name);
                        if (result != NULL)
                        {
                              cout << result->data.id << " | " << result->data.name << " | " << result->data.size << " | " << result->data.upload_date << " | " << result->data.source << " | " << result->data.content << endl;
                        }
                        else
                        {
                              cout << "Data not found" << endl;
                        }

                        auto end = chrono::high_resolution_clock::now();
                        searchTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                  }
                  break;
            }
            // DETECT DUPLICATE
            case 3:
            {
                  int option;

                  cout << endl;
                  cout << "DETECT DUPLICATE" << endl;
                  cout << "1. Based on Content" << endl;
                  cout << "2. Based on Metadata (Name + Size)" << endl;
                  cout << "3. Based on Full Data" << endl;
                  cout << "Choose option: ";
                  cin >> option;

                  auto start = chrono::high_resolution_clock::now();

                  if (option == 1)
                  {
                        list.detectDuplicateByContent();
                  }
                  else if (option == 2)
                  {
                        list.detectDuplicateByMetadata();
                  }
                  else if (option == 3)
                  {
                        list.detectDuplicateByFullData();
                  }
                  cout << "Duplicate detection completed!" << endl;

                  auto end = chrono::high_resolution_clock::now();
                  detectTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                  break;
            }
            // SHOW DATA
            case 4:
            {
                  int option;
                  cout << endl;
                  cout << "SHOW DATA" << endl;
                  cout << "1. Show All Data" << endl;
                  cout << "2. Show Duplicate Data" << endl;
                  cout << "Choose option: ";

                  auto start = chrono::high_resolution_clock::now();

                  cin >> option;
                  if (option == 1)
                  {
                        list.printAll();
                  }
                  else if (option == 2)
                  {
                        list.printDuplicates();
                  }

                  auto end = chrono::high_resolution_clock::now();
                  showTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                  break;
            }
            // DELETE DATA
            case 5:
            {
                  string id;
                  cout << "Enter ID to delete: ";
                  cin >> id;

                  auto start = chrono::high_resolution_clock::now();

                  list.deleteByID(id);

                  auto end = chrono::high_resolution_clock::now();
                  deleteTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                  break;
            }
            // STATISTICS
            case 6:
            {
                  int total = list.count();
                  int duplicate = list.countDuplicate();

                  cout << endl;
                  cout << "Total Data : " << total << endl;
                  cout << "Duplicate Data : " << duplicate << endl;
                  cout << "Unique Data : " << total - duplicate << endl;
                  cout << endl;
                  cout << "Execution Time Statistics (ms)" << endl;
                  cout << "Insert Time : " << insertTime << endl;
                  cout << "Search Time : " << searchTime << endl;
                  cout << "Delete Time : " << deleteTime << endl;
                  cout << "Show Time : " << showTime << endl;
                  cout << "Duplicate Detection Time : " << detectTime << endl;
                  break;
            }
            // EXIT
            case 7:
            {
                  cout << "Program Ended" << endl;
                  return 0;
            }
            default:
            {
                  cout << "Invalid menu!" << endl;
                  break;
            }
            }
      }

      return 0;
}