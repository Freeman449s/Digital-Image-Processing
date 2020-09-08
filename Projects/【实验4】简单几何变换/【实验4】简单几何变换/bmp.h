#ifndef _BMP_H_
#define _BMP_H_
#include<stdio.h>

//��������
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//�ṹ������
class BITMAPFILEHEADER {
public:
	WORD bfType; //�ļ����ͣ�����BMP�ļ�����ֵ��Ϊ"BM",ʮ������ֵΪ42 4D��
	DWORD bfSize; //�ļ���С
	WORD bfReserved1; //�����֣�����
	WORD bfReserved2; //�����֣�����
	DWORD bfOffBits; //����������ļ���ʼλ�õ�ƫ����
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	BITMAPFILEHEADER() {
		bfType = 0;
		bfSize = 0;
		bfReserved1 = 0;
		bfReserved2 = 0;
		bfOffBits = 0;
	}
};
class BITMAPINFOHEADER {
public:
	DWORD biSize; //���ṹ��С����Windows����Ϊ40���ֽڣ���
	LONG biWidth; //ͼ���ȣ����أ�
	LONG biHeight; //ͼ��߶ȣ����أ�
	WORD biPlanes;
	WORD biBitCount; //��ɫλ��
	DWORD biCompression;
	DWORD biSizeImage; //��������С���ֽڣ���������ʵ�ʵ���������С��2.
	LONG biXPelsPerMeter; //���������ܶ�
	LONG biYPelsPerMeter; //���������ܶ�
	DWORD biClrUsed;
	DWORD biClrImportant;
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	BITMAPINFOHEADER() {
		biSize = 0;
		biWidth = 0;
		biHeight = 0;
		biPlanes = 0;
		biBitCount = 0;
		biCompression = 0;
		biSizeImage = 0;
		biXPelsPerMeter = 0;
		biYPelsPerMeter = 0;
		biClrUsed = 0;
		biClrImportant = 0;
	}
};
class RGBQUAD {
public:
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE reserved;
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	RGBQUAD() {
		R = G = B = reserved = 0;
	}
};
class RGBInfo {
public:
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	BYTE *data;
	int width_byte;
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	RGBInfo() {
		data = NULL;
		width_byte = 0;
	}
};
class GSInfo {
public:
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	RGBQUAD *palette;
	BYTE *data;
	int width_byte;
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	GSInfo() {
		palette = NULL;
		data = NULL;
		width_byte = 0;
	}
};

//����ԭ������
RGBInfo imread_RGB(const char name[]);
GSInfo imread_GS(const char name[]);
void imwrite(RGBInfo info, const char name[]);
void imwrite(GSInfo info, const char name[]);

#endif