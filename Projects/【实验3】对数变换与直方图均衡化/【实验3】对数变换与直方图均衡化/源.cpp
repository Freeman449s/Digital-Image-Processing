#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<map>
#include"BMPDef.h"
using namespace std;

double iLog(double antilog, double base);
void LogTrans(BYTE *data, int w, int h);
map<int, double> GeneMap(BYTE *data, int size);
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
	BYTE *OriData = new BYTE[ah*aw];
	fread(OriData, aw*ah * sizeof(BYTE), 1, fp);

	//�����任
	cout << "���ڽ��ж����任..." << endl;
	BYTE *data = new BYTE[aw*ah];
	for (int i = 0; i <= aw * ah - 1; i++) {
		data[i] = OriData[i];
	}
	LogTrans(data, aw, ah);
	imwrite(file, info, palette, data, aw*ah, "Logarithmic Transformed Image.bmp");
	cout << "�����任���" << endl;

	//ֱ��ͼ���⻯
	cout << "���ڽ���ֱ��ͼ���⻯..." << endl;
	cout << "����������..." << endl;
	for (int i = 0; i <= aw * ah - 1; i++) {
		data[i] = OriData[i];
	}
	cout << "ȡ��ӳ��..." << endl;
	map<int, double> NewGS = GeneMap(data, aw*ah);
	cout << "Ӧ�ø���..." << endl;
	for (int i = 0; i <= aw * ah - 1; i++) {
		data[i] = NewGS[data[i]];
	}
	imwrite(file, info, palette, data, aw*ah, "Equalized Image.bmp");
	cout << "ֱ��ͼ���⻯���" << endl;

	delete[] data;
	delete[] OriData;
	delete[] palette;
	system("pause");
	return 0;
}

//�������㺯��
//��������������antilog���base��Ϊ����
//����ֵ��������baseΪ��antilog�Ķ���
double iLog(double antilog, double base) {
	return log(antilog) / log(base);
}

//�����任����
//��������������������������ȡ��߶���Ϊ����
//����ֵ���޷���ֵ
void LogTrans(BYTE *data, int w, int h) {
	double *LogData = new double[w*h];
	int MaxV = 0;
	for (int i = 0; i <= w * h - 1; i++) {
		if (data[i] > MaxV) MaxV = data[i];
	}
	for (int i = 0; i <= w * h - 1; i++) {
		LogData[i] = iLog(data[i] + 1, MaxV + 1);
	}
	//���������캯��
	double MaxLog = 0, MinLog = 1;
	for (int i = 0; i <= w * h - 1; i++) {
		if (LogData[i] > MaxLog) MaxLog = LogData[i];
		if (LogData[i] < MinLog) MinLog = LogData[i];
	}
	double slope = 255 / (MaxLog - MinLog);
	//��������
	for (int i = 0; i <= w * h - 1; i++) {
		data[i] = (LogData[i] - MinLog)*slope + 0.5;
	}
	delete[] LogData;
}

//�Ҷ�ӳ�䴴������
//�������������������Լ��������ߴ���Ϊ����
//����ֵ������ԭʼ�Ҷ�-�»Ҷ�ӳ��
map<int, double> GeneMap(BYTE *data, int size) {
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
		it->second /= size;
	}
	//����ԭʼ�Ҷ�-�»Ҷ�ӳ��
	map<int, double> NewGS;
	map<int, double>::iterator it2;
	for (it = probabilities.begin(); it != probabilities.end(); it++) {
		if (it == probabilities.begin()) {
			NewGS[it->first] = it->second;
			it2 = NewGS.begin();
		}
		else {
			NewGS[it->first] = NewGS[it2->first] + it->second;
			it2++;
		}
	}
	for (it = NewGS.begin(); it != NewGS.end(); it++) {
		//����NewGS�д洢���»Ҷ�(double)�ᱻǿ��תΪBYTE���������0.5��ʵ������Ч��
		NewGS[it->first] = NewGS[it->first] * 255 + 0.5;
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