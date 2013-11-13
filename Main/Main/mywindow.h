#pragma once
#include "stdafx.h"
class mywindow
{
public:
	mywindow(void);
	~mywindow(void);
	HINSTANCE hInst;
	std::wstring filesave(void);
	std::wstring fileopen(void);
};

