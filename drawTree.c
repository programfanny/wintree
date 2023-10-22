// drawtree.c
#include <windows.h>
#include <stdio.h>
#include "treetype.h"
#include "tree.h"

VOID SearchBiTree(pBiTree T,int vpos,int pos, pBiTree *pnode){
	if(T){
		if(vpos == pos){*pnode = T; return; }
		SearchBiTree(T->lchild,vpos*2,pos,pnode);
		SearchBiTree(T->lchild,vpos*2+1,pos,pnode);
	}
}
void SearchTree(pBiTree T, int pos, pBiTree *pnode){
	*pnode=NULL;
	SearchBiTree(T, 1, pos, pnode);
}
VOID DispArray(HDC hdc,int xpos, int ypos,const char *title, int *arr,int arrSize){
	char buf[100];
	TextOut(hdc,xpos,ypos,title,strlen(title));
	for(int i=0;i<arrSize;i++){
		sprintf(buf,"%d ",arr[i]);
		TextOut(hdc,xpos+5,ypos+10+i*10,buf,strlen(buf));
	}
}
VOID DrawTreeBackground(HWND hwnd, HPEN *hPen,int n, HFONT hFont){
	RECT rect;
	char buf[100];
	GetClientRect(hwnd,&rect);
	HDC hdc = GetDC(hwnd);
	SelectObject(hdc,hFont);
	SelectObject(hdc,hPen[n]);
	SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
	float x, y, dx, dy;
	int ss;
	dy=(rect.bottom-rect.top)/8.0;
	for (int d=0;d<6;d++){
		int dep=6-d;
		y=dy*dep;
		ss=1<<dep;
		dx=(rect.right-rect.left-30)*1.0/ss;
		for(int s=1;s<ss;s+=2){
			x=s*dx;
			//Rectangle(hdc,x-dx/2,y-dy/2,x+dx/2,y+dy/2);
			Ellipse(hdc,x-15,y-15,x+15,y+15);
			sprintf(buf,"%d",(ss+s)/2);
			TextOut(hdc,x+12,y-15,buf,strlen(buf));		
			if(dep-1>0){
				MoveToEx(hdc,x,y-15,NULL);
				LineTo(hdc,(2*(2*(s/4+1)-1))*dx,y-dy+15);
			}
		}
	}
	ReleaseDC(hwnd,hdc);
}
// pnode ＝ 起点 pos ＝ 路径 返回 不在树中的空结点或到达的点
pBiTree GoThrough(pBiTree pnode, int pos){
	int j=msb(pos);
	pBiTree qnode;
	while(pnode){
		qnode=pnode;
		//DrawNode(pnode);
		if(j==0)break;
		if(pos & (1<<(j-1))){
			pnode=pnode->rchild;
		}else{
			pnode=pnode->lchild;
		}
		//DrawNode(pnode);
		j--;
	}
	return pnode?qnode:NULL;
}

