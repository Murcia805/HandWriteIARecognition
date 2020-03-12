//
// Created by Guaberx on 9/21/2016.
//

#ifndef HANDWRITERECOGNITION_BMPSTRUCTURE_H
#define HANDWRITERECOGNITION_BMPSTRUCTURE_H
#include <vector>
using std::vector;

#pragma pack(1)
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned char BYTE;

typedef void* HANDLE;

typedef struct tagBITMAPFILEHEADER {
    WORD	bfType;
    DWORD	bfSize;
    WORD	bfReserved1;
    WORD	bfReserved2;
    DWORD	bfOffBits;
} BITMAPFILEHEADER,*LPBITMAPFILEHEADER,*PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
    DWORD	biSize;
    LONG	biWidth;
    LONG	biHeight;
    WORD	biPlanes;
    WORD	biBitCount;
    DWORD	biCompression;
    DWORD	biSizeImage;
    LONG	biXPelsPerMeter;
    LONG	biYPelsPerMeter;
    DWORD	biClrUsed;
    DWORD	biClrImportant;
} BITMAPINFOHEADER,*LPBITMAPINFOHEADER,*PBITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} RGBTRIPLE,*LPRGBTRIPLE;

struct ReadBMP{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    vector<RGBTRIPLE> image;
};
#pragma pack(0)
#endif //HANDWRITERECOGNITION_BMPSTRUCTURE_H
