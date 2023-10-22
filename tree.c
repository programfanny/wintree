#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "treetype.h"

unsigned msb(unsigned x){
	return 31-__builtin_clz(x);
}
int getHeight(pBiTree node){
	return node?node->height:0;
}
int maxint(int a, int b){
	return a>b?a:b;
}
void rrRotation(pBiTree node,pBiTree *root){
	pBiTree temp=node->rchild;
	node->rchild=temp->lchild;
	temp->lchild=node;
	node->height=maxint(getHeight(node->lchild),getHeight(node->rchild))+1;
	temp->height=maxint(getHeight(temp->lchild),getHeight(temp->rchild))+1;
	*root=temp;
}
void llRotation(pBiTree node,pBiTree *root){
	pBiTree temp=node->lchild;
	node->lchild=temp->rchild;
	temp->rchild=node;
	node->height=maxint(getHeight(node->lchild),getHeight(node->rchild))+1;
	temp->height=maxint(getHeight(temp->lchild),getHeight(temp->rchild))+1;
	*root=temp;
}
void avlInsert(pBiTree* T, int data, int *TreeSize){
	if(*T==NULL){
		*T=(BiTree*)malloc(sizeof(BiTree));
		(*T)->pdata=(pTreeNode)malloc(sizeof(TreeNode));
		(*T)->pdata->val=data;
		(*T)->pdata->pnode=*T;
		(*T)->pdata->count=1;
		(*T)->pdata->height=0;

		(*T)->data=data;
		(*T)->height=0;
		(*T)->lchild=NULL;
		(*T)->rchild=NULL;
		(*TreeSize)++;

	}else if( data == (*T)->data ){
		(*T)->pdata->count++;
		return;
	}else if(data<(*T)->data){
		avlInsert(&(*T)->lchild,data,TreeSize);
		int lHeight=getHeight((*T)->lchild);
		int rHeight=getHeight((*T)->rchild);
		if(lHeight-rHeight==2){
			if(data<(*T)->lchild->data){
				llRotation(*T,T);
			}else{
				rrRotation((*T)->lchild,&(*T)->lchild);
				llRotation(*T,T);
			}
		}
	}else if(data>(*T)->data){
		avlInsert(&(*T)->rchild,data,TreeSize);
		int lHeight=getHeight((*T)->lchild);
		int rHeight=getHeight((*T)->rchild);
		if(rHeight-lHeight==2){
			if(data>(*T)->rchild->data){
				//RR
				rrRotation(*T,T);
			}else{
				//RL
				llRotation((*T)->rchild,&(*T)->rchild);
				rrRotation(*T,T);
			}
		}
	}
	(*T)->height=maxint(getHeight((*T)->lchild),getHeight((*T)->rchild))+1;
}
void avlDelete(pBiTree *T, int data, int *TreeSize){
	if(T==NULL)return;
	if(*T==NULL)return;
	if(data > (*T)->data){
		avlDelete(&((*T)->rchild),data,TreeSize);
	}else if(data<(*T)->data){
		avlDelete(&((*T)->lchild),data,TreeSize);
	}else{
		if((*T)->pdata->count>1){
			(*T)->pdata->count--;
			return;
		}
		if((*T)->lchild==NULL && (*T)->rchild==NULL){
			free(*T);
			*T=NULL;
			(*TreeSize)--;
		}else if((*T)->lchild==NULL){
			pBiTree temp=(*T)->rchild;
			free(*T);*T=temp;(*TreeSize)--;
		}else{
			pBiTree temp=(*T)->lchild;
			while(temp->rchild!=NULL){
				temp=temp->rchild;
			}
			(*T)->data=temp->data;
			avlDelete(&((*T)->lchild),temp->data,TreeSize);
		}
	}
	if(*T==NULL)return;

	(*T)->height=maxint(getHeight((*T)->lchild),getHeight((*T)->rchild))+1;
	int lheight=getHeight((*T)->lchild);
	int rheight=getHeight((*T)->rchild);
	if(lheight-rheight==2){
		if(getHeight((*T)->lchild->lchild)-getHeight((*T)->lchild->rchild)==0 
			|| getHeight((*T)->lchild->lchild)-getHeight((*T)->lchild->rchild)==1){
			llRotation(*T,T);
		}else{
			rrRotation((*T)->lchild, &(*T)->lchild);
			llRotation(*T,T);
		}
	}else if(rheight-lheight==2){
		if(getHeight((*T)->rchild->rchild)-getHeight((*T)->rchild->lchild)==0 
			|| getHeight((*T)->rchild->rchild)-getHeight((*T)->rchild->lchild)==1){
			rrRotation(*T,T);
		}else{
			llRotation((*T)->rchild, &(*T)->rchild);
			rrRotation(*T,T);
		}
	}
}
void preOrder(pBiTree T){
	if(T){
		printf("%d ",T->data);
		preOrder(T->lchild);
		preOrder(T->rchild);
	}
}
void innOrder(pBiTree T){
	if(T){
		innOrder(T->lchild);
		printf("%d ",T->data);
		innOrder(T->rchild);
	}
}
void posOrder(pBiTree T){
	if(T){
		posOrder(T->lchild);
		posOrder(T->rchild);
		printf("%d ",T->data);
	}
}
void InitTree(pBiTree *pnode, int **nums, int *numsSize, pTreeNode **pData, int *TreeSize){
	*numsSize = rand()%120+10;
	*nums = (int*)malloc(*numsSize*sizeof(int));
	*TreeSize=0;
	for(int i=0;i<*numsSize;i++){
		(*nums)[i]=rand()%250;
		avlInsert(pnode,(*nums)[i],TreeSize);
	}
}
void DestroyTree(pBiTree T){
	if(T){
		DestroyTree(T->lchild);
		DestroyTree(T->rchild);
		if(T->pdata){
			T->pdata->pnode=NULL;
			free(T->pdata);
			T->pdata=NULL;
		}		
		free(T);
		T=NULL;
	}
}

