#include "../include/avlSystem.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

//  CONSTRUCTOR / DESTRUCTOR
AVLSystem::AVLSystem()
    : root(nullptr), totalNodes(0), duplicateMode(0) {}

AVLSystem::~AVLSystem()
{
      destroyTree(root);
}

void AVLSystem::destroyTree(AVLNode *n)
{
      if (n == nullptr)
            return;
      destroyTree(n->left);
      destroyTree(n->right);
      delete n;
}

//  HEIGHT & BALANCE
int AVLSystem::height(AVLNode *n) const
{
      return (n == nullptr) ? -1 : n->height;
}

int AVLSystem::balanceFactor(AVLNode *n) const
{
      return (n == nullptr) ? 0 : height(n->left) - height(n->right);
}

void AVLSystem::updateHeight(AVLNode *n)
{
      if (n == nullptr)
            return;
      n->height = 1 + max(height(n->left), height(n->right));
}

//  ROTATIONS
AVLNode *AVLSystem::rotateRight(AVLNode *y)
{
      AVLNode *x = y->left;
      AVLNode *T2 = x->right;

      // Perform rotation
      x->right = y;
      y->left = T2;

      // Update heights (y first because it is now lower)
      updateHeight(y);
      updateHeight(x);

      return x; // new subtree root
}

AVLNode *AVLSystem::rotateLeft(AVLNode *x)
{
      AVLNode *y = x->right;
      AVLNode *T2 = y->left;

      y->left = x;
      x->right = T2;

      updateHeight(x);
      updateHeight(y);

      return y;
}

AVLNode *AVLSystem::rotateLeftRight(AVLNode *n)
{
      n->left = rotateLeft(n->left);
      return rotateRight(n);
}

AVLNode *AVLSystem::rotateRightLeft(AVLNode *n)
{
      n->right = rotateRight(n->right);
      return rotateLeft(n);
}

//  rebalance – pilih rotasi yang tepat berdasarkan BF
AVLNode *AVLSystem::rebalance(AVLNode *n)
{
      updateHeight(n);
      int bf = balanceFactor(n);

      // ── Left-heavy ──────────────────────────────────────────────────────────
      if (bf > 1)
      {
            if (balanceFactor(n->left) >= 0)
                  return rotateRight(n); // LL
            else
                  return rotateLeftRight(n); // LR
      }

      // ── Right-heavy ─────────────────────────────────────────────────────────
      if (bf < -1)
      {
            if (balanceFactor(n->right) <= 0)
                  return rotateLeft(n); // RR
            else
                  return rotateRightLeft(n); // RL
      }

      return n; // already balanced
}

//  INSERT (recursive)
AVLNode *AVLSystem::insertNode(AVLNode *n, Data d)
{
      // Standard BST insert
      if (n == nullptr)
      {
            AVLNode *node = new AVLNode;
            node->data = d;
            node->isDuplicate = false;
            node->height = 0;
            node->left = nullptr;
            node->right = nullptr;
            return node;
      }

      if (d.id < n->data.id)
            n->left = insertNode(n->left, d);
      else if (d.id > n->data.id)
            n->right = insertNode(n->right, d);
      else
      {
            // Duplicate IDs: insert into right subtree with a slight key modification
            // is not ideal; instead we allow equal keys to go right (consistent policy).
            n->right = insertNode(n->right, d);
      }

      return rebalance(n);
}

void AVLSystem::insert(Data d)
{
      root = insertNode(root, d);
      totalNodes++;
}

//  DELETE (recursive)
AVLNode *AVLSystem::minValueNode(AVLNode *n) const
{
      AVLNode *curr = n;
      while (curr->left != nullptr)
            curr = curr->left;
      return curr;
}

AVLNode *AVLSystem::deleteNode(AVLNode *n, const string &id, bool &deleted)
{
      if (n == nullptr)
            return nullptr;

      if (id < n->data.id)
      {
            n->left = deleteNode(n->left, id, deleted);
      }
      else if (id > n->data.id)
      {
            n->right = deleteNode(n->right, id, deleted);
      }
      else
      {
            // Node found
            deleted = true;

            // Case 1: one or no child
            if (n->left == nullptr || n->right == nullptr)
            {
                  AVLNode *child = (n->left != nullptr) ? n->left : n->right;
                  delete n;
                  return child; // rebalance will be called on the way back up
            }

            // Case 2: two children – replace with in-order successor
            AVLNode *successor = minValueNode(n->right);
            n->data = successor->data;
            n->isDuplicate = successor->isDuplicate;

            // Delete the successor (it has at most one right child)
            bool dummyDeleted = false;
            n->right = deleteNode(n->right, successor->data.id, dummyDeleted);
      }

      return rebalance(n);
}

