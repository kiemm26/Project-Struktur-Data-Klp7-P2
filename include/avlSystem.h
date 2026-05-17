#ifndef AVLSYSTEM_H
#define AVLSYSTEM_H

#include <string>
#include <vector>
#include "data.h"

using namespace std;

struct AVLNode
{
      Data data;
      bool isDuplicate;
      int height; // height subtree yang di-root oleh node ini
      AVLNode *left;
      AVLNode *right;
};

class AVLSystem
{
private:
      AVLNode *root;
      int totalNodes;
      int duplicateMode; // 0=none 1=content 2=metadata 3=full

      // ── Internal tree helpers ───────────────────────────────────────────────
      int height(AVLNode *n) const;
      int balanceFactor(AVLNode *n) const;
      void updateHeight(AVLNode *n);

      // Rotations
      AVLNode *rotateRight(AVLNode *y);     // Right rotation  (LL case)
      AVLNode *rotateLeft(AVLNode *x);      // Left rotation   (RR case)
      AVLNode *rotateLeftRight(AVLNode *n); // Left-Right      (LR case)
      AVLNode *rotateRightLeft(AVLNode *n); // Right-Left      (RL case)
      AVLNode *rebalance(AVLNode *n);       // pick & apply correct rotation

      AVLNode *insertNode(AVLNode *n, Data d);
      AVLNode *deleteNode(AVLNode *n, const string &id, bool &deleted);
      AVLNode *minValueNode(AVLNode *n) const; // leftmost node (in-order successor helper)

      // ── Traversal helpers ──────────────────────────────────────────────────
      void inorder(AVLNode *n, vector<AVLNode *> &out) const;

      // ── Duplicate detection internals ─────────────────────────────────────
      void resetDuplicateFlags(AVLNode *n);
      int countDuplicateNodes(AVLNode *n) const;

      // ── Output / formatting helpers ────────────────────────────────────────
      void printTableHeader() const;
      void printRow(int rowNum, const Data &d) const;

      // ── File output helpers ────────────────────────────────────────────────
      void writeGroupContent(ofstream &out, vector<AVLNode *> &group, int groupNum);
      void writeGroupMetadata(ofstream &out, vector<AVLNode *> &group, int groupNum);
      void writeGroupFull(ofstream &out, vector<AVLNode *> &group, int groupNum);

      // ── Memory cleanup ─────────────────────────────────────────────────────
      void destroyTree(AVLNode *n);

public:
      AVLSystem();
      ~AVLSystem();

      void insert(Data d);

      AVLNode *searchByID(const string &id);
      AVLNode *searchByName(const string &name);
      AVLNode *searchByIDAndName(const string &id, const string &name);
      void deleteByID(const string &id);

      void printAll();
      void printDuplicates();

      void detectDuplicateByContent();
      void detectDuplicateByMetadata();
      void detectDuplicateByFullData();

      int count() const;
      int countDuplicate() const;
      void showStatistics(long insertTime, long searchTime,
                          long deleteTime, long showTime, long detectTime);
};

#endif