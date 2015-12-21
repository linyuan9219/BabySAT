#ifndef _LOADBMP_H
#define _LOADBMP_H

#ifndef EXTRA_NAME
#define EXTRA_NAME "~EX."
#endif

#include "BMPHandler.h"

using namespace std;

BMPHandler::BMPHandler()
{
	lpBitmap = NULL;
	lpBackup = NULL;
	lpBits = NULL;
}

BMPHandler::~BMPHandler()
{}

void BMPHandler::GetPoints(BYTE *lpPoints)
{
	int x, y, p;
	for(y = 0; y < nHeight; y++)
	{
		for(x = 0; x < nWidth; x++)
		{
			p = x * 3 + y * nByteWidth;
			lpPoints[p] = lpBits[p];
			lpPoints[p + 1] = lpBits[p + 1];
			lpPoints[p + 2] = lpBits[p + 2];
			/*
			lpPoints[(x + y * nWidth) * 3] = lpBits[p];
			lpPoints[(x + y * nWidth) * 3 + 1] = lpBits[p + 1];
			lpPoints[(x + y * nWidth) * 3 + 2] = lpBits[p + 2];
			*/
			/*
			lpPoints[x + y * nWidth] = (BYTE)(0.299 * (float)lpBits[p + 2]
										+ 0.587 * (float)lpBits[p + 1]
										+ 0.114 * (float)lpBits[p] + 0.1);
			*/
		}
	}
}

void BMPHandler::PutPoints(BYTE *lpPoints)
{
	int x, y, p, p1;
	for(y = 0; y < nHeight; y++)
	{
		for(x = 0; x < nWidth; x++)
		{
			p = x * 3 + y * nWidth * 3;//nByteWidth;
			p1 = x * 3 + y * nWidth * 3;
			lpBits[p] = lpPoints[p1];
			lpBits[p + 1] = lpPoints[p1 + 1];
			lpBits[p + 2] = lpPoints[p1 + 2];
			//lpBits[p + 3] = lpPoints[p1 + 3];
		}
	}
}
/*
void BMPHandler::CuttheName()
{
	int Position;
	Position = FileName.Find('.');
	Front = FileName.Left(Position);
	Rear = FileName.Mid(Position+1);
}
*/
void BMPHandler::LoadBitmap()
{
	BITMAPINFOHEADER *pInfo;
	//cout << sizeof(BITMAPFILEHEADER) << endl;
	//system ("pause");
	pInfo = (BITMAPINFOHEADER *)(lpBitmap + sizeof(BITMAPFILEHEADER));
	//pInfo = (BITMAPINFOHEADER *)(sizeof(BITMAPFILEHEADER));
	nWidth = pInfo->biWidth;
	nByteWidth = nWidth * 3;
	if (nByteWidth % 4) 
		nByteWidth += 4 - (nByteWidth % 4);
	nHeight = pInfo->biHeight;

	if (pInfo->biBitCount != 24)
	{
		if (pInfo->biBitCount != 8)
		{
			cout << "Invalidation Bitmap";
			delete lpBitmap;
			lpBitmap = 0;
			return;
		}
		unsigned int PaletteSize = 1 << pInfo->biBitCount;
		if (pInfo->biClrUsed != 0 && pInfo->biClrUsed < PaletteSize) 
			PaletteSize = pInfo->biClrUsed;
		lpBits = lpBitmap + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		RGBQUAD *pPalette = (RGBQUAD *)lpBits;
		lpBits += sizeof(RGBQUAD) * PaletteSize;
		nLen = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nByteWidth * nHeight;
		BYTE *lpTemp = lpBitmap;
		lpBitmap = new BYTE[nLen];
		BITMAPFILEHEADER bmh;
		BITMAPINFOHEADER bmi;
		bmh.bfType = 'B'+'M' * 256;
		bmh.bfSize = nLen;
		bmh.bfReserved1 = 0;
		bmh.bfReserved2 = 0;
		bmh.bfOffBits = 54;
		bmi.biSize = sizeof(BITMAPINFOHEADER);
		bmi.biWidth = nWidth;
		bmi.biHeight = nHeight;
		bmi.biPlanes = 1;
		bmi.biBitCount = 24;
		bmi.biCompression = BI_RGB;
		bmi.biSizeImage = 0;
		bmi.biXPelsPerMeter = 0;
		bmi.biYPelsPerMeter = 0;
		bmi.biClrUsed = 0;
		bmi.biClrImportant = 0;
		int nBWidth = pInfo->biWidth;
		if (nBWidth % 4) 
			nBWidth += 4 - (nBWidth % 4);
		memset(lpBitmap, 0, nLen);
		memcpy(lpBitmap, &bmh, sizeof(BITMAPFILEHEADER));
		memcpy(lpBitmap + sizeof(BITMAPFILEHEADER), &bmi, sizeof(BITMAPINFOHEADER));
		BYTE *lpBits2 = lpBitmap + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		int x, y, p1, p2, Palette;
		for(y = 0; y < nHeight; y++)
		{
			for(x = 0; x < nWidth; x++)
			{
				p1 = y * nBWidth + x;
				p2 = y * nByteWidth + x * 3;
				if (lpBits[p1] < PaletteSize)
					Palette = lpBits[p1];
				else 
					Palette = 0;
				lpBits2[p2] = pPalette[Palette].rgbBlue;
				lpBits2[p2 + 1] = pPalette[Palette].rgbGreen;
				lpBits2[p2 + 2] = pPalette[Palette].rgbRed;
			}
		}
		delete lpTemp;
	}
	
	lpBits = lpBitmap + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	if (lpBackup) 
		delete lpBackup;
	lpBackup = new BYTE[nLen];
	memcpy(lpBackup, lpBitmap, nLen);
}

