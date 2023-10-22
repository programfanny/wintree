#ifndef _DRAWTREE_H_
#define _DRAWTREE__H_

#include <windows.h>
#include "treetype.h"


pBiTree GoThrough(pBiTree pnode, int pos);
DWORD PosXY(RECT rect,int window,int pos);
VOID DrawTreeBackground(HWND hwnd, HPEN *hPen,int n,HFONT hFont);
VOID LineBetween(HWND hwnd, HPEN *hPen,int n, int x, int y,int u, int v,int window);
VOID ShowNode(HWND hwnd, HPEN *hPen,int n, int x, int y, pBiTree T, int window);
VOID DrawTreeNode(HWND hwnd, HPEN *hPen,int n,HFONT hFont, RECT rect, int window, pBiTree T,int upos,int vpos);
VOID DrawLine(HWND hwnd, HPEN *hPen,int n, RECT rect,int window, int Tpos,int mpos);
VOID DrawNode(HWND hwnd, HFONT *hPen,int n,RECT rect,int window, pBiTree T, int upos,int vpos);
VOID DrawTree(HWND hwnd, HFONT *hPen,int n,HFONT hFont,RECT rect,int window, pBiTree T, int upos,int vpos);
VOID DrawBiTree(HWND hwnd, HFONT *hPen,int n, HFONT hFont,RECT rect,int window, pBiTree T, int upos);
VOID DrawPath(HWND hwnd, HPEN* hPen, int n, RECT rect, int window, int mpos);

//pBiTree GoThrough(pBiTree pnode, int pos);
//DWORD PosXY(RECT rect,int window,int pos);
//VOID DrawTreeBackground(HDC hdc);
//VOID LineBetween(HDC hdc, int pos1, int pos2, int window);
//VOID ShowNode(HDC hdc, pBiTree T, int Tpos, int window);
//VOID DrawTreeNode(HDC hdc, int width,int height, pBiTree T,int upos,int vpos);
//VOID DrawLine(HDC hdc,int width,int height, int Tpos,int mpos);
//VOID DrawNode(HDC hdc,int width,int height, pBiTree T, int upos,int vpos);
//VOID DrawTree(HDC hdc,int width,int height, pBiTree T, int upos,int vpos);
//VOID DrawBiTree(HDC hdc,int width,int height, pBiTree T, int upos);
//ShowPath(HDC hdc,int width,int height, pBiTree root,int rootPos,pBiTree curT,int curPos, pBiTree pnode,int mpos);

#endif