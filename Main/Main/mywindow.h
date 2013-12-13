#pragma once
#include "stdafx.h"
class mywindow
{
public:
	mywindow(void);
	~mywindow(void);
	HINSTANCE hInst;
	std::string filesave(void);
	std::string fileopen(void);
	int getint(int countdown);
};

