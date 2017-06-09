#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>

#include "bmp_header.h"

#define DEBUG false
#define STEP false

#define SRC_FILE "src.bmp"

int rgb2y(int rgb) {
	int r, g, b;
	r = (rgb >> 16) & 0xff;
	g = (rgb >> 8) & 0xff;
	b = rgb & 0xff;

	return ((r * 77 + g * 150 + b * 29) >> 8) & 0xff;
}

int main(int argc, char *argv[]) {
	tagBITMAPFILEHEADER bmpfh;
	tagBITMAPINFOHEADER bmpih;
	unsigned char *offset;
	FILE *fpr, *fpw;
	unsigned int *src, *dst[2];

	fpr = fopen(SRC_FILE, "rb");
	if (fpr == NULL) {
		std::cerr << "Can't open source file." << std::endl;
		return 1;
	}
	readBMPHeader(fpr, bmpfh, bmpih, offset, src);
	fclose(fpr);

	dst[0] = new unsigned int[bmpih.biWidth * bmpih.biHeight]();
	dst[1] = new unsigned int[bmpih.biWidth * bmpih.biHeight]();

	// rgb2y
	for (int y = 0; y < bmpih.biHeight; y++) {
		for (int x = 0; x < bmpih.biWidth; x++) {
			src[y * bmpih.biWidth + x] = rgb2y(src[y * bmpih.biWidth + x]);
		}
	}

	// filter
	int k[3][3] = { {1,2,1}, {2,4,2},{1,2,1} };

	for (int f = 0; f < 2; f++) {
		for (int y = 0; y < bmpih.biHeight; y++) {
			for (int x = 0; x < bmpih.biWidth; x++) {
				int idx = y * bmpih.biWidth + x;
				int sum = 0;
#if DEBUG
				printf("%2d:(%2d, %2d)\n", f, x, y);
#endif
				for (int n = -1; n < 2; n++) {
					for (int m = -1; m < 2; m++) {
						int tmp_idx = idx + m + n * bmpih.biWidth + bmpih.biWidth * bmpih.biHeight * f;
						tmp_idx %= bmpih.biWidth * bmpih.biHeight;
#if DEBUG
						printf("(%2d, %2d) ", tmp_idx % bmpih.biWidth, tmp_idx / bmpih.biWidth);
#endif
						if (0 <= tmp_idx/* && tmp_idx < bmpih.biWidth * bmpih.biHeight*/) {
							if (x != 0 || y != 0 || m != -1 || n != 1 || f != 0) {
								sum += src[tmp_idx] * k[m + 1][n + 1];
#if DEBUG
								printf("o [%02x]", src[tmp_idx]);
#endif
							}
#if DEBUG
							else {
								printf("x [00]");
							}
#endif
						}
#if DEBUG
						else {
							printf("x [00]");
						}
#endif
					}
#if DEBUG
					printf("\n");
#endif
				}
				sum /= 16;
				dst[f][idx] = (sum << 16) | (sum << 8) | sum;
#if DEBUG
				printf("%06x\n", dst[f][idx]);
#if STEP
				getchar();
#endif
#endif
			}
		}
	}

#if DEBUG
	for (int f = 0; f < 2; f++) {
		for (int i = 0; i < bmpih.biWidth * bmpih.biHeight; i++) {
			if (i % bmpih.biWidth == 0) {
				std::cout << std::endl;
			}
			printf("%06x ", dst[f][i]);
		}
		std::cout << std::endl;
	}
#endif

	fpw = fopen("dst0.bmp", "wb");
	if (fpw == NULL) {
		std::cerr << "Can't open destination file." << std::endl;
		return 2;
	}
	writeBMPHeader(fpw, bmpfh, bmpih, offset, dst[0]);
	fclose(fpw);

	fpw = fopen("dst1.bmp", "wb");
	if (fpw == NULL) {
		std::cerr << "Can't open destination file." << std::endl;
		return 2;
	}
	writeBMPHeader(fpw, bmpfh, bmpih, offset, dst[1]);
	fclose(fpw);

	delete dst[0];
	delete dst[1];

	delete offset;

#if DEBUG
	getchar();
#endif
}