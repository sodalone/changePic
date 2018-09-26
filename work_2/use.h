#pragma once

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <string.h>
#include <commdlg.h>

using namespace std;

typedef struct node {
	unsigned char b;
	unsigned char g;
	unsigned char r;
}NODE;


void showPic(NODE *p, HWND win, int w, int h) {
	int r, g, b, pix;
	HDC dc = GetDC(win);
	int x = 40;
	int y = 20;
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			b = p->b;
			g = p->g;
			r = p->r;
			p++;
			pix = RGB(r, g, b);
			SetPixel(dc, x + i, y + h - j, pix);
		}
	}
}

NODE * readBMP(FILE *fp, int w, int h, int zero) {
	NODE *buf;
	buf = (NODE*)malloc(sizeof(NODE)*(w*h));
	for (int i = 0; i < w*h; i++) {
		fread(&buf[i], sizeof(NODE), 1, fp);
		if (i % w == (w - 1) && zero != 0) {
			char temp;
			for (int j = 0; j < zero; j++)
				fread(&temp, sizeof(char), 1, fp);
		}
	}
	return buf;
}

string to_String(long n)
{
	int m = n;
	char s[100];
	char ss[100];
	int i = 0, j = 0;
	if (n < 0)// 处理负数
	{
		m = 0 - m;
		j = 1;
		ss[0] = '-';
	}
	while (m > 0)
	{
		s[i++] = m % 10 + '0';
		m /= 10;
	}
	s[i] = '\0';
	i = i - 1;
	while (i >= 0)
	{
		ss[j++] = s[i--];
	}
	ss[j] = '\0';
	return ss;
}

void TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}

int charToint(char temp[]) {
	int sum = 0;
	for (int i = 0; i < strlen(temp); i++) {
		sum = sum * 10 + temp[i] - '0';
	}
	return sum;
}

void changePixel(int w_new, int h_new, NODE *p, BITMAPINFOHEADER bi) {
	int s1, s2;
	int flag = 1;
	s1 = bi.biHeight / h_new;
	s2 = bi.biWidth / w_new;
	for (int i = 0; i < h_new+5; i++) {
		for (int j = 0; j < w_new+5; j++) {
			if (i*s1*bi.biWidth + j * s2 > bi.biHeight*bi.biWidth)
				continue;
			NODE temp = p[i*s1*bi.biWidth + j * s2];
			for (int k = 0; k < s1; k++) {
				for (int l = 0; l < s2; l++) {
					if ((i*s1 + k)*bi.biWidth + j * s2 + l > bi.biHeight*bi.biWidth)
						continue;
					p[(i*s1 + k)*bi.biWidth + j * s2 + l] = temp;
				}
			}
		}
	}
}

unsigned char change(unsigned char temp, int gery) {
	int s = 256 / gery;
	unsigned char res = ((2 * (temp / s) + 1)*s - 1) / 2;
	return res;
}

void changeGrey(int grey, NODE *p, BITMAPINFOHEADER bi) {
	for (int j = 0; j < bi.biHeight; j++) {
		for (int i = 0; i < bi.biWidth; i++) {
			if (!(p->b == p->g && p->b == p->r)) {
				unsigned char temp = 0.114*p->b + 0.587*p->g + 0.2999*p->r;
				p->b = p->g = p->r = temp;
			}
			p->b = change(p->b, grey);
			p->g = change(p->g, grey);
			p->r = change(p->r, grey);
			p++;
		}
	}
}