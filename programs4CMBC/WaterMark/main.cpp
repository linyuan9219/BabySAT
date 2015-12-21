#include <iostream>
#include <ctime>
#include <string.h>

#include "BMPHandler.h"
#include "ColorRGB.h"

//#define kScreenHeight 600
//#define kScreenWidth 800
#define kRandSeed1 2010011272
#define kRandSeed2 7

#define kModNum 4093

using namespace std;

int kScreenHeight = 0;
int kScreenWidth = 0;
/*
ColorRGB PixelBuffer[kScreenWidth][kScreenHeight];
int sequence[kScreenHeight * kScreenWidth];
bool changed[kScreenWidth][kScreenHeight];
*/

ColorRGB *PixelBuffer;
int *sequence;
bool *changed;

void judgeChanged(int i, int *rand1, int *rand2, int *rand3)
{
	int num_r = 1;
	int num_g = 1;
	int num_b = 1;
	for (int k = 0; k < 12; k++)
	{
		/*
		int x = sequence[i + k] % kScreenWidth;
		int y = sequence[i + k] / kScreenWidth;

		num_r = 64 * num_r + (PixelBuffer[x][y].R >> 2) ^ (rand1[k]) + 1;
		num_r = num_r % kModNum;
		num_g = 64 * num_g + (PixelBuffer[x][y].G >> 2) ^ (rand2[k]) + 1;
		num_g = num_g % kModNum;
		num_b = 64 * num_b + (PixelBuffer[x][y].B >> 2) ^ (rand3[k]) + 1;
		num_b = num_b % kModNum;
		*/
		num_r = 64 * num_r + (PixelBuffer[sequence[i + k]].R >> 2) ^ (rand1[k]) + 1;
		num_r = num_r % kModNum;
		num_g = 64 * num_g + (PixelBuffer[sequence[i + k]].G >> 2) ^ (rand2[k]) + 1;
		num_g = num_g % kModNum;
		num_b = 64 * num_b + (PixelBuffer[sequence[i + k]].B >> 2) ^ (rand3[k]) + 1;
		num_b = num_b % kModNum;
	}

	int judge_r = 0;
	int judge_g = 0;
	int judge_b = 0;

	for (int k = 0; k < 12; k++)
	{
		/*
		int x = sequence[i + k] % kScreenWidth;
		int y = sequence[i + k] / kScreenWidth;
		
		judge_r += ((PixelBuffer[x][y].R % 2) << k);
		judge_g += ((PixelBuffer[x][y].G % 2) << k);
		judge_b += ((PixelBuffer[x][y].B % 2) << k);
		*/
		judge_r += ((PixelBuffer[sequence[i + k]].R % 2) << k);
		judge_g += ((PixelBuffer[sequence[i + k]].G % 2) << k);
		judge_b += ((PixelBuffer[sequence[i + k]].B % 2) << k);
	}
	int x = sequence[i] % kScreenWidth;
    int y = sequence[i] / kScreenWidth;
    
	if (judge_r != num_r || judge_g != num_g || judge_b != num_b)
		changed[sequence[i]] = true;
	else
        changed[sequence[i]] = false;
}

ColorRGB fix4Index(int index, int start)
{
	ColorRGB tmpColor(0, 0, 0);
	ColorRGB resColor(0, 0, 0);
	for (int k = 0; k < 12; k++)
	{
		if (k == index)
			continue;
		int x = sequence[start + k] % kScreenWidth;
		int y = sequence[start + k] / kScreenWidth;

        tmpColor.R ^= PixelBuffer[sequence[start + k]].R;
        tmpColor.G ^= PixelBuffer[sequence[start + k]].G;
        tmpColor.B ^= PixelBuffer[sequence[start + k]].B;
	}
	
	tmpColor.R = (tmpColor.R >> 2) << 2;
	tmpColor.G = (tmpColor.G >> 2) << 2;
	tmpColor.B = (tmpColor.B >> 2) << 2;
	
	int s_i = 0, e_i = 6;
	if (index < 6)
		s_i = 6, e_i = 12;

	for (int k = s_i; k < e_i; k++)
	{
        int x = sequence[start + k] % kScreenWidth;
		int y = sequence[start + k] / kScreenWidth;
		
        resColor.R += ((PixelBuffer[sequence[start + k]].R % 4) >> 1) << (2 + k % 6);
        resColor.G += ((PixelBuffer[sequence[start + k]].G % 4) >> 1) << (2 + k % 6);
        resColor.B += ((PixelBuffer[sequence[start + k]].B % 4) >> 1) << (2 + k % 6);
	}
	int x = sequence[start + index] % kScreenWidth;
    int y = sequence[start + index] / kScreenWidth;

	resColor.R += PixelBuffer[sequence[start + index]].R % 4;
	resColor.G += PixelBuffer[sequence[start + index]].G % 4;
	resColor.B += PixelBuffer[sequence[start + index]].B % 4;

	resColor.R ^= tmpColor.R;
	resColor.G ^= tmpColor.G;
	resColor.B ^= tmpColor.B;

	return resColor;
}