void AVLSystem::deleteByID(const string &id)
{
      bool deleted = false;
      root = deleteNode(root, id, deleted);

      if (deleted)
      {
            totalNodes--;
            cout << "Data deleted" << endl;
      }
      else
      {
            cout << "Data not found" << endl;
      }
}

//  SEARCH
AVLNode *AVLSystem::searchByID(const string &id)
{
      AVLNode *curr = root;
      while (curr != nullptr)
      {
            if (id == curr->data.id)
                  return curr;
            else if (id < curr->data.id)
                  curr = curr->left;
            else
                  curr = curr->right;
      }
      return nullptr;
}

// Full scan (inorder) – no secondary index on name
AVLNode *AVLSystem::searchByName(const string &name)
{
      vector<AVLNode *> all;
      inorder(root, all);

      for (AVLNode *n : all)
            if (n->data.name == name)
                  return n;

      return nullptr;
}

AVLNode *AVLSystem::searchByIDAndName(const string &id, const string &name)
{
      AVLNode *byID = searchByID(id);
      if (byID != nullptr && byID->data.name == name)
            return byID;
      return nullptr;
}

//  TRAVERSAL
void AVLSystem::inorder(AVLNode *n, vector<AVLNode *> &out) const
{
      if (n == nullptr)
            return;
      inorder(n->left, out);
      out.push_back(n);
      inorder(n->right, out);
}

//  PRINT HELPERS
static string avlTruncate(const string &text, int width)
{
      if ((int)text.length() <= width)
            return text;
      return text.substr(0, width - 3) + "...";
}

void AVLSystem::printTableHeader() const
{
      cout << "\n+-----------------------------------------------------------------------------------------------------------------------------+" << endl;
      cout << "| #  | ID         | Name                 | Size      | Upload Date  | Source                     | Content                    |" << endl;
      cout << "+-----------------------------------------------------------------------------------------------------------------------------+" << endl;
}

void AVLSystem::printRow(int rowNum, const Data &d) const
{
      cout << "| " << left
           << setw(2) << rowNum << " | "
           << setw(10) << avlTruncate(d.id, 10) << " | "
           << setw(20) << avlTruncate(d.name, 18) << " | "
           << setw(9) << d.size << " | "
           << setw(12) << d.upload_date << " | "
           << setw(26) << avlTruncate(d.source, 26) << " | "
           << setw(27) << avlTruncate(d.content, 26) << "|"
           << endl;
}

//  PRINT ALL  (paginated, 20 rows per page, inorder = sorted by ID)
void AVLSystem::printAll()
{
      vector<AVLNode *> rows;
      inorder(root, rows);

      if (rows.empty())
      {
            cout << "No data available." << endl;
            return;
      }

      const int pageSize = 20;
      int page = 0;
      int total = (int)rows.size();

      while (true)
      {
            int start = page * pageSize;
            if (start >= total)
                  break;
            int end = min(start + pageSize, total);

            printTableHeader();
            for (int i = start; i < end; i++)
                  printRow(i + 1, rows[i]->data);

            cout << "+-----------------------------------------------------------------------------------------------------------------------------+" << endl;
            cout << "Showing data " << (start + 1) << "-" << end << " of " << total << endl;

            if (end >= total)
                  break;

            cout << "Next page? (y/n): ";
            char next;
            cin >> next;
            if (next != 'y' && next != 'Y')
                  break;
            page++;
      }
}

//  DUPLICATE FLAGS
void AVLSystem::resetDuplicateFlags(AVLNode *n)
{
      if (n == nullptr)
            return;
      n->isDuplicate = false;
      resetDuplicateFlags(n->left);
      resetDuplicateFlags(n->right);
}

int AVLSystem::countDuplicateNodes(AVLNode *n) const
{
      if (n == nullptr)
            return 0;
      return (n->isDuplicate ? 1 : 0) + countDuplicateNodes(n->left) + countDuplicateNodes(n->right);
}

static int miniHash(const string &s, int tableSize)
{
      const int BASE = 31;
      long long h = 0, p = 1;
      for (char c : s)
      {
            h = (h + (c - 'a' + 1) * p) % tableSize;
            p = (p * BASE) % tableSize;
      }
      return (int)((h % tableSize + tableSize) % tableSize);
}

