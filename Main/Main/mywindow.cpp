#include "stdafx.h"
#include "mywindow.h"
#include <tchar.h>


#define BUTTON_ID1 1
#define BUTTON_ID2 2
#define EDIT_ID 3
int count;
mywindow::mywindow(void)
{
	hInst = GetModuleHandle(NULL) ;
}


mywindow::~mywindow(void)
{
}

std::string mywindow::filesave(void)
{
	std::string name=".mywindow.dat";
    static OPENFILENAME     ofn;
    static TCHAR            szPath[ MAX_PATH ];
    static TCHAR            szFile[ MAX_PATH ];
    
    if ( szPath[0] == TEXT('\0') ){
        GetCurrentDirectory( MAX_PATH, szPath );
    }
    if ( ofn.lStructSize == 0 ){
        ofn.lStructSize         = sizeof(OPENFILENAME);
        ofn.hwndOwner           = NULL;
        ofn.lpstrInitialDir     = szPath;       // 初期フォルダ位置
        ofn.lpstrFile           = szFile;       // 選択ファイル格納
        ofn.nMaxFile            = MAX_PATH;
        ofn.lpstrDefExt         = TEXT(".dat");
        ofn.lpstrFilter         = TEXT("datファイル(*.dat)\0*.dat\0");
        ofn.lpstrTitle          = TEXT("データを保存します。");
        ofn.Flags               = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
    }
    if ( GetSaveFileName(&ofn) ){
        //MessageBox( NULL, szFile, TEXT("ファイル名を付けて保存"), MB_OK );
		name=szFile;
    }

	return name;
}

std::string mywindow::fileopen(void){
	std::string name=".mywindow.dat";
	static OPENFILENAME     ofn;
    static TCHAR            szPath[ MAX_PATH ];
    static TCHAR            szFile[ MAX_PATH ];
    
    if ( szPath[0] == TEXT('\0') ){
        GetCurrentDirectory( MAX_PATH, szPath );
    }
    if ( ofn.lStructSize == 0 ){
        ofn.lStructSize         = sizeof(OPENFILENAME);
        ofn.hwndOwner           = NULL;
        ofn.lpstrInitialDir     = szPath;       // 初期フォルダ位置
        ofn.lpstrFile           = szFile;       // 選択ファイル格納
        ofn.nMaxFile            = MAX_PATH;
        ofn.lpstrDefExt         = TEXT(".dat");
        ofn.lpstrFilter         = TEXT("datファイル(*.dat)\0*.dat\0");
        ofn.lpstrTitle          = TEXT("モデルデータを開きます。");
        ofn.Flags               = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
    }
    if ( GetOpenFileName(&ofn) ){
				name=szFile;
    }
	return name;
}


LRESULT CALLBACK WndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	static HWND edit;
	PAINTSTRUCT ps;
	LPSTR strText,szStr1=TEXT("カウントダウンの秒数（1～10秒の間）を指定してください。");
	int i;
	HDC hdc;
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 30, 10, szStr1, strlen(szStr1));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_CREATE:
		edit = CreateWindow(
			TEXT("EDIT") , TEXT("5") , 
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT ,
			220 , 40 , 50 , 25 , hwnd , (HMENU)EDIT_ID ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);


		CreateWindow(
			TEXT("BUTTON") , TEXT("OK") ,
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON ,
			80 , 70 , 150 , 30 ,
			hwnd , (HMENU)BUTTON_ID1 , ((LPCREATESTRUCT)(lp))->hInstance , NULL);

		CreateWindow(
			TEXT("BUTTON") , TEXT("cancel") ,
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON ,
			250 , 70 , 150 , 30 ,
			hwnd , (HMENU)BUTTON_ID2 , ((LPCREATESTRUCT)(lp))->hInstance , NULL);
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wp)) {
			case BUTTON_ID1:
				//コンボボックスの情報からint型を返す
				strText = (LPSTR)malloc(GetWindowTextLength(hwnd) + 4);
				GetWindowTextA(edit , strText , GetWindowTextLength(hwnd) + 4);
				i=atoi(strText);
				if(i<1||i>10){
					MessageBox(hwnd , "値が指定範囲外です。" , TEXT("error") , MB_OK);
				}else{
					//SendMessage(hwnd,);
					count=i*30;
					DestroyWindow(hwnd);
					PostQuitMessage(0);
				}
				free(strText);
				break;
			case BUTTON_ID2:
				//値を変化せずに終了(ok)
				DestroyWindow(hwnd);
				PostQuitMessage(0);
				break;
		}
		return 0;
	}
	return DefWindowProc(hwnd , msg , wp , lp);
}


int mywindow::getint(int countdown){
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	count=countdown;
	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInst;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("STATIC");
	
	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
			TEXT("STATIC") , TEXT("countdown設定") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			500 , 150 ,
			NULL , NULL , hInst , NULL
	);

	if (hwnd == NULL) return -1;

	while(GetMessage(&msg , NULL , 0 , 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(winc.lpszClassName,winc.hInstance);//解除、２回目のwindowを表示させるために行う
	return count;
}