#ifndef _BMPDEF_H_
#define _BMPDEF_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

class BITMAPFILEHEADER {
public:
	WORD bfType; //�ļ����ͣ�����BMP�ļ�����ֵ��Ϊ"BM",ʮ������ֵΪ42 4D��
	DWORD bfSize; //�ļ���С
	WORD bfReserved1; //�����֣�����
	WORD bfReserved2; //�����֣�����
	DWORD bfOffBits; //����������ļ���ʼλ�õ�ƫ����
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
};
class RGBQUAD {
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE reserved;
public:
	//���캯������reserved�Զ���0.
	RGBQUAD() {
		this->reserved = 0;
	}
	BYTE getR() {
		return R;
	}
	BYTE getG() {
		return G;
	}
	BYTE getB() {
		return B;
	}
	void setR(BYTE R) {
		this->R = R;
	}
	void setG(BYTE G) {
		this->G = G;
	}
	void setB(BYTE B) {
		this->B = B;
	}
};

#endif