void postList(pBiTree pnode, int* arr, int* count){
	if(pnode){		
		postList(pnode->lchild, arr, count);
		postList(pnode->rchild, arr, count);
		arr[(*count)++] = pnode->pdata->val;
	}
}
int *postOrderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) *TreeNodeMax);
	int count = 0;
	postList(root, arr, &count);
	*returnSize = count;
	return arr;
}
void innerList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		innerList(pnode->lchild, arr, count);
		arr[(*count)++] = pnode->pdata->val;
		innerList(pnode->rchild, arr, count);
	}
}
int *innerOrderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * TreeNodeMax);
	int count = 0;
	innerList(root, arr, &count);
	*returnSize = count;
	return arr;
}
void preList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		arr[(*count)++] = pnode->pdata->val;
		preList(pnode->lchild, arr, count);
		preList(pnode->rchild, arr, count);
	}
}
int *preOrderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * TreeNodeMax);
	int count = 0;
	preList(root, arr, &count);
	*returnSize = count;
	return arr;
}
int *layerOrderTraversal(pBiTree pnode,int *returnSize){
	int* arr = (int *)malloc(sizeof(int) * TreeNodeMax);
	int count = 0;
	Queue seq;
	seq = InitQueue();
	pBiTree tmp;
	tmp = pnode;
	int len;
	PushQueue(&seq,tmp);
	int level=0;
	while(!IsEmptyQueue(seq)){
		len = seq.len;
		for(int i=0;i<len;i++){
			arr[count*2]=tmp->pdata->val;
			arr[count*2+1]=level;
			count++;
			if(tmp->lchild)PushQueue(&seq,tmp->lchild);
			if(tmp->rchild)PushQueue(&seq,tmp->rchild); 
			PopQueue(&seq,&tmp);
		}
		level++;
	}
	*returnSize=count;
	return arr;
}