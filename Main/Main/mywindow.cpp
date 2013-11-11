#include "stdafx.h"
#include "mywindow.h"
#include <tchar.h>


mywindow::mywindow(void)
{
	hInst = GetModuleHandle(NULL) ;
}


mywindow::~mywindow(void)
{
}

std::wstring mywindow::filesave(void)
{
	std::wstring name=L".mywindow.dat";
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
        ofn.lpstrTitle          = TEXT("モデルデータを保存します。");
        ofn.Flags               = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
    }
    if ( GetSaveFileName(&ofn) ){
        MessageBox( NULL, szFile, TEXT("ファイル名を付けて保存"), MB_OK );
		name=szFile;
    }

	return name;
}