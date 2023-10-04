#include <windows.h>
#include <stdio.h>
#include "tree.h"
#include "queue.h"

pBiTree AddNode(pTreeNode pdata, int pos, pBiTree pnode){
	if(pnode == NULL)return CreateNode(pdata,pos);
	if(pdata->val == pnode->pdata->val){
		pnode->pdata->count++;
		pdata->pos=pnode->pdata->pos;
		return pnode;
	}
	if(pdata->val < pnode->pdata->val){
		if(pnode->left == NULL){
			pnode->left = CreateNode(pdata,pos*2);
			return pnode->left;
		}else{
			return AddNode(pdata,pos*2,pnode->left);
		}
	}else{
		if(pnode->right == NULL){
			pnode->right=CreateNode(pdata,pos*2+1);
			return pnode->right;
		}else{
			return AddNode(pdata,pos*2+1,pnode->right);
		}
	}
}
pBiTree CreateNode(pTreeNode pdata, int pos){
	pBiTree pnode = (pBiTree)malloc(sizeof(BiTree));
	pdata->count = 1;
	pdata->pos = pos;
	pdata->pnode = pnode;
	pnode->pdata = pdata;
	pnode->left = pnode->right = NULL;
	return pnode;
}

VOID DestroyTree(pBiTree pnode){
	if(pnode){
		DestroyTree(pnode->left);
		DestroyTree(pnode->right);
		free(pnode);pnode=NULL;
	}
}
VOID DrawBiTreeAllNodes(HWND hwnd, pTreeNode* pNode, int size, int depth){
	// pos--->(s,d)--->(x,y)
	// display 6 layers of the tree
	char buf[100];
	RECT rect;
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
	HFONT hFont = CreateFont(-8,-4,0,0,0,0,0,0,0,0,0,0,0,0);
	GetClientRect(hwnd, &rect);
	HDC hdc = GetDC(hwnd);
	SelectObject(hdc,hFont);
	SelectObject(hdc,hPen);
	SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
	float dx,dy;
	int x, y, ss,s,d;
	dy=(rect.bottom - rect.top)*1.0/8;	
	for(int i=0;i<size;i++){
		d = msb(pNode[i]->pos);
		if(d<6){
			ss=1<<d; 
			s=pNode[i]->pos-ss;
			dx=(rect.right-rect.left)*1.0/ss;
			x=(s+0.5)*dx; y=20+(d+1)*dy;
			Ellipse( hdc, x-15, y-15, x+15, y+15 );
			if(pNode[i]->count){
				if(pNode[i]->count>1){
					sprintf(buf,"%dx%d",pNode[i]->val,pNode[i]->count);
					TextOut(hdc,x-12,y-5,buf,strlen(buf));
				}else{
					sprintf(buf,"%d",pNode[i]->val);
					TextOut(hdc,x-8,y-5,buf,strlen(buf));
				}				
			}
			if(d>0){
				MoveToEx(hdc,x,y-15,NULL);
				LineTo(hdc,(2*(s/2+1)-1)*dx,y-dy+15);
			}
		}
	}
	ReleaseDC(hwnd,hdc);
	DeleteObject(hPen);
}
void DrawBiTree(HWND hwnd, pBiTree pnode,int pos){
	if(pnode){
		DrawBiTree(hwnd,pnode->left,pos*2);
		DrawBiTreeNode(hwnd,pnode,pos);
		pnode->pdata->pos=pos;
		DrawBiTree(hwnd,pnode->right,pos*2+1);

	}
}