void insertWaterMark()
{
	srand(kRandSeed1);

	for (int i = 0; i < kScreenHeight * kScreenWidth; i++)
		sequence[i] = i;
	for (int i = kScreenHeight * kScreenWidth - 1; i > 0; i--)
	{
		int index = (rand() * 30000 + rand()) % i;
		swap(sequence[i], sequence[index]);
	}
    
	srand(kRandSeed2);

	for (int i = 0; i < kScreenWidth * kScreenHeight; i += 12)
	{
		ColorRGB tmp(0, 0, 0);
		int num_r = 1;
		int num_g = 1;
		int num_b = 1;
		for (int k = 0; k < 12; k++)
		{
			int x = sequence[i + k] % kScreenWidth;
			int y = sequence[i + k] / kScreenWidth;
			tmp.R ^= PixelBuffer[sequence[i + k]].R;
			tmp.G ^= PixelBuffer[sequence[i + k]].G;
			tmp.B ^= PixelBuffer[sequence[i + k]].B;

			num_r = 64 * num_r + (PixelBuffer[sequence[i + k]].R >> 2) ^ (rand() % 64) + 1;
			num_r = num_r % kModNum;
			num_g = 64 * num_g + (PixelBuffer[sequence[i + k]].G >> 2) ^ (rand() % 64) + 1;
			num_g = num_g % kModNum;
			num_b = 64 * num_b + (PixelBuffer[sequence[i + k]].B >> 2) ^ (rand() % 64) + 1;
			num_b = num_b % kModNum;
		}

		for (int k = 0; k < 12; k++)
		{
			int x = sequence[i + k] % kScreenWidth;
			int y = sequence[i + k] / kScreenWidth;

			PixelBuffer[sequence[i + k]].R >>= 2;
			PixelBuffer[sequence[i + k]].R <<= 2;
			PixelBuffer[sequence[i + k]].R += ((tmp.R >> (k % 6 + 2)) % 2) << 1;
			PixelBuffer[sequence[i + k]].R += (num_r >> k) % 2;
            
			PixelBuffer[sequence[i + k]].G >>= 2;
			PixelBuffer[sequence[i + k]].G <<= 2;
			PixelBuffer[sequence[i + k]].G += ((tmp.G >> (k % 6 + 2)) % 2) << 1;
			PixelBuffer[sequence[i + k]].G += (num_g >> k) % 2;

			PixelBuffer[sequence[i + k]].B >>= 2;
			PixelBuffer[sequence[i + k]].B <<= 2;
			PixelBuffer[sequence[i + k]].B += ((tmp.B >> (k % 6 + 2)) % 2) << 1;
			PixelBuffer[sequence[i + k]].B += (num_b >> k) % 2;
		}
	}
}