DWORD PosXY(RECT rect,int window,int pos){
	WORD x,y;
	DWORD XY;
	float dx,dy;
	int width=rect.right-rect.left-30;
	int height=rect.bottom-rect.top;
	int layers=8;
	int d,s,ss;
	if(window==0){layers=20;width=width/4;height=height/4;}
	dy=height*1.0/layers;
	d=msb(pos);
	ss=1<<d;
	dx=width*1.0/ss;
	s=pos-ss;
	x=(int)((s+0.5)*dx);
	y=(int)((1+d)*dy);
	XY=(x<<16)|y;
	return XY;
}
DWORD PosBox(RECT rect,int window,int pos){
	WORD x,y;
	DWORD XY;
	float dx,dy;
	int width=rect.right-rect.left-30;
	int height=rect.bottom-rect.top;
	int layers=8;
	int d,s,ss,ux,uy;
	if(window==0){layers=20;width=width/4;height=height/4;}
	dy=height*1.0/layers;
	d=msb(pos);
	ss=1<<d;
	dx=width*1.0/ss;
	ux=(int)dx;
	uy=(int)dy;
	return (ux<<16)|uy;
}
VOID LineBetween(HWND hwnd, HPEN *hPen,int n, int x, int y,int u, int v,int window){
	HDC hdc=GetDC(hwnd);
	SelectObject(hdc, hPen[n]);
	int d=0;
	if(window)d=15;
	MoveToEx(hdc,x,y+d,NULL);
	LineTo(hdc,u,v-d);
	ReleaseDC(hwnd, hdc);
}
VOID DrawLine(HWND hwnd, HPEN *hPen,int n, RECT rect,int window, int curPos,int mpos){
	int pos;
	if(window==0)pos=(curPos<<msb(mpos)) | (mpos^(1<<msb(mpos)));
	else pos=mpos;
	if(window==0 ||(window==1 && (msb(pos)<6))){
		DWORD xy= PosXY( rect, window, pos/2 );
		DWORD uv= PosXY( rect, window, pos );
		LineBetween(hwnd,hPen, n, HIWORD(xy), LOWORD(xy),HIWORD(uv), LOWORD(uv),window);	
	}
}
VOID ShowNode(HWND hwnd, HPEN *hPen,int n,HFONT hFont, int x, int y, pBiTree T, int window, int Tpos,int vpos){
	HDC hdc=GetDC(hwnd);
	char buf[100];
	SelectObject(hdc, hPen[n]);
	SelectObject(hdc,hFont);
	SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
	int r=1;
	if(window==1){r=14;}
	Ellipse( hdc, x-r, y-r, x+r, y+r );
	if(window){
		//sprintf(buf,"%d",vpos);
		//TextOut(hdc,x+r,y-r,buf,strlen(buf));
		sprintf(buf,"%d",T->data);
		TextOut(hdc,x-r/2,y-r*2/3,buf,strlen(buf));
		if(1<T->pdata->count){
			sprintf(buf,"x%d",T->pdata->count);
			TextOut(hdc,x-r/3,y,buf,strlen(buf));
		}
	}
	ReleaseDC(hwnd, hdc);
}
//VOID DrawTreeNode(HWND hwnd, HFONT *hPen,int n,HFONT hFont, RECT rect, int window, pBiTree T, int Tpos,int vpos){
//	if(window==0 ||(window==1 && (msb(vpos)<6))){
//		DWORD xy;
//		xy=PosXY( rect, window, vpos );
//		ShowNode( hwnd,hPen,n,hFont, HIWORD(xy), LOWORD(xy), T, window, Tpos, vpos);
//	}
//}
VOID DrawNode(HWND hwnd, HPEN *hPen,int n,HFONT hFont,RECT rect,int window, pBiTree T, int Tpos,int vpos){
	if(vpos>1){	
		int pos;
		if(window==0)pos=(Tpos<<msb(vpos)) | (vpos^(1<<msb(vpos)));
		else pos=vpos;
		if(window==0 ||(window==1 && (msb(pos)<6))){
			DWORD xy= PosXY( rect, window, pos/2 );
			DWORD uv= PosXY( rect, window, pos );
			LineBetween(hwnd,hPen, n, HIWORD(xy), LOWORD(xy),HIWORD(uv), LOWORD(uv),window);	
		}
	}
	if(window==0 ||(window==1 && (msb(vpos)<6))){
		DWORD xy;
		xy=PosXY( rect, window, vpos );
		ShowNode( hwnd,hPen,n,hFont, HIWORD(xy), LOWORD(xy), T, window, Tpos, vpos);
	}
}
VOID DrawTree(HWND hwnd, HPEN *hPen,int n,HFONT hFont,RECT rect,int window, pBiTree T, int Tpos,int vpos){
	if(T){
		DrawTree(hwnd,hPen,n,hFont,rect, window, T->lchild, Tpos, vpos*2);
		DrawTree(hwnd,hPen,n,hFont,rect, window, T->rchild, Tpos, vpos*2+1);
		DrawNode(hwnd,hPen,n,hFont, rect, window, T, Tpos, vpos);
	}
}
VOID DrawBiTree(HWND hwnd, HPEN *hPen,int n,HFONT hFont,RECT rect,int window, pBiTree T, int Tpos){
	int vpos=Tpos;
	if(window==1)vpos=1;
	DrawTree(hwnd,hPen,n,hFont,rect, window, T, Tpos, vpos);
}

VOID DrawPath(HWND hwnd,HPEN* hPen, int n,RECT rect,int window,int mpos){
	DWORD XY,xy,uv;
	int x,y,dx,dy;
	XY=PosXY( rect, window, mpos );
	x=HIWORD(XY); y=LOWORD(XY);
	XY=PosBox( rect, window, mpos );
	dx=HIWORD(XY); dy=LOWORD(XY);
	int j=msb(mpos);
	int pos=1;
	while(j>0){
		if(mpos & (1<<(j-1))){
			xy= PosXY( rect, window, pos );
			uv= PosXY( rect, window, pos*2+1 );
			pos=pos*2+1;
			LineBetween(hwnd, hPen, n, HIWORD(xy), LOWORD(xy),HIWORD(uv), LOWORD(uv),window);	
		}else{
			xy= PosXY( rect, window, pos );
			uv= PosXY( rect, window, pos*2 );
			pos=pos*2;
			LineBetween(hwnd, hPen, n, HIWORD(xy), LOWORD(xy),HIWORD(uv), LOWORD(uv),window);	
		}
		j--;
	}	
}

