/*
 * bmp_header.h 
 *
 * Created on: 2015/08/03
 *      Author: Masaaki
 */

/*
 * BMP ファイルフォーマットから引用
 * http://www.kk.iij4u.or.jp/~kondo/bmp/
 */

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#ifndef BMP_HEADER_H_
#define BMP_HEADER_H_

#define BITMAPFILEHEADER_LEN 14
#define BITMAPINFOHEADER_LEN 40

// BITMAPFILEHEADER 14bytes
typedef struct tagBITMAPFILEHEADER {
  unsigned short bfType;
  unsigned long  bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned long  bfOffBits;
} BITMAPFILEHEADER;

// BITMAPINFOHEADER 40bytes
typedef struct tagBITMAPINFOHEADER{
    unsigned long  biSize;
    long           biWidth;
    long           biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long           biXPixPerMeter;
    long           biYPixPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImporant;
} BITMAPINFOHEADER;

typedef struct BMP24bitsFORMAT {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} BMP24FORMAT;

void readBMPHeader(FILE *fp, tagBITMAPFILEHEADER &bmpfh, tagBITMAPINFOHEADER &bmpih, unsigned char *&offset, unsigned int *&img);
void writeBMPHeader(FILE *fp, tagBITMAPFILEHEADER &bmpfh, tagBITMAPINFOHEADER &bmpih, unsigned char *&offset, unsigned int *&img);

#endif /* BMP_HEADER_H_ */