int testWaterMark(int mode)
{
	srand(kRandSeed1);

	for (int i = 0; i < kScreenHeight * kScreenWidth; i++)
		sequence[i] = i;
	for (int i = kScreenHeight * kScreenWidth - 1; i > 0; i--)
	{
		int index = (rand() * 30000 + rand()) % i;
		swap(sequence[i], sequence[index]);
	}
    
	srand(kRandSeed2);

	memset(changed, 0, sizeof(changed));
    
    int r1[12];
    int r2[12];
    int r3[12];
    
	for (int i = 0; i < kScreenWidth * kScreenHeight; i += 12)
	{
		for (int k = 0; k < 12; k++)
		{
		    r1[k] = rand() % 64;
		    r2[k] = rand() % 64;
		    r3[k] = rand() % 64;
		}
		judgeChanged(i, r1, r2, r3);
	}

	srand(kRandSeed2);

	int count = 0;
	int fixed_count = 0;
	for (int i = 0; i < kScreenHeight * kScreenWidth; i += 12)
	{
		int x = sequence[i] % kScreenWidth;
		int y = sequence[i] / kScreenWidth;
        
        for (int k = 0; k < 12; k++)
		{
		    r1[k] = rand() % 64;
		    r2[k] = rand() % 64;
		    r3[k] = rand() % 64;
		}
        
		if (changed[sequence[i]])
		{
			//count++;
			for (int k = 0; k < 12; k++)
			{
				int _x = sequence[i + k] % kScreenWidth;
                int _y = sequence[i + k] / kScreenWidth;
				
				ColorRGB backup = PixelBuffer[sequence[i + k]];
				
				bool canbreak = false;
                for (int t = 0; t < 8; t++)
                {
                    PixelBuffer[sequence[i + k]].R ^= t % 2;
                    PixelBuffer[sequence[i + k]].G ^= (t % 4) >> 1;
                    PixelBuffer[sequence[i + k]].B ^= (t % 8) >> 2;
                    
                    ColorRGB fixed = fix4Index(k, i);
                    PixelBuffer[sequence[i + k]] = fixed;
				
                    judgeChanged(i, r1, r2, r3);

                    if (changed[sequence[i]])
                    {
                        PixelBuffer[sequence[i + k]] = backup;
                    }
                    else
                    {
                        if (mode == 1)
                            changed[sequence[i + k]] = true;
                        fixed_count++;
                        canbreak = true;
                        break;
                    }
                }
                if (canbreak)
                    break;
			}
		}
	}
	
	for (int i = 0; i < kScreenHeight * kScreenWidth; i += 12)
	{
	    int x = sequence[i] % kScreenWidth;
		int y = sequence[i] / kScreenWidth;
	    if (changed[sequence[i]])
        {
            for (int k = 0; k < 12; k++)
            {
                int _x = sequence[i + k] % kScreenWidth;
                int _y = sequence[i + k] / kScreenWidth;
                changed[sequence[i + k]] = true;
            }
        }
	}

	for (int i = 0; i < kScreenHeight * kScreenWidth; i += 12)
	{
	    int x = sequence[i] % kScreenWidth;
		int y = sequence[i] / kScreenWidth;

		if (changed[sequence[i]])
	    for (int s = 0; s < 12; s++)
        {
            int tmp = 0;
            int _x = sequence[i + s] % kScreenWidth;
            int _y = sequence[i + s] / kScreenWidth;
            for (int k = -3; k < 4; k++)
                for (int t = -3; t < 4; t++)
                {
                    /*
                    int __x = max(_x + k, 0);
                    __x = min(__x, kScreenWidth);
                    int __y = max(_y + t, 0);
                    __y = min(__y, kScreenHeight);
                    */
                    int index = max(sequence[i + s] + k + t * kScreenWidth, 0);
                    index = min(index, kScreenHeight * kScreenWidth - 1);
                    if (changed[index])
                        tmp++;
                }

            if ((tmp < 20 && mode == 1) || (tmp < 2 && mode == 0))
            {
                changed[sequence[i + s]] = false;
            }
            else
                changed[sequence[i + s]] = true;
            
        }
    }
    
    int error_count = 0;
	for (int i = 0; i < kScreenHeight * kScreenWidth; i++)
	{
	    int _x = sequence[i] % kScreenWidth;
        int _y = sequence[i] / kScreenWidth;
        
        if (changed[sequence[i]])
            count++;
        
	    if (mode == 1)
	    if (changed[sequence[i]])
	    {
            PixelBuffer[sequence[i]].R = 0;
            PixelBuffer[sequence[i]].G = 0;
            PixelBuffer[sequence[i]].B = 0;
            continue;
	    }
	    else
	    {
	        PixelBuffer[sequence[i]].R = 255;
            PixelBuffer[sequence[i]].G = 255;
            PixelBuffer[sequence[i]].B = 255;
            continue;
	    }

	    if (!changed[sequence[i]])
            continue;

	    int r = 0, g = 0, b = 0;
	    int num = 0;
        
        for (int l = 1; l < 10; l++)
        {
            num = 0;
	    for (int k = -l; k < l + 1; k++)
            for (int t = -l; t < l + 1; t++)
            {
                /*
                int __x = max(_x + k, 0);
                __x = min(__x, kScreenWidth);
                int __y = max(_y + t, 0);
                __y = min(__y, kScreenHeight);
                */
                
                int index = max(sequence[i] + k + t * kScreenWidth, 0);
                index = min(index, kScreenHeight * kScreenWidth - 1);
                
                if (changed[index])
                    continue;
                
                num++;
                r += (int)PixelBuffer[index].R;
                g += (int)PixelBuffer[index].G;
                b += (int)PixelBuffer[index].B;
            }
            if (num > 0)
                break;
        }
        if (num == 0)
        {
            PixelBuffer[sequence[i]].R = 255;
            PixelBuffer[sequence[i]].G = 0;
            PixelBuffer[sequence[i]].B = 0;
            error_count++;
            continue;
        }
        r /= num;
        g /= num;
        b /= num;
        PixelBuffer[sequence[i]].R = r % 256;
        PixelBuffer[sequence[i]].G = g % 256;
        PixelBuffer[sequence[i]].B = b % 256;
	}
	cout << "error_count is " <<  error_count << endl;
	cout << "fixed point is " << fixed_count << endl;
	return count;
}

