//tree.h
#ifndef _TREE_H_
#define _TREE__H_
#include <windows.h>
#include "treetype.h"


int getHeight(pBiTree node);
int maxint(int a, int b);
void rrRotation(pBiTree node,pBiTree *root);
void llRotation(pBiTree node,pBiTree *root);
void avlInsert(BiTree** T, int data, int *TreeSize);
void avlDelete(pBiTree *T, int data, int *TreeSize);
void preOrder(pBiTree T);
void innOrder(pBiTree T);
void posOrder(pBiTree T);
void InitTree(pBiTree *pnode, int **nums, int *numsSize, pTreeNode **pData, int *TreeSize,int operate);
void DestroyTree(pBiTree T);


int *postOrderTraversal(pBiTree root, int* returnSize);
int *innerOrderTraversal(pBiTree root, int* returnSize);
int *preOrderTraversal(pBiTree root, int* returnSize) ;
int *layerOrderTraversal(pBiTree pnode,int *returnSize);

unsigned msb(unsigned x);

#endif