void BMPHandler::OpenFile()
{
	/*
	CFile File;// = NULL;
	if (!File.Open(FileName, CFile::modeRead))
	{
		cout << "error in read file" << endl;
		return;
	}
	//CuttheName();
	*/
	int handle = open(FileName.c_str(), 0); 
	nLen = filelength(handle);
	close(handle);
    
	if (lpBitmap)
		delete lpBitmap;

	FILE *file = fopen(FileName.c_str(), "rb");
	if (file == NULL)
	{
		cout << "error in read file" << endl;
		return;
	}

	//nLen = File.GetLength();
	lpBitmap = new BYTE[nLen];
	int size = fread(lpBitmap, sizeof(BYTE), nLen, file);
	/*
	File.Read(lpBitmap, nLen);
	File.Close();
	*/
	fclose(file);
	LoadBitmap();
}

void BMPHandler::SaveAs()
{
	//CFile file;
	if (lpBitmap == 0) 
		return;
	FILE *file = fopen(FileName.c_str(), "wb");
	if (file == NULL)
    {
        cout << "ERROR~~\n";
		return;
    }
	/*
	if (!file.Open(FileName, CFile::modeWrite|CFile::modeCreate)) // Front + EXTRA_NAME + Rear
	{
		cout << "ERROR~~\n";
		return;
	}
	*/
	int nLen = nByteWidth * nHeight;
	BYTE *pMem = new BYTE[nLen + sizeof(BITMAPINFOHEADER)];
	BITMAPINFOHEADER *bmi = (BITMAPINFOHEADER *)pMem;
	bmi->biSize = sizeof(BITMAPINFOHEADER);
	bmi->biWidth = nWidth;
	bmi->biHeight = nHeight;
	bmi->biPlanes = 1;
	bmi->biBitCount = 24;
	bmi->biCompression = BI_RGB;
	bmi->biSizeImage = 0;
	bmi->biXPelsPerMeter = 0;
	bmi->biYPelsPerMeter = 0;
	bmi->biClrUsed = 0;
	bmi->biClrImportant = 0;
	BITMAPFILEHEADER bmh;
	bmh.bfType = 'B'+'M' * 256;
	bmh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nLen;
	bmh.bfReserved1 = 0;
	bmh.bfReserved2 = 0;
	bmh.bfOffBits = 54;
	memcpy(pMem + sizeof(BITMAPINFOHEADER), lpBits, nLen);
    
	fwrite(&bmh, 1, sizeof(BITMAPFILEHEADER), file);
	fwrite(pMem, 1, nLen + sizeof(BITMAPINFOHEADER), file);
	fclose(file);
	/*
	file.Write(&bmh, sizeof(BITMAPFILEHEADER));
	file.Write(pMem, nLen + sizeof(BITMAPINFOHEADER));
	file.Close();
	*/

}
#endif
