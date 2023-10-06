#include <windows.h>
#include <stdio.h>
#include "tree.h"
#include "queue.h"

pBiTree AddNode(pBiTree pnode,int value,int pos, pTreeNode **pData,int *TreeSize){
	if(pnode == NULL)return CreateNode(value,pos,pData,TreeSize);
	else if(value == pnode->pdata->val){
		pnode->pdata->count++;	
		return pnode;
	}else if(value < pnode->pdata->val){
		if(pnode->left == NULL){
			pnode->left = CreateNode(value,pos*2,pData,TreeSize);
			return pnode->left;
		}else{
			return AddNode(pnode->left,value,pos*2,pData,TreeSize);
		}
	}else{
		if(pnode->right == NULL){
			pnode->right=CreateNode(value,pos*2+1,pData,TreeSize);
			return pnode->right;
		}else{
			return AddNode(pnode->right,value,pos*2,pData,TreeSize);
		}
	}	
}
pBiTree CreateNode(int value,int pos, pTreeNode **pData,int *TreeSize){
	pBiTree pnode=(pBiTree)malloc(sizeof(BiTree));
	pnode->pdata = (pTreeNode)malloc(sizeof(TreeNode));
	pTreeNode *pNewData=(pTreeNode*)malloc((*TreeSize+1)*sizeof(pTreeNode));
	memcpy(pNewData,*pData,*TreeSize*sizeof(pTreeNode));
	pNewData[*TreeSize]=pnode->pdata;
	free(*pData);
	(*pData)=pNewData;
	(*TreeSize)++;
	pnode->left = pnode->right=NULL;
	pnode->pdata->val = value;
	pnode->pdata->count = 1;
	pnode->pdata->pos = pos;
	pnode->pdata->pnode = pnode;
	return pnode;
}
VOID DestroyTree(pBiTree pnode){
	if(pnode){
		DestroyTree(pnode->left);
		DestroyTree(pnode->right);
		free(pnode);pnode=NULL;
	}
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
	SetBkMode(hdc,TRANSPARENT);
	float dx,dy;
	int x, y, ss,s,d;
	dy=(rect.bottom - rect.top)*1.0/8;	
	d=msb(pos);
	if(d<6){
		ss=1<<d;
		s=pos-ss;
		dx=(rect.right-rect.left)*1.0/ss;
		x=(s+0.5)*dx; y=20+(d+1)*dy;
		Ellipse( hdc, x-15, y-15, x+15, y+15 );
		if(pnode->pdata->count>1){
			sprintf(buf,"%dx%d",pnode->pdata->val,pnode->pdata->count);
			TextOut(hdc,x-12,y-5,buf,strlen(buf));
		}else{
			sprintf(buf,"%d",pnode->pdata->val);
			TextOut(hdc,x-8,y-5,buf,strlen(buf));
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
pBiTree SearchTreeNode( pBiTree pnode, pBiTree *parentNode, int value, int *pos ){
	pBiTree qnode=NULL;
	while(pnode!=NULL){
		if(value==pnode->pdata->val){
			return pnode;
		}else{
			qnode=pnode;
			if(value<pnode->pdata->val){
				*pos=*pos*2;
				pnode=pnode->left;
			}else{
				*pos=*pos*2+1;
				pnode=pnode->right;
			}
		}
	}
	*parentNode = qnode;
	return pnode;
}
VOID InitTree(pBiTree *T, int *size, pTreeNode **pData, int *TreeSize){
	*size = rand()%30+1;
	pBiTree qnode,pnode;
	int pos;
	int value;

	for(int i=0;i<*size;i++){
		value=rand()%50;
		qnode=pnode=*T;
		pos=1;
		pnode=SearchTreeNode(pnode,&qnode,value,&pos);
		if(pnode==NULL){
			pnode = CreateNode(value,pos,pData,TreeSize);
			if(i==0) *T=pnode;
			else if(pos%2) qnode->right=pnode;
				else qnode->left=pnode;			
		}
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
VOID RemoveTreeNode(pBiTree *T, int value, pTreeNode **pData,int *TreeSize)
{
	int *path,pathlen,dir,i,pos=1;	
	pBiTree qnode,p,q;
	pBiTree pnode = SearchTreeNode(*T,&qnode,value,&pos);
	p=q=qnode=*T;
	if(pnode!=NULL){
		if(pnode->pdata->count>1){
			pnode->pdata->count--;
		}else{
			path=GetBitPath(pnode->pdata->pos,&pathlen);
			for( i=1;i<pathlen-1;i++){
				switch(path[i]){
					case 0:qnode=qnode->left;break;
					case 1:qnode=qnode->right;break;
				}
			}
			dir=path[pathlen-1];
			if(pnode->left==NULL){
				p=pnode->right;
			}else if(pnode->right==NULL){
				p=pnode->left;
			}else{
				//Randomly choose the max node in the left tree or the min node in the right tree as p
				if(rand()%2){
					//Replace with the max node in the left tree.
					q=pnode;p=pnode->left;
					while(p->right!=NULL){
						q=p;p=p->right;
					}
					p->right=pnode->right;
					if(q!=pnode){
						q->right=p->left;
						p->left=pnode->left;
					}
				}else{
					//Replace with the min node in the right tree.
					q=pnode;p=pnode->right;
					while(p->left!=NULL){
						q=p;p=p->left;
					}
					p->left=pnode->left;
					if(q!=pnode){
						q->left=p->right;
						p->right=pnode->right;
					}
				}
			}
			if(qnode==pnode){
				*T=p;
			}else if(dir==1){
				qnode->right=p;
			}else {
				qnode->left=p;
			}
			pTreeNode *pNewData=(pTreeNode*)malloc(((*TreeSize)-1)*sizeof(pTreeNode));
			int l=0;
			for(int i=0;i<*TreeSize;i++){
				if(value!=(*pData)[i]->val){
					pNewData[l++]=(*pData)[i];
				}else{
					free(pnode->pdata);
				}
			}
			free(*pData);
			*pData=pNewData;
			(*TreeSize)--;
			pnode->left=NULL;
			pnode->right=NULL;
			free(pnode);
			free(path);	
		}
	}
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


