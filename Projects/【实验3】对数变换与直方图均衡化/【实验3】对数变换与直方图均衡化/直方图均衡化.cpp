#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<map>
#include"BMPDef.h"
using namespace std;

void imwrite(BITMAPFILEHEADER file, BITMAPINFOHEADER info, RGBQUAD *palette, BYTE *data, int size, const char name[]);

int main(void) {
	BITMAPFILEHEADER file;
	BITMAPINFOHEADER info;
	FILE *fp;
	fp = fopen("GrayScaleBMP.bmp", "rb");
	//��ȡBMP�ļ���Ϣ
	fread(&(file.bfType), sizeof(WORD), 1, fp);
	fread(&(file.bfSize), sizeof(DWORD), 1, fp);
	fread(&(file.bfReserved1), sizeof(WORD), 1, fp);
	fread(&(file.bfReserved2), sizeof(WORD), 1, fp);
	fread(&(file.bfOffBits), sizeof(DWORD), 1, fp);
	fread(&info, sizeof(BITMAPINFOHEADER), 1, fp);
	RGBQUAD *palette = new RGBQUAD[256];
	fread(palette, 256 * sizeof(RGBQUAD), 1, fp);
	int ah = info.biHeight;
	int aw = (info.biWidth*info.biBitCount + 31) / 32 * 32 / info.biBitCount;
	BYTE *data = new BYTE[ah*aw];
	fread(data, aw*ah * sizeof(BYTE), 1, fp);

	//ֱ��ͼ���⻯
	map<int, int> NewGS = GeneMap(data, aw*ah);
	for (int i = 0; i <= aw * ah - 1; i++) {
		data[i] = NewGS[i];
	}
	imwrite(file, info, palette, data, aw*ah, "Equalized Imgae.bmp");

	delete[] data;
	delete[] palette;
	return 0;
}

//�Ҷ�ӳ�䴴������
//�������������������Լ��������ߴ���Ϊ����
//����ֵ�����ػҶ�-�»Ҷ�ӳ��
map<int, int> GeneMap(BYTE *data, int size) {
	//����ԭʼ�Ҷ�-����ӳ��
	map<int, double> probabilities;
	for (int i = 0; i <= size - 1; i++) {
		if (probabilities.count(data[i]) == 0) {
			probabilities[data[i]] = 1;
		}
		else probabilities[data[i]]++;
	}
	map<int, double>::iterator it;
	for (it = probabilities.begin(); it != probabilities.end(); it++) {
		it->second /= (size);
	}
	//����ԭʼ�Ҷ�-�»Ҷ�ӳ��
	map<int, double> temp;
	map<int, double>::iterator it2;
	for (it = probabilities.begin(); it != probabilities.end(); it++) {
		if (it == probabilities.begin()) {
			temp[it->first] = it->second;
			it2 = temp.begin();
		}
		else {
			temp[it->first] = temp[it2->first] + it->second;
			it2++;
		}
	}
	map<int, int> NewGS;
	for (it = temp.begin(); it != temp.end(); it++) {
		NewGS[it->first] = temp[it->first] * 255 + 0.5;
	}
	return NewGS;
}

//�Ҷ�ͼƬ�������
//�����������ļ�ͷ������Ϣͷ���󡢵�ɫ�̡���������ͼ��ߴ硢�ļ�����Ϊ����
//����ֵ���޷���ֵ
void imwrite(BITMAPFILEHEADER file, BITMAPINFOHEADER info, RGBQUAD *palette, BYTE *data, int size, const char name[]) {
	FILE *fp;
	fp = fopen(name, "wb");
	fwrite(&(file.bfType), 1, sizeof(file.bfType), fp);
	fwrite(&(file.bfSize), 1, sizeof(file.bfSize), fp);
	fwrite(&(file.bfReserved1), 1, sizeof(file.bfReserved1), fp);
	fwrite(&(file.bfReserved2), 1, sizeof(file.bfReserved2), fp);
	fwrite(&(file.bfOffBits), 1, sizeof(file.bfOffBits), fp);
	fwrite(&info, 1, sizeof(info), fp);
	fwrite(palette, 1, 256 * sizeof(RGBQUAD), fp);
	fwrite(data, 1, size * sizeof(BYTE), fp);
	fclose(fp);
}