//  DETECT DUPLICATE BY CONTENT
void AVLSystem::detectDuplicateByContent()
{
      duplicateMode = 1;
      resetDuplicateFlags(root);

      ofstream out("output/result.txt", ios::trunc);
      if (!out.is_open())
      {
            cout << "Failed to open output file\n";
            return;
      }

      vector<AVLNode *> all;
      inorder(root, all);

      const int BUCKET = 2003;
      struct CNode
      {
            string key;
            vector<AVLNode *> nodes;
            CNode *next;
      };
      vector<CNode *> cidx(BUCKET, nullptr);

      for (AVLNode *node : all)
      {
            const string &key = node->data.content;
            int ci = miniHash(key, BUCKET);
            CNode *cn = cidx[ci];
            bool found = false;
            while (cn)
            {
                  if (cn->key == key)
                  {
                        cn->nodes.push_back(node);
                        found = true;
                        break;
                  }
                  cn = cn->next;
            }
            if (!found)
            {
                  CNode *nc = new CNode{key, {node}, cidx[ci]};
                  cidx[ci] = nc;
            }
      }

      int group = 1;
      for (int i = 0; i < BUCKET; i++)
      {
            CNode *cn = cidx[i];
            while (cn)
            {
                  if (cn->nodes.size() > 1)
                  {
                        out << "Duplicate Group " << group++ << " (Content)\n\n";
                        for (AVLNode *n : cn->nodes)
                        {
                              out << n->data.id << " | " << n->data.name << " | " << n->data.content << "\n";
                              n->isDuplicate = true;
                        }
                        out << "\n";
                  }
                  CNode *tmp = cn;
                  cn = cn->next;
                  delete tmp;
            }
            cidx[i] = nullptr;
      }

      out.close();
      cout << "Duplicate detection finished. Result saved to output/result.txt\n";
}

//  DETECT DUPLICATE BY METADATA  (name + size)
void AVLSystem::detectDuplicateByMetadata()
{
      duplicateMode = 2;
      resetDuplicateFlags(root);

      ofstream out("output/result.txt", ios::trunc);
      if (!out.is_open())
      {
            cout << "Failed to open output file\n";
            return;
      }

      vector<AVLNode *> all;
      inorder(root, all);

      const int BUCKET = 2003;
      struct MNode
      {
            string key;
            vector<AVLNode *> nodes;
            MNode *next;
      };
      vector<MNode *> midx(BUCKET, nullptr);

      for (AVLNode *node : all)
      {
            string key = node->data.name + "|" + to_string(node->data.size);
            int ci = miniHash(key, BUCKET);
            MNode *mn = midx[ci];
            bool found = false;
            while (mn)
            {
                  if (mn->key == key)
                  {
                        mn->nodes.push_back(node);
                        found = true;
                        break;
                  }
                  mn = mn->next;
            }
            if (!found)
            {
                  MNode *nm = new MNode{key, {node}, midx[ci]};
                  midx[ci] = nm;
            }
      }

      int group = 1;
      for (int i = 0; i < BUCKET; i++)
      {
            MNode *mn = midx[i];
            while (mn)
            {
                  if (mn->nodes.size() > 1)
                  {
                        out << "Duplicate Group " << group++ << " (Metadata)\n\n";
                        for (AVLNode *n : mn->nodes)
                        {
                              out << n->data.id << " | " << n->data.name << " | " << n->data.size << "\n";
                              n->isDuplicate = true;
                        }
                        out << "\n";
                  }
                  MNode *tmp = mn;
                  mn = mn->next;
                  delete tmp;
            }
            midx[i] = nullptr;
      }

      out.close();
      cout << "Duplicate detection finished. Result saved to output/result.txt\n";
}

//  DETECT DUPLICATE BY FULL DATA
void AVLSystem::detectDuplicateByFullData()
{
      duplicateMode = 3;
      resetDuplicateFlags(root);

      ofstream out("output/result.txt", ios::trunc);
      if (!out.is_open())
      {
            cout << "Failed to open output file\n";
            return;
      }

      vector<AVLNode *> all;
      inorder(root, all);

      const int BUCKET = 2003;
      struct FNode
      {
            string key;
            vector<AVLNode *> nodes;
            FNode *next;
      };
      vector<FNode *> fidx(BUCKET, nullptr);

      for (AVLNode *node : all)
      {
            const Data &d = node->data;
            string key = d.id + "|" + d.name + "|" + to_string(d.size) + "|" + d.upload_date + "|" + d.source + "|" + d.content;
            int ci = miniHash(key, BUCKET);
            FNode *fn = fidx[ci];
            bool found = false;
            while (fn)
            {
                  if (fn->key == key)
                  {
                        fn->nodes.push_back(node);
                        found = true;
                        break;
                  }
                  fn = fn->next;
            }
            if (!found)
            {
                  FNode *nf = new FNode{key, {node}, fidx[ci]};
                  fidx[ci] = nf;
            }
      }

      int group = 1;
      for (int i = 0; i < BUCKET; i++)
      {
            FNode *fn = fidx[i];
            while (fn)
            {
                  if (fn->nodes.size() > 1)
                  {
                        out << "Duplicate Group " << group++ << " (Full Data)\n\n";
                        for (AVLNode *n : fn->nodes)
                        {
                              const Data &d = n->data;
                              out << d.id << " | " << d.name << " | " << d.size << " | "
                                  << d.upload_date << " | " << d.source << " | " << d.content << "\n";
                              n->isDuplicate = true;
                        }
                        out << "\n";
                  }
                  FNode *tmp = fn;
                  fn = fn->next;
                  delete tmp;
            }
            fidx[i] = nullptr;
      }

      out.close();
      cout << "Duplicate detection finished. Result saved to output/result.txt\n";
}

