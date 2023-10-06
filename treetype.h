#ifndef _TREE_TYPES_H_
#define _TREE_TYPES_H_

#define QueueMax 10000

struct biTreeNode;

struct element{
	int val, pos, count;
	struct biTreeNode* pnode;
};
typedef struct element TreeNode,*pTreeNode;

struct biTreeNode{
	struct element* pdata;
	struct biTreeNode *left,*right; 
};
typedef struct biTreeNode BiTree,*pBiTree;

typedef struct{
	pBiTree data[QueueMax];
	int head;
	int rear;
	int len;
}Queue;

#endif