VOID DrawBiTreeNode(HWND hwnd, pBiTree pnode,int pos){
	char buf[100];
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
	HFONT hFont = CreateFont(-8,-4,0,0,0,0,0,0,0,0,0,0,0,0);	
	RECT rect;
	GetClientRect(hwnd, &rect);
	HDC hdc=GetDC(hwnd);
	SelectObject(hdc,hFont);
	SelectObject(hdc,hPen);
	SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
	float dx,dy;
	int x, y, ss,s,d;
	dy=(rect.bottom - rect.top)*1.0/8;	
	d=msb(pos);
	if(d<6){
		ss=1<<d;
		s=pos-ss;
		dx=(rect.right-rect.left)*1.0/ss;
		x=(s+0.5)*dx+2; y=20+(d+1)*dy;
		Ellipse( hdc, x-15, y-15, x+15, y+15 );
		if(pnode->pdata->count){
			if(pnode->pdata->count>1){
				sprintf(buf,"%dx%d",pnode->pdata->val,pnode->pdata->count);
				TextOut(hdc,x-12,y-5,buf,strlen(buf));
			}else{
			sprintf(buf,"%d",pnode->pdata->val);
			TextOut(hdc,x-8,y-5,buf,strlen(buf));
			}				
		}
		if(d>0){
			MoveToEx(hdc,x,y-15,NULL);
			LineTo(hdc,(2*(s/2+1)-1)*dx,y-dy+15);
		}
	}
	DeleteObject(hPen);
	DeleteObject(hFont);
	ReleaseDC(hwnd, hdc);
}
void DrawTreeBackground(HWND hwnd){
	RECT rect;
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(192,240,255));
	GetClientRect(hwnd,&rect);
	HDC hdc = GetDC(hwnd);
	SelectObject(hdc,hPen);
	SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
	float x, y, dx, dy;
	int ss;
	dy=(rect.bottom-rect.top)/8.0;
	for (int d=0;d<6;d++){
		int dep=6-d;
		y=20+dy*dep;
		ss=1<<dep;
		dx=(rect.right-rect.left)*1.0/ss;
		for(int s=1;s<ss;s+=2){
			x=s*dx;
			//Rectangle(hdc,x-dx/2,y-dy/2,x+dx/2,y+dy/2);
			Ellipse(hdc,x-15,y-15,x+15,y+15);
			if(dep-1>0){
				MoveToEx(hdc,x,y-15,NULL);
				LineTo(hdc,(2*(2*(s/4+1)-1))*dx,y-dy+15);
			}
		}
	}
	ReleaseDC(hwnd,hdc);
}
int* GetBitPath(int pos,int *pathlen){
	int bit,d=msb(pos)+1;*pathlen=d;
	int *path=malloc(d*sizeof(int));
	path[0]=1;
	for(int i=1;i<d;i++){
		int mask=1<<(d-1-i);
		bit=(pos & mask)>>(d-1-i);
		path[i]=bit;		
	}
	return path;
}
int getTreeDepth(pBiTree pnode){
	int rightdep=0;
	int leftdep=0;
	if(pnode==NULL)return -1;
	if(pnode->left!=NULL)
		leftdep=getTreeDepth(pnode->left);
	else
		leftdep=-1;
	if(pnode->right!=NULL)
		rightdep=getTreeDepth(pnode->right);
	else
		rightdep=-1;
	return (rightdep>leftdep)?rightdep+1:leftdep+1;
}
int InitTree(pBiTree *T, TreeNode **Node, pTreeNode **pNode, int **nums, int *size){
	*size = rand()%30+1;
	*nums = (int*)malloc(*size * sizeof(int));
	memset(*nums,0,*size * sizeof(int));
	for(int i=0;i<*size;i++)
	{
		(*nums)[i] = rand()%50;
	}
	*Node = (TreeNode*)malloc(*size*sizeof(TreeNode));
	memset(*Node,0,*size * sizeof(TreeNode));
	for(int i=0;i<*size;i++){
		(*Node)[i].val=(*nums)[i];
	}
	*pNode = (pTreeNode*)malloc(*size * sizeof(pTreeNode));
	memset(*pNode,0,*size * sizeof(pTreeNode));

	for(int i=0;i<*size;i++){
		(*pNode)[i]=&(*Node)[i];
	}
	//int l=0;
	*T = (*Node)[0].pnode = CreateNode((*pNode)[0],1); 
	//(*pNode)[l]=&(*Node)[0]; 
	for(int i=1;i<*size;i++){
		(*Node)[i].pnode=AddNode((*pNode)[i], 1, *T);
		//if((*Node)[i].count){
		//	l++;(*pNode)[l]=&(*Node)[i];
		//}
	}
	
	return getTreeDepth(*T);
}
void InsertDataToTree(pBiTree T,int value, pTreeNode *pData, int *treeSize){
	//char buf[100];
	pBiTree pnode = SearchTreeNode(T,value);
	if(pnode==NULL){
		//make a new node
		(*treeSize)++;
		pTreeNode *pNewData=(pTreeNode*)malloc(*treeSize*sizeof(pTreeNode));
		memcpy(pNewData,pData,(*treeSize-1)*sizeof(pTreeNode));
		free(pData);
		pNewData[*treeSize-1]=(pTreeNode)malloc(sizeof(TreeNode));
		pNewData[*treeSize-1]->val=value;
		pData=pNewData;
		MessageBox(NULL,"here is OK","OKOK",MB_OK);
		AddNode(pNewData[*treeSize-1], 1, T);
	}else{
		pnode->pdata->count++;
	}
}
unsigned msb(unsigned x){
	return 31-__builtin_clz(x);
}