//  PRINT DUPLICATES  (paginated, groups by current duplicateMode)
void AVLSystem::printDuplicates()
{
      if (duplicateMode == 0)
      {
            cout << "No duplicate data found. Run duplicate detection first." << endl;
            return;
      }

      vector<AVLNode *> all;
      inorder(root, all);

      // Group flagged nodes by their duplicate key
      vector<vector<AVLNode *>> groups;
      vector<AVLNode *> processed;

      for (AVLNode *hn : all)
      {
            if (!hn->isDuplicate)
                  continue;

            bool seen = false;
            for (AVLNode *p : processed)
                  if (p == hn)
                  {
                        seen = true;
                        break;
                  }
            if (seen)
                  continue;

            vector<AVLNode *> group;
            group.push_back(hn);
            processed.push_back(hn);

            for (AVLNode *other : all)
            {
                  if (!other->isDuplicate || other == hn)
                        continue;
                  bool alreadyIn = false;
                  for (AVLNode *p : processed)
                        if (p == other)
                        {
                              alreadyIn = true;
                              break;
                        }
                  if (alreadyIn)
                        continue;

                  bool match = false;
                  if (duplicateMode == 1)
                        match = (hn->data.content == other->data.content);
                  else if (duplicateMode == 2)
                        match = (hn->data.name == other->data.name && hn->data.size == other->data.size);
                  else if (duplicateMode == 3)
                        match = (hn->data.id == other->data.id &&
                                 hn->data.name == other->data.name &&
                                 hn->data.size == other->data.size &&
                                 hn->data.upload_date == other->data.upload_date &&
                                 hn->data.source == other->data.source &&
                                 hn->data.content == other->data.content);
                  if (match)
                  {
                        group.push_back(other);
                        processed.push_back(other);
                  }
            }

            if (group.size() > 1)
                  groups.push_back(group);
      }

      if (groups.empty())
      {
            cout << "No duplicate data found. Run duplicate detection first." << endl;
            return;
      }

      const int pageSize = 20;
      int shown = 0, groupNumber = 1;

      for (size_t g = 0; g < groups.size(); g++)
      {
            cout << "\nDuplicate Group " << groupNumber++;
            if (duplicateMode == 1)
                  cout << " (Content)";
            else if (duplicateMode == 2)
                  cout << " (Metadata)";
            else if (duplicateMode == 3)
                  cout << " (Full Data)";
            cout << endl;

            printTableHeader();

            for (size_t i = 0; i < groups[g].size(); i++)
            {
                  printRow(shown + 1, groups[g][i]->data);
                  shown++;

                  if (shown % pageSize == 0)
                  {
                        cout << "+-----------------------------------------------------------------------------------------------------------------------------+" << endl;
                        cout << "Showing duplicate data " << shown << " so far. Next page? (y/n): ";
                        char next;
                        cin >> next;
                        if (next != 'y' && next != 'Y')
                              return;
                        printTableHeader();
                  }
            }
            cout << "+-----------------------------------------------------------------------------------------------------------------------------+" << endl;
      }
}

//  COUNT
int AVLSystem::count() const
{
      return totalNodes;
}

int AVLSystem::countDuplicate() const
{
      return countDuplicateNodes(root);
}

//  STATISTICS
void AVLSystem::showStatistics(long insertTime, long searchTime,
                               long deleteTime, long showTime, long detectTime)
{
      int total = count();
      int duplicate = countDuplicate();

      cout << endl;
      cout << "Total Data              : " << total << endl;
      cout << "Duplicate Data          : " << duplicate << endl;
      cout << "Unique Data             : " << total - duplicate << endl;
      cout << endl;
      cout << "Execution Time Statistics (ms)" << endl;
      cout << "Insert Time             : " << insertTime << endl;
      cout << "Search Time             : " << searchTime << endl;
      cout << "Delete Time             : " << deleteTime << endl;
      cout << "Show Time               : " << showTime << endl;
      cout << "Duplicate Detection Time: " << detectTime << endl;
}