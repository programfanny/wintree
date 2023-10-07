#ifndef _TREE_H_
#define _TREE_H_

#include <windows.h>
#include "treetype.h"

pBiTree AddNode(pBiTree pnode,int value,int pos, pTreeNode **pData,int *TreeSize);
pBiTree CreateNode(int value,int pos, pTreeNode **pData,int *TreeSize);
VOID DestroyTree(pBiTree pnode);
VOID DrawBiTree(HWND hwnd, pBiTree pnode,int vpos,pTreeNode *pData,int TreeNodeCount);
void DrawSubTree(HWND hwnd, pBiTree pnode,int vpos);
VOID DrawBiTreeNode(HWND hwnd, pBiTree pnode,int pos);
VOID DrawTreeMap(HWND hwnd, pBiTree pnode,pBiTree qnode,int pos);
void DrawTreeBackground(HWND hwnd);

int* GetBitPath(int pos,int *pathlen);
int GetTreeDepth(pBiTree pnode);

VOID InitTree(pBiTree *T, int *size, pTreeNode **pData, int *TreeSize);

unsigned msb(unsigned x);
int *layerOrderTraversal(pBiTree pnode,int *returnSize);
int *postOrderTraversal(pBiTree root, int* returnSize);
int *innerOrderTraversal(pBiTree root, int* returnSize);
int *preOrderTraversal(pBiTree root, int* returnSize);

VOID  RemoveTreeNode(pBiTree *T, int value, pTreeNode **pData,int *size);

pBiTree SearchTreeNode(pBiTree pnode,pBiTree *parentNode,int value,int *pos);

POINT* ShowPath(HWND hwnd, int pos,int *pathlen);

#endif