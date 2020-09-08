#ifndef _BMPDEF_H_
#define _BMPDEF_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

class BITMAPFILEHEADER {
public:
	WORD bfType;
	DWORD bfSize; //�ļ���С
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits; //����������ļ���ʼλ�õ�ƫ����
};
class BITMAPINFOHEADER {
public:
	DWORD biSize;
	LONG biWidth; //ͼ���ȣ����أ�
	LONG biHeight; //ͼ��߶ȣ����أ�
	WORD biPlanes;
	WORD biBitCount; //��ɫλ��
	DWORD biCompression; 
	DWORD biSizeImage; //��������С���ֽڣ�
	LONG biXPelsPerMeter; //���������ܶ�
	LONG biYPelsPerMeter; //���������ܶ�
	DWORD biClrUsed;
	DWORD biClrImportant;
};

#endif