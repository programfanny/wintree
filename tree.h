#ifndef _TREE_H_
#define _TREE_H_
#include <windows.h>
#include "treetype.h"

pBiTree AddNode(pTreeNode pdata,int id, pBiTree pnode);
pBiTree CreateNode(pTreeNode pdata,int id);
VOID DestroyTree(pBiTree pnode);

void DrawBiTreeAllNodes(HWND hwnd, pTreeNode* pNode, int size, int depth);

void DrawBiTree(HWND hwnd, pBiTree pnode,int pos);
VOID DrawBiTreeNode(HWND hwnd, pBiTree pnode,int pos);
void DrawTreeBackground(HWND hwnd);

int* GetBitPath(int pos,int *pathlen);
int getTreeDepth(pBiTree pnode);
int InitTree(pBiTree *T, TreeNode **Node, pTreeNode **pNode, int **nums, int *size);
void InsertDataToTree(pBiTree T,int value, pTreeNode *pData,int *treeSize);
unsigned msb(unsigned x);
int *layerOrderTraversal(pBiTree pnode,int *returnSize);
int *postOrderTraversal(pBiTree root, int* returnSize);
int *innerOrderTraversal(pBiTree root, int* returnSize);
int *preOrderTraversal(pBiTree root, int* returnSize);
VOID  RemoveTreeNode(HWND hwnd, pBiTree *T, int value,pTreeNode *pData,int *size);
pBiTree SearchTreeNode(pBiTree T,int value);
POINT* ShowPath(HWND hwnd, int pos,int *pathlen);

#endif