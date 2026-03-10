#include <iostream>
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

void LinkedList::printAll()
{
      Node *curr = head;

      while (curr != NULL)
      {
            cout << curr->data.id << " | " << curr->data.name << " | " << curr->data.size << " | " << curr->data.upload_date << " | " << curr->data.source << " | " << curr->data.content << endl;

            curr = curr->next;
      }
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