void postList(pBiTree pnode, int* arr, int* count){
	if(pnode){		
		postList(pnode->left, arr, count);
		postList(pnode->right, arr, count);
		arr[(*count)++] = pnode->pdata->val;
	}
}
int *postOrderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	postList(root, arr, &count);
	*returnSize = count;
	return arr;
}
void innerList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		innerList(pnode->left, arr, count);
		arr[(*count)++] = pnode->pdata->val;
		innerList(pnode->right, arr, count);
	}
}
int *innerOrderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	innerList(root, arr, &count);
	*returnSize = count;
	return arr;
}
void preList(pBiTree pnode, int* arr, int* count){
	if(pnode){
		arr[(*count)++] = pnode->pdata->val;
		preList(pnode->left, arr, count);
		preList(pnode->right, arr, count);
	}
}
int *preOrderTraversal(pBiTree root, int* returnSize) {
	int* arr = (int *)malloc(sizeof(int) * 100);
	int count = 0;
	preList(root, arr, &count);
	*returnSize = count;
	return arr;
}
int *layerOrderTraversal(pBiTree pnode,int *returnSize){
	int* arr = (int *)malloc(sizeof(int) * 200);
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
			if(tmp->left)PushQueue(&seq,tmp->left);
			if(tmp->right)PushQueue(&seq,tmp->right); 
			PopQueue(&seq,&tmp);
		}
		level++; 
	}
	*returnSize=count;
	return arr;
}
VOID RemoveTreeNode(HWND hwnd, pBiTree *T, int value, pTreeNode *pData,int *treeSize)
{
	HDC hdc;
	int *path,pathlen,dir,i;	
	char buf[100];
	pBiTree qnode,p,q;
	pBiTree pnode = SearchTreeNode(*T,value);
	hdc=GetDC(hwnd);
	p=q=qnode=*T;
	if(pnode!=NULL){
		if(pnode->pdata->count>1){
			pnode->pdata->count--;
		}else{
			//go to pnode and get the father.
			path=GetBitPath(pnode->pdata->pos,&pathlen);
			for( i=1;i<pathlen-1;i++){
				switch(path[i]){
					case 0:qnode=qnode->left;break;
					case 1:qnode=qnode->right;break;
				}
				sprintf(buf,"%d %d",path[i],qnode->pdata->val);
				TextOut(hdc,350,30+20*i,buf,strlen(buf));
			}
			if(qnode!=pnode){
				// Delete non-root node
				dir=path[pathlen-1];
				if(pnode->left == NULL && pnode->right == NULL){
					if(dir==1){
						qnode->right=NULL;
					}else{
						qnode->left=NULL;
					}
				}else if(pnode->left == NULL && pnode->right != NULL){
					if(dir==1){
						qnode->right=pnode->right;
					}else{
						qnode->left=pnode->right;
					}
				}else if(pnode->left != NULL && pnode->right == NULL){
					if(dir==1){
						qnode->right=pnode->left;
					}else{
						qnode->left=pnode->left;
					}
				}else if(pnode->left != NULL && pnode->right != NULL){
					q=pnode;
					p=pnode->left;
					while(p->right!=NULL){
						q=p;p=p->right;
					}
					p->right=pnode->right;
					if(q!=pnode){
						p->left=pnode->left;
						q->right=NULL;
					}
					if(dir==1){
						qnode->right=p;
					}else{
						qnode->left=p;
					}				
				}
				pnode->left=NULL;
				pnode->right=NULL;
				free(pnode);
			}else{//delete root
				if(pnode->left==NULL){
					*T=pnode->right;
					pnode->right=NULL;
				}else{
					if(pnode->right==NULL){
						*T=pnode->left;
						pnode->left=NULL;
					}else{
						p=pnode->left;q=pnode;
						while(p->right!=NULL){
							q=p; p=p->right;
						}
						p->right=pnode->right;
						if(q!=pnode){
							q->right=p->left;
							p->left=pnode->left;
							q->right=NULL;
						}
						*T=p;
					}
				}
			}
			free(path);		
		}
	}
	ReleaseDC(hwnd, hdc);
}
pBiTree SearchTreeNode(pBiTree T,int value)
{
    if ( T != NULL){
		if (T->pdata->val == value)
		{
		    return T;
		}else if (T->pdata->val > value)
		{
		    T=SearchTreeNode(T->left,value);  
		}else if (T->pdata->val < value)
		{
		    T=SearchTreeNode(T->right,value); 
		}
    }
   	return T;
}
POINT* ShowPath(HWND hwnd, int pos,int *pathlen){
	//Show the path from root to the node
	HDC hdc = GetDC(hwnd);
	HPEN hPen = CreatePen(PS_SOLID,3,RGB(0,0,255));
	SelectObject(hdc,hPen);
	RECT rect;
	GetClientRect(hwnd, &rect);
	float x,y,dx,dy;
	int s,bit,ss,d=msb(pos)+1;*pathlen=d;
	POINT *path=malloc(d*sizeof(POINT));
	dy=(rect.bottom-rect.top)/8.0;
	dx=(rect.right-rect.left)/2.0;
	ss=1<<d;
	s=0;x=(s+1)*dx;y=20+dy;
	pos=pos-ss;path[0].x=dx;path[0].y=y;
	for(int i=1;i<d;i++){
		int mask=1<<(d-1-i);
		bit=(pos & mask)>>(d-1-i);		
		MoveToEx(hdc,x,y+15,NULL);
		dx/=2.0;
		s=(s + bit)*2;
		x=(s+1)*dx;
		y+=dy;
		LineTo(hdc,x,y-15);
		path[i].x=x;path[i].y=y;
		Sleep(300);
	}
	DeleteObject(hPen);	
	ReleaseDC(hwnd, hdc);
	return path;
}
