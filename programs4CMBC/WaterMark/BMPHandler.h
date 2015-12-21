#ifndef _BMPHANDLER_H_ 
#define _BMPHANDLER_H_ 

//#include <afx.h>
#include <CString>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

#define pi (double)3.14159265359

using namespace std;

class BMPHandler
{
public:
	int nWidth;
	int nHeight;
	int nLen;
	int nByteWidth;
	BYTE *lpBackup;
	BYTE *lpBitmap;
	BYTE *lpBits;
	string FileName;
	string Front;
	string Rear;

	BMPHandler();
	~BMPHandler();

	void GetPoints(BYTE *lpPoints);
	void PutPoints(BYTE *lpPoints);
	void CuttheName();
	void LoadBitmap();
	void OpenFile();
	void SaveAs();
};

#endif