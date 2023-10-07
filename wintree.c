#include <windows.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "tree.h"
#include "queue.h"
#include "treeres.h"

VOID DispArray(HDC hdc,int xpos, int ypos,const char *title, int *arr,int arrSize){
	char buf[100];
	TextOut(hdc,xpos,ypos,title,strlen(title));
	for(int i=0;i<arrSize;i++){
		sprintf(buf,"%d ",arr[i]);
		TextOut(hdc,xpos,ypos+20+i*20,buf,strlen(buf));
	}
}

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
	float x,y,dx,dy,x0,y0;
	int s,d,ss,pos,num,value;
	static POINT *patharr;
	static int pathlen;
	static RECT rect;
	static HINSTANCE hInstance; 
	static int numsSize,TreeNodeCount,vpos;
	static int *arrA, *arrB, *arrC, *arrD;
	static int returnSizeA = 0,returnSizeB = 0,returnSizeC = 0,returnSizeD = 0;
	static pBiTree root=NULL,pnode,qnode;
	static int depth;
	static pTreeNode *pData;
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
		InitTree(&root, &numsSize, &pData, &TreeNodeCount);
		depth=GetTreeDepth(root);
		ti=FALSE,showtree=TRUE,vpos=1;
		return 0 ;
	case WM_PAINT:
		hdc=BeginPaint (hwnd, &ps) ;
		GetClientRect(hwnd,&rect);
		sprintf(buf,"Tree Node Count : %d",TreeNodeCount);
		TextOut(hdc,30,0,buf,strlen(buf));
		/*
		for(int i=0;i<TreeNodeCount;i++){
			sprintf(buf,"%p %d %d %d",pData[i],i,pData[i]->val,pData[i]->pos);
			TextOut(hdc,30,30+i*20,buf,strlen(buf));
		}
		*/
		SetTextColor(hdc,RGB(0,180,250));	
		DispArray(hdc,650,30," pre ",arrA,returnSizeA);
		DispArray(hdc,700,30,"inner",arrB,returnSizeB);
		DispArray(hdc,750,30," post",arrC,returnSizeC);
		TextOut(hdc,800,30,"layer",5);
		for(int i=0;i<returnSizeD;i++){
			sprintf(buf,"%d ",arrD[2*i]);
			TextOut(hdc,800,50+i*20,buf,strlen(buf));
			sprintf(buf,"[%d] ",arrD[2*i+1]);
			TextOut(hdc,830,50+i*20,buf,strlen(buf));
		}		
		DrawTreeBackground(hwnd);
		//DrawTreeMap(hwnd, root, 1);
		if(showtree)DrawBiTree(hwnd,root,vpos,pData,TreeNodeCount);
		EndPaint (hwnd, &ps) ;
		return 0 ;
	case WM_TIMER:
		if(ti)
		PostMessage(hwnd,WM_KEYDOWN,VK_RETURN,0);
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
				if(arrA){free(arrA);}
				if(arrB){free(arrB);}
				if(arrC){free(arrC);}
				if(arrD){free(arrD);}
				if(root){DestroyTree(root);root=NULL;}
				if(pData){
					for(int i=0;i<TreeNodeCount;i++)free(pData[i]);
					free(pData);
				}
				InitTree( &root,  &numsSize, &pData, &TreeNodeCount );
				depth=GetTreeDepth(root);
				ti=TRUE;showtree=TRUE,vpos=1;
				break;
			case VK_F5:
				arrA =   preOrderTraversal(root, &returnSizeA);
				arrB = innerOrderTraversal(root, &returnSizeB);
				arrC =  postOrderTraversal(root, &returnSizeC);
				arrD = layerOrderTraversal(root, &returnSizeD);			
				break;
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
		}
		InvalidateRect(hwnd,&rect,TRUE);
		return 0;	
	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);y=HIWORD(lParam);
		if(650<x && x<700 && 30<y && y<50){
			PostMessage(hwnd,WM_COMMAND,IDM_TREE_PRE,0);
		}else if(700<x && x<750 && 30<y && y<50){
			PostMessage(hwnd,WM_COMMAND,IDM_TREE_INNER,0);
		}else if(750<x && x<800 && 30<y && y<50){
			PostMessage(hwnd,WM_COMMAND,IDM_TREE_POST,0);
		}else if(800<x && x<850 && 30<y && y<50){
			PostMessage(hwnd,WM_COMMAND,IDM_TREE_LAYER,0);
		}else{
			dy=(rect.bottom - rect.top)*1.0/8;
			d=(y-20)/dy+0.5;
			ss=1<<d;
			dx=(rect.right-rect.left)*1.0/ss;
			s=x/dx-0.5;
			x0=(s+1)*dx;y0=20+d*dy;
			hdc=GetDC(hwnd);
			if(0<d && d<7 && s%2==0){
				pos=(s+ss)/2;
				num=-1;
				for(int i=0;i<TreeNodeCount;i++){
					if(pData[i]->pos==pos){num=i;break;}
				}
				Ellipse(hdc, x0-10, y0+10, x0-10, y0+10);
				BitBlt(hdc,300,0,400,20,hdc,750,0,SRCCOPY);
				if(num>=0){
					if(patharr)free(patharr);
					patharr = ShowPath(hwnd,pos,&pathlen);
				}else{
					sprintf(buf,"empty tree node.");
					MessageBox(NULL,buf,"empty tree node",MB_OK);
				}
			}
			ReleaseDC(hwnd, hdc);
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
						pnode = SearchTreeNode(root,&qnode,value,&pos);
						if(pnode){
							if(patharr)free(patharr);
							patharr = ShowPath(hwnd,pnode->pdata->pos,&pathlen);
							for(int i=0;i<pathlen;i++){
								sprintf(buf,"(%ld %ld)",patharr[i].x,patharr[i].y);
								TextOut(hdc,880,30+20*i,buf,strlen(buf));
							}
						}else{
							MessageBox(NULL,"value not found","not found",MB_OK);
						}
						break;
					case IDCANCEL:
						break;
					case IDC_ADD:
						value=atoi(buf);
						pos=1;
						pnode = SearchTreeNode(root,&qnode,value,&pos);
						if(pnode==NULL){
							pnode=CreateNode(value, pos, &pData,&TreeNodeCount);
							if(pos%2)qnode->right=pnode;
							else qnode->left=pnode;
						}else{
							pnode->pdata->count++;
						}
						depth=GetTreeDepth(root);
						break;
					case IDC_DEL:
						value=atoi(buf);
						RemoveTreeNode(&root, value, &pData, &TreeNodeCount);
						depth=GetTreeDepth(root);
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
OPAQUE - Background is filled with the current background color b
         efore the text, hatched brush, or pen is drawn.
TRANSPARENT - Background remains untouched.

*/