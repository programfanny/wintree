//winavltree.c
#include <windows.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "tree.h"
#include "drawTree.h"
#include "queue.h"
#include "treeres.h"


INT_PTR CALLBACK TreeNodeDlgProc (HWND, UINT, WPARAM, LPARAM) ;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("HelloWin") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, MAKEINTRESOURCE(IDI_TREE_ICON)) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	hwnd = CreateWindow (szAppName,                 	// window class name
						TEXT ("The Tree Demo"), 	// window caption
						WS_OVERLAPPEDWINDOW,        	// window style
						CW_USEDEFAULT,              	// initial x position
						CW_USEDEFAULT,              	// initial y position
						CW_USEDEFAULT,              	// initial x size
						CW_USEDEFAULT,              	// initial y size
						NULL,                       	// parent window handle
						(HMENU)LoadMenu(hInstance,MAKEINTRESOURCE(IDR_TREE_MENU)),//NULL,                       	// window menu handle
						hInstance,                  	// program instance handle
						NULL) ;                     	// creation parameters
	 
	 ShowWindow (hwnd, iCmdShow) ;
	 UpdateWindow (hwnd) ;
	 
	 while (GetMessage (&msg, NULL, 0, 0))
	 {
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	 }
	 return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc ;
	PAINTSTRUCT ps ;
	char buf[100];
	float dx,dy,x0,y0;
	int mx,my,x,y,XY,tx,ty,s,d,ss,num,value,pos,width,height,layers;
	static POINT *patharr;
	static int pathlen;
	static RECT rect;
	static HINSTANCE hInstance; 
	static int numsSize, *nums, TreeSize, curPos, vpos, mpos;
	static pTreeNode *pData;
	static int *arrA, *arrB, *arrC, *arrD;
	static int returnSizeA = 0,returnSizeB = 0,returnSizeC = 0,returnSizeD = 0;
	static pBiTree root=NULL,pnode,qnode,curT;

	static HPEN hPen[8];
	static HFONT hFont;
	
	static BOOL ti,showtree;
	switch (message)
	{
	case WM_CREATE:
		//PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TREE_ICON));
		if (hIcon)
		{
    		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		}
		srand(time(0));
		root=NULL;

		hPen[0] = CreatePen(PS_SOLID,1,RGB(232,232,255));
		hPen[1] = CreatePen(PS_SOLID,1,RGB(0,0,255));
		hPen[2] = CreatePen(PS_SOLID,1,RGB(255,1,255));
		hPen[3] = CreatePen(PS_SOLID,2,RGB(255,1,255));
		hPen[4] = CreatePen(PS_SOLID,1,RGB(64,255,255));
		hPen[5] = CreatePen(PS_SOLID,1,RGB(192,192,255));
		hPen[6] = CreatePen(PS_SOLID,1,RGB(55,255,255));
		hPen[7] = CreatePen(PS_SOLID,1,RGB(128,192,255));

		hFont = CreateFont(-8,-4,0,0,0,0,0,0,0,0,0,0,0,0);
		numsSize = rand()%20+75;
		InitTree(&root,&nums,&numsSize,&pData,&TreeSize,1);
		ti=FALSE,showtree=TRUE,curPos=1;curT=root;
		return 0 ;
	case WM_PAINT:
		hdc=BeginPaint (hwnd, &ps) ;
		GetClientRect(hwnd,&rect);
		SelectObject(hdc,hPen[5]);	
		SelectObject(hdc,hFont);
		SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
		SetTextColor(hdc,RGB(0,180,250));
		sprintf(buf,"curPsos = %d = %xH ",curPos,curPos);
		TextOut(hdc,200,0,buf,strlen(buf));

		if(pnode){
			sprintf(buf,"pnode = %d ",pnode->data);
			TextOut(hdc,250,10,buf,strlen(buf));
		}
		sprintf(buf,"numsSize = %d   TreeSize=%d ",numsSize, TreeSize);
		TextOut(hdc,350,0,buf,strlen(buf));
		DispArray(hdc,900,10," num ",nums,numsSize);
		TextOut(hdc, 920,10,"No.",3);
		
		for(int i=0;i<TreeSize;i++){
			sprintf(buf,"%d ",i);
			TextOut(hdc,925,20+i*10,buf,strlen(buf));
		}
		DispArray(hdc,950,10," pre ",arrA,returnSizeA);
		DispArray(hdc,975,10,"inner",arrB,returnSizeB);
		DispArray(hdc,1000,10," post",arrC,returnSizeC);
		TextOut(hdc,1025,10,"layer",5);
		for(int i=0;i<returnSizeD;i++){
			sprintf(buf,"%d ",arrD[2*i]);
			TextOut(hdc,1025,20+i*10,buf,strlen(buf));
			sprintf(buf,"[%d] ",arrD[2*i+1]);
			TextOut(hdc,1050,20+i*10,buf,strlen(buf));
		}
		DrawTreeBackground(hwnd,hPen,0,hFont);
		//DrawTreeMap(hwnd, root, curT, 1);
		if(showtree){
			DrawBiTree(hwnd,hPen,1,hFont,rect,0,root,1);
			XY=PosXY( rect, 0, curPos );
			x=HIWORD(XY),y=LOWORD(XY);
			XY=PosBox(rect, 0, curPos);
			dx=HIWORD(XY),dy=LOWORD(XY);
			SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
			SelectObject(hdc,hPen[4]);
			Rectangle(hdc,x-dx/2,y,x+dx/2,y+5*dy+dy/2);
			Ellipse(hdc,x-3,y-3,x+3,y+3);
			DrawBiTree(hwnd,hPen,1,hFont,rect,1,curT,curPos);

		}
		EndPaint (hwnd, &ps) ;
		return 0 ;
	case WM_TIMER:
		if(ti)
			PostMessage(hwnd,WM_KEYDOWN,VK_RETURN,0);
		return 0;
	case WM_LBUTTONDOWN:
		mx=LOWORD(lParam);my=HIWORD(lParam);
		width = rect.right-rect.left-30;
		height = rect.bottom-rect.top;
		layers=8;
		if(mx<width/4 && my<height/4)layers=20;
		dy=height/layers;
		d=(int)(my/dy+0.5);
		ss=1<<d;
		dx=width/ss;
		s=mx/dx-0.5;
		mpos=(s+ss)/2;	
		x=(s+1)*dx;y=d*dy;
		if((x-mx)*(x-mx)+(y-my)*(y-my)<225){
			DrawPath(hwnd,hPen,3,rect,0, (curPos<<msb(mpos)) | (mpos^(1<<msb(mpos))));
			DrawPath(hwnd,hPen,3,rect,1, mpos);	
			Sleep(2000);				
			InvalidateRect(hwnd,&rect,TRUE);
		}
		return 0;	
	case WM_KEYDOWN:
		switch(wParam){
			case VK_ESCAPE:
				PostMessage( hwnd,WM_DESTROY,0,0 );
				break;
			case VK_F1:
				sprintf(buf,"F1-show this window\nF2-set timer on/off\nRETURN-New Data\nF5-traversal\nESC-Exit");
				MessageBox(NULL,buf,"Message",MB_OK);
				return 0;	
			case VK_F2:
				if(ti==FALSE){
					SetTimer(hwnd,ID_TREE_TIMER,2000,NULL);					
				}else{
					KillTimer(hwnd,ID_TREE_TIMER);
				}
				ti=!ti;
				break;
			case VK_RETURN:
				if(arrA){free(arrA);arrA=NULL;returnSizeA=0;}
				if(arrB){free(arrB);arrB=NULL;returnSizeB=0;}
				if(arrC){free(arrC);arrC=NULL;returnSizeC=0;}
				if(arrD){free(arrD);arrD=NULL;returnSizeD=0;}
				if(root){DestroyTree(root);root=NULL;}
				if(pData){free(pData);TreeSize=0;}
				if(nums){free(nums);}
				InitTree( &root, &nums, &numsSize, &pData, &TreeSize, 0 );
				//depth=GetTreeDepth(root);
				ti=TRUE;showtree=TRUE,curPos=1;curT=root;
				break;
			case VK_F3:
				arrA =   preOrderTraversal(root, &returnSizeA);
				arrB = innerOrderTraversal(root, &returnSizeB);
				arrC =  postOrderTraversal(root, &returnSizeC);
				arrD = layerOrderTraversal(root, &returnSizeD);			
				break;
			case VK_F5:

				break;	
			case VK_F6:

				break;	
			case VK_F7:

				break;	
			case VK_F8:
			
				break;	
			case VK_LEFT:
				if(curT->lchild){curT=curT->lchild;curPos=curPos*2;}
				else{curT=root;curPos=1;}
				break;
			case VK_RIGHT:
				if(curT->rchild){curT=curT->rchild;curPos=curPos*2+1;}
				else{curT=root;curPos=1;}
				break;				
			default:
				return 0;	
		}
		InvalidateRect(hwnd,&rect,TRUE);
		return 0;	
	case WM_COMMAND:
		switch(wParam){
			case IDM_TREE_EXIT:
				PostMessage( hwnd,WM_DESTROY,0,0 );
				break;
			case IDM_TREE_NEW:
				PostMessage( hwnd,WM_KEYDOWN,VK_RETURN,0 );
				break;
			case IDM_TREE_SHOW:
				showtree=!showtree;
				PostMessage(hwnd,WM_PAINT,0,0);
				break;
			case IDM_TREE_NODE:
				num = DialogBoxParamA( hInstance, TEXT("TREENODEDLG"), hwnd, TreeNodeDlgProc, (LPARAM)buf );
				switch(num){
					case IDOK:
						value=atoi(buf);
						pos=1;
						//pnode = SearchTreeNode(root,&qnode,value,&pos,&height);
						//if(pnode){
						//	if(patharr)free(patharr);
						//	patharr = ShowPath(hwnd,pnode->pdata->pos,&pathlen);
						//	for(int i=0;i<pathlen;i++){
						//		sprintf(buf,"(%ld %ld)",patharr[i].x,patharr[i].y);
						//		TextOut(hdc,880,30+20*i,buf,strlen(buf));
						//	}
						//}else{
						//	MessageBox(NULL,"value not found","not found",MB_OK);
						//}
						break;
					case IDCANCEL:
						break;
					case IDC_ADD:
						value=atoi(buf);
						avlInsert(&root, value, &TreeSize);
						break;
					case IDC_DEL:
						value=atoi(buf);
						avlDelete(&root, value, &TreeSize);
						//depth=GetTreeDepth(root);
						break;
				}
				break;
			case IDM_TREE_PRE:
				returnSizeB=returnSizeC=returnSizeD=0;
				arrA = preOrderTraversal(root, &returnSizeA);
				break;
			case IDM_TREE_INNER:
				returnSizeA=returnSizeC=returnSizeD=0;
				arrB = innerOrderTraversal(root, &returnSizeB);
				break;
			case IDM_TREE_POST:
				returnSizeA=returnSizeB=returnSizeD=0;
				arrC = postOrderTraversal(root, &returnSizeC);
				break;
			case IDM_TREE_LAYER:
				returnSizeA=returnSizeB=returnSizeC=0;
				arrD = layerOrderTraversal(root, &returnSizeD);
				break;	
		}
		InvalidateRect(hwnd,&rect,TRUE);
		return 0;	
	case WM_DESTROY:
		if(arrA){free(arrA);arrA=NULL;}
		if(arrB){free(arrB);arrB=NULL;}
		if(arrC){free(arrC);arrC=NULL;}
		if(arrD){free(arrD);arrD=NULL;}
		if(patharr)free(patharr);
		if(root){DestroyTree(root);}
		if(pData){free(pData);}
		DeleteObject(hFont);
		for(int i=0;i<8;i++)DeleteObject(hPen[i]);
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

BOOL CALLBACK TreeNodeDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static char *pbuf;
	HWND hEdit;
	int len;
	switch(message){
		case WM_INITDIALOG:
			pbuf = (char*)lParam;
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:
					hEdit = GetDlgItem(hDlg,IDC_EDIT);
					len = GetWindowTextLength(hEdit);
					memset(pbuf,0,100);
					GetDlgItemText(hDlg,IDC_EDIT,pbuf,len+1);
					EndDialog(hDlg,IDOK);
					return TRUE;
				case IDCANCEL:
					EndDialog(hDlg,IDCANCEL);
					return FALSE;
				case IDC_ADD:
					hEdit = GetDlgItem(hDlg,IDC_EDIT);
					len = GetWindowTextLength(hEdit);
					memset(pbuf,0,100);
					GetDlgItemText(hDlg,IDC_EDIT,pbuf,len+1);					
					EndDialog(hDlg,IDC_ADD);
					return TRUE;
				case IDC_DEL:
					hEdit = GetDlgItem(hDlg,IDC_EDIT);
					len = GetWindowTextLength(hEdit);
					memset(pbuf,0,100);
					GetDlgItemText(hDlg,IDC_EDIT,pbuf,len+1);
					EndDialog(hDlg,IDC_DEL);
					return TRUE;
			}
			break;
	}	
	return FALSE;	
}


