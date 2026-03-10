#include <iostream>
#include <fstream>
#include "linkedList.h"

using namespace std;

LinkedList::LinkedList()
{
      head = NULL;
}

void LinkedList::insert(Data d)
{
      Node *newNode = new Node;

      newNode->data = d;
      newNode->isDuplicate = false;
      newNode->next = head;

      head = newNode;
}

Node *LinkedList::searchByID(string id)
{
      Node *curr = head;

      while (curr != NULL)
      {
            if (curr->data.id == id)
            {
                  return curr;
            }

            curr = curr->next;
      }

      return NULL;
}

Node *LinkedList::searchByName(string name)
{
      Node *curr = head;

      while (curr != NULL)
      {
            if (curr->data.name == name)
            {
                  return curr;
            }

            curr = curr->next;
      }

      return NULL;
}

Node *LinkedList::searchByIDAndName(string id, string name)
{
      Node *curr = head;

      while (curr != NULL)
      {
            if (curr->data.id == id && curr->data.name == name)
            {
                  return curr;
            }
            curr = curr->next;
      }

      return NULL;
}

bool LinkedList::existsContent(string content)
{
      Node *curr = head;

      while (curr != NULL)
      {
            if (curr->data.content == content)
            {
                  return true;
            }

            curr = curr->next;
      }

      return false;
}

void LinkedList::resetDuplicate()
{
      Node *curr = head;

      while (curr != NULL)
      {
            curr->isDuplicate = false;
            curr = curr->next;
      }
}

void LinkedList::detectDuplicateByContent()
{
      resetDuplicate();
      ofstream out("output/result.txt");

      Node *curr1 = head;
      int group = 1;

      while (curr1 != NULL)
      {
            if (!curr1->isDuplicate)
            {
                  Node *curr2 = curr1->next;
                  bool found = false;

                  while (curr2 != NULL)
                  {
                        if (curr1->data.content == curr2->data.content)
                        {
                              if (!found)
                              {
                                    out << "Duplicate Group " << group++ << " (Content)\n"
                                        << endl;
                                    out << curr1->data.id << " | " << curr1->data.name << " | " << curr1->data.content << endl;

                                    curr1->isDuplicate = true;
                                    found = true;
                              }

                              out << curr2->data.id << " | " << curr2->data.name << " | " << curr2->data.content << endl;

                              curr2->isDuplicate = true;
                        }
                        curr2 = curr2->next;
                  }
                  if (found)
                        out << "\n";
            }
            curr1 = curr1->next;
      }
      out.close();
      cout << "Duplicate detection finished. Result saved to output/result.txt\n";
}

void LinkedList::detectDuplicateByMetadata()
{
      resetDuplicate();

      ofstream out("output/result.txt", ios::trunc);

      Node *curr1 = head;
      int group = 1;

      while (curr1 != NULL)
      {
            if (!curr1->isDuplicate)
            {
                  Node *curr2 = curr1->next;
                  bool found = false;

                  while (curr2 != NULL)
                  {
                        if (curr1->data.name == curr2->data.name && curr1->data.size == curr2->data.size)
                        {
                              if (!found)
                              {
                                    out << "Duplicate Group " << group++ << " (Metadata)\n\n";
                                    out << curr1->data.id << " | " << curr1->data.name << " | " << curr1->data.size << endl;

                                    curr1->isDuplicate = true;
                                    found = true;
                              }
                              out << curr2->data.id << " | " << curr2->data.name << " | " << curr2->data.size << endl;

                              curr2->isDuplicate = true;
                        }
                        curr2 = curr2->next;
                  }
                  if (found)
                        out << "\n";
            }
            curr1 = curr1->next;
      }

      out.close();
}

void LinkedList::detectDuplicateByFullData()
{
      resetDuplicate();

      ofstream out("output/result.txt", ios::trunc);

      Node *curr1 = head;

      int group = 1;

      while (curr1 != NULL)
      {
            if (!curr1->isDuplicate)
            {
                  Node *curr2 = curr1->next;

                  bool found = false;

                  while (curr2 != NULL)
                  {
                        if (curr1->data.id == curr2->data.id && curr1->data.name == curr2->data.name && curr1->data.size == curr2->data.size && curr1->data.upload_date == curr2->data.upload_date && curr1->data.source == curr2->data.source && curr1->data.content == curr2->data.content)
                        {
                              if (!found)
                              {
                                    out << "Duplicate Group " << group++ << " (Full Data)\n\n";

                                    out << curr1->data.id << " | " << curr1->data.name << " | " << curr1->data.size << " | " << curr1->data.upload_date << " | " << curr1->data.source << " | " << curr1->data.content << endl;

                                    curr1->isDuplicate = true;
                                    found = true;
                              }
                              out << curr2->data.id << " | " << curr2->data.name << " | " << curr2->data.size << " | " << curr2->data.upload_date << " | " << curr2->data.source << " | " << curr2->data.content << endl;
                              curr2->isDuplicate = true;
                        }
                        curr2 = curr2->next;
                  }
                  if (found)
                        out << "\n";
            }
            curr1 = curr1->next;
      }
      out.close();
}

void LinkedList::printAll()
{
      Node *curr = head;

      while (curr != NULL)
      {
            cout << curr->data.id << " | " << curr->data.name << " | " << curr->data.size << " | " << curr->data.upload_date << " | " << curr->data.source << " | " << curr->data.content << endl;
            curr = curr->next;
      }
}

void LinkedList::printDuplicates()
{
      Node *curr = head;

      while (curr != NULL)
      {
            if (curr->isDuplicate)
            {
                  cout << curr->data.id << " | " << curr->data.name << " | " << curr->data.size << " | " << curr->data.upload_date << " | " << curr->data.source << " | " << curr->data.content << endl;
            }
            curr = curr->next;
      }
}

void LinkedList::deleteByID(string id)
{
      Node *curr = head;
      Node *prev = NULL;

      while (curr != NULL)
      {
            if (curr->data.id == id)
            {
                  if (prev == NULL)
                  {
                        head = curr->next;
                  }
                  else
                  {
                        prev->next = curr->next;
                  }

                  delete curr;

                  cout << "Data deleted" << endl;

                  return;
            }

            prev = curr;
            curr = curr->next;
      }

      cout << "Data not found" << endl;
}

int LinkedList::count()
{

      int total = 0;
      Node *curr = head;

      while (curr != NULL)
      {
            total++;
            curr = curr->next;
      }

      return total;
}

int LinkedList::countDuplicate()
{
      int total = 0;
      Node *curr = head;

      while (curr != NULL)
      {
            if (curr->isDuplicate)
            {
                  total++;
            }

            curr = curr->next;
      }

      return total;
}