int main(int argc, char* argv[])
{
	BMPHandler bmphandler;
	if (argc < 3)
	{
	    cout << "not enough arg..." << endl;
        return 1;
	}
	bmphandler.FileName = argv[2];

	//bmphandler.FileName = "C:\\Users\\apple\\Desktop\\rabbit.bmp";
    //bmphandler.FileName = "C:\\Users\\apple\\Desktop\\test\\rabbit_test.bmp";
	bmphandler.OpenFile();
    
    cout << "finish reading" << endl;
    
	//bmphandler.FileName = "C:\\Users\\Public\\Desktop\\rabbit_test2.bmp";

	int len = strlen(argv[2]);
	argv[2][len - 4] = '\0';
	string out_name = argv[2];
	string out_name2 = argv[2];

	if (argv[1][0] == 'i')
        out_name += "_marked.bmp";
    else
    {
        out_name += "_fixed.bmp";
        out_name2 += "_tested.bmp";
    }

	kScreenHeight = bmphandler.nHeight;
	kScreenWidth = bmphandler.nWidth;

	PixelBuffer = new ColorRGB[kScreenWidth * kScreenHeight];
	sequence = new int[kScreenWidth * kScreenHeight];
	changed = new bool[kScreenWidth * kScreenHeight];

	bmphandler.GetPoints(&PixelBuffer[0].B);
    
    cout << "finish it " << endl;

    if (argv[1][0] == 'i')
        insertWaterMark(), cout << "enter here!" << endl;
    else
    {
        cout << testWaterMark(1) << endl;

        bmphandler.FileName = out_name2;
        bmphandler.PutPoints(&PixelBuffer[0].B);
        bmphandler.SaveAs();
        
        delete []PixelBuffer;
        delete []changed;
        
        PixelBuffer = new ColorRGB[kScreenWidth * kScreenHeight];
        changed = new bool[kScreenWidth * kScreenHeight];
        
        bmphandler.FileName = argv[2];
        bmphandler.FileName += ".bmp";
        
        cout << bmphandler.FileName << endl;

        bmphandler.OpenFile();
        bmphandler.GetPoints(&PixelBuffer[0].B);

        testWaterMark(0);
    }
    
    bmphandler.FileName = out_name;
	bmphandler.PutPoints(&PixelBuffer[0].B);
    bmphandler.SaveAs();
    
	system ("pause");
	return 0;
}