/*
typedef struct tagCREATESTRUCTA {
  LPVOID    lpCreateParams;
  HINSTANCE hInstance;
  HMENU     hMenu;
  HWND      hwndParent;
  int       cy;
  int       cx;
  int       y;
  int       x;
  LONG      style;
  LPCSTR    lpszName;
  LPCSTR    lpszClass;
  DWORD     dwExStyle;
} CREATESTRUCTA, *LPCREATESTRUCTA;

void DialogBoxA(
  [in, optional]  hInstance,
  [in]            lpTemplate,
  [in, optional]  hWndParent,
  [in, optional]  lpDialogFunc
);

INT_PTR DialogBoxIndirectParamA(
  [in, optional] HINSTANCE       hInstance,
  [in]           LPCDLGTEMPLATEA hDialogTemplate,
  [in, optional] HWND            hWndParent,
  [in, optional] DLGPROC         lpDialogFunc,
  [in]           LPARAM          dwInitParam
);

INT_PTR DialogBoxParamA(
  [in, optional] HINSTANCE hInstance,
  [in]           LPCSTR    lpTemplateName,
  [in, optional] HWND      hWndParent,
  [in, optional] DLGPROC   lpDialogFunc,
  [in]           LPARAM    dwInitParam
);
HFONT CreateFontA(
  [in] int    cHeight,
  [in] int    cWidth,
  [in] int    cEscapement,
  [in] int    cOrientation,
  [in] int    cWeight,
  [in] DWORD  bItalic,
  [in] DWORD  bUnderline,
  [in] DWORD  bStrikeOut,
  [in] DWORD  iCharSet,
  [in] DWORD  iOutPrecision,
  [in] DWORD  iClipPrecision,
  [in] DWORD  iQuality,
  [in] DWORD  iPitchAndFamily,
  [in] LPCSTR pszFaceName
);
int SetBkMode(
  [in] HDC hdc,
  [in] int mode
);
OPAQUE - Background is filled with the current background color
		 before the text, hatched brush, or pen is drawn.
TRANSPARENT - Background remains untouched.
			case VK_UP:
				if(vpos>2)vpos/=2;else vpos=1;
				break;
			case VK_LEFT:
				if(msb(vpos)<depth)vpos=vpos*2;
				break;
			case VK_RIGHT:
				if(msb(vpos)<depth)vpos=vpos*2+1;
				break;
			case VK_HOME:
				vpos=1;
				break;
*/
//-------------------------------------------------------------------
/*
	case WM_LBUTTONDOWN:
		mx=LOWORD(lParam);my=HIWORD(lParam);
		dy=(rect.bottom - rect.top)*1.0/8;
		d=(my-20)/dy+0.5;
		ss=1<<d;
		dx=(rect.right-rect.left)*1.0/ss;
		s=mx/dx-0.5;
		pos=(s+ss)/2;
		mpos=pos;
		
		SearchTree(root,pos,&pnode);
		if(pnode)wParam=wParam | 0x20;
		if((wParam & 0x04)==0x00){
			switch(wParam){
				case 0x21:				
					if(0<d && d<7 && s%2==0){
						x0=(s+1)*dx;y0=20+d*dy;
						if((x0-mx)*(x0-mx)+(y0-my)*(y0-my)<225){
							if(pnode){
								//ShowPath(hwnd,pnode,pos);
								GoThrough(hwnd,root,pos);
							}
						}
					}
					break;
				case 0x01:		
					if(650<mx && mx<700 && 30<my && my<50){
						PostMessage(hwnd,WM_COMMAND,IDM_TREE_PRE,0);
					}else if(700<mx && mx<750 && 30<my && my<50){
						PostMessage(hwnd,WM_COMMAND,IDM_TREE_INNER,0);
					}else if(750<mx && mx<800 && 30<my && my<50){
						PostMessage(hwnd,WM_COMMAND,IDM_TREE_POST,0);
					}else if(800<mx && mx<850 && 30<my && my<50){
						PostMessage(hwnd,WM_COMMAND,IDM_TREE_LAYER,0);
					}
			}
		}
		InvalidateRect(hwnd,&rect,TRUE);		
		return 0;
*/