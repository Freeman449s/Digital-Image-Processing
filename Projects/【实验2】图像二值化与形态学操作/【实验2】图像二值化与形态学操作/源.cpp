#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"BMPDef.h"
using namespace std;

int OTSU(BYTE* data, int size);
void binarization(BYTE *data, int size, int t);
void AdaptiveBi(BYTE *data, int w, int h, int rblk, int cblk);
BYTE *dilation(BYTE *data, int w, int h, int value);
BYTE *erosion(BYTE *data, int w, int h, int value);
BYTE *opening(BYTE *data, int w, int h, int value);
BYTE *closing(BYTE *data, int w, int h, int value);
void imwrite(BITMAPFILEHEADER file, BITMAPINFOHEADER info, RGBQUAD *palette, BYTE *data, int size, const char name[]);

int main() {
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
	BYTE *GSData = new BYTE[ah*aw];
	fread(GSData, aw*ah * sizeof(BYTE), 1, fp);
	//ȫ�ֶ�ֵ��
	cout << "���ڽ���ȫ�ֶ�ֵ��...";
	BYTE *BiData = new BYTE[ah*aw];
	for (int i = 0; i <= aw * ah - 1; i++) {
		BiData[i] = GSData[i];
	}
	int treshold = OTSU(BiData, aw*ah);
	binarization(BiData, aw*ah, treshold);
	imwrite(file, info, palette, BiData, aw*ah, "BinarizedBMP.bmp");
	delete[] BiData;
	cout << "���" << endl;
	//����Ӧ��ֵ��
	cout << "���ڽ��оֲ�����Ӧ��ֵ��...";
	BYTE *AdBiData = new BYTE[ah*aw];
	for (int i = 0; i <= aw * ah - 1; i++) {
		AdBiData[i] = GSData[i];
	}
	AdaptiveBi(AdBiData, aw, ah, 2, 2);
	imwrite(file, info, palette, AdBiData, aw*ah, "AdaptiveBinarizedBMP.bmp");
	cout << "���" << endl;
	//���Ͳ���
	cout << "���ڽ������Ͳ���...";
	BYTE *DiData = dilation(AdBiData, aw, ah, 0);
	imwrite(file, info, palette, DiData, aw*ah, "DilatedBMP.bmp");
	delete[] DiData;
	cout << "���" << endl;
	//��ʴ����
	cout << "���ڽ��и�ʴ����...";
	BYTE *EroData = erosion(AdBiData, aw, ah, 0);
	imwrite(file, info, palette, EroData, aw*ah, "ErodedBMP.bmp");
	delete[] EroData;
	cout << "���" << endl;
	//������
	cout << "���ڽ��п�����...";
	BYTE *OpData = opening(AdBiData, aw, ah, 0);
	imwrite(file, info, palette, OpData, aw*ah, "OpenedBMP.bmp");
	delete[] OpData;
	cout << "���" << endl;
	//�ղ���
	cout << "���ڽ��бղ���...";
	BYTE *CloData = closing(AdBiData, aw, ah, 0);
	imwrite(file, info, palette, CloData, aw*ah, "ClosedBMP.bmp");
	delete[] CloData;
	cout << "���" << endl;
	//�ͷſռ�
	delete[] AdBiData;
	delete[] GSData;
	delete[] palette;
	fclose(fp);

	system("pause");
	return 0;
}

/*����㷨ʵ�ֺ���*/
/*����������ָ�����ͼ��Ҷ���Ϣ�Ŀռ��BYTE��ָ���Լ��ռ��С*/
/*����ֵ��ʹ����䷽��������ֵ*/
int OTSU(BYTE* data, int size) {
	int mings = 255;
	int maxgs = 0;
	for (int i = 0; i <= size - 1; i++) {
		if (data[i] > maxgs) maxgs = data[i];
		if (data[i] < mings) mings = data[i];
	}
	double avg_f, avg_b, portion_f, portion_b;
	int num_f = 0, num_b = 0, total_f = 0, total_b = 0;
	int treshold = mings;
	double var, tempvar;
	for (int gs = mings; gs <= maxgs - 1; gs++) {
		for (int i = 0; i <= size - 1; i++) {
			if (data[i] <= gs) {
				num_b++;
				total_b += data[i];
			}
			else {
				num_f++;
				total_f += data[i];
			}
		}
		portion_f = num_f / size;
		portion_b = num_b / size;
		avg_f = total_f / num_f;
		avg_b = total_b / num_b;
		tempvar = portion_f * portion_b*pow(avg_f - avg_b, 2);
		if (gs == mings) {
			treshold = mings;
			var = tempvar;
		}
		else {
			if (tempvar > var) {
				treshold = gs;
				var = tempvar;
			}
		}
	}
	return treshold;
}

/*��ֵ��ʵ�ֺ���*/
/*����������ָ�����ͼ��Ҷ���Ϣ�Ŀռ��BYTE��ָ�룬�ռ��С�Լ���ֵ*/
/*����ֵ���޷���ֵ*/
void binarization(BYTE *data, int size, int t) {
	for (int i = 0; i <= size - 1; i++) {
		if (data[i] <= t) data[i] = 0;
		else data[i] = 255;
	}
}

/*����Ӧ��ֵ������*/
/*����������ָ�����ͼ��Ҷ���Ϣ�Ŀռ��BYTE��ָ�롢ͼ���ȡ�ͼ��߶ȡ���������Լ��������*/
/*����ֵ���޷���ֵ*/
/*ע�⣺�û����뱣֤ͼ��������ܹ�������������������ܹ������������*/
void AdaptiveBi(BYTE *data, int w, int h, int rblk, int cblk) {
	BYTE *block = new BYTE[w*h / (rblk*cblk)];
	int block_it, data_it; //data_it��������data�������ĵ�����������ָʾdata�е�ǰ���ݿ��ж������Ѿ�д��block.
	int startpos; //ָʾ��ǰdata�����ݿ����ʼλ�ã����±���ʽָʾλ�ã���
	int treshold;
	for (int i = 0; i <= rblk - 1; i++) {
		for (int j = 0; j <= cblk - 1; j++) {
			startpos = i * h / rblk * w + j * w / cblk;
			block_it = 0;
			//��data�������еĿ鸳��block.
			for (data_it = 0; data_it <= h / rblk - 1; data_it++) {
				for (int k = 0; k <= w / cblk - 1; k++) {
					block[block_it++] = data[startpos + w * data_it + k];
				}
			}
			treshold = OTSU(block, h*w / (rblk*cblk));
			binarization(block, h*w / (rblk*cblk), treshold);
			//��block���ݿ�д��data.
			block_it = 0;
			for (data_it = 0; data_it <= h / rblk - 1; data_it++) {
				for (int k = 0; k <= w / cblk - 1; k++) {
					data[startpos + w * data_it + k] = block[block_it++];
				}
			}
		}
	}
	delete[] block;
}

/*���Ͳ���*/
/*����������ָ��ͼ����������ָ�롢ͼ���ȡ�ͼ��߶��Լ�Ŀ�꣨ǰ����������ֵ*/
/*����ֵ������ָ�����Ͳ�������������ָ��*/
BYTE *dilation(BYTE *data, int w, int h, int value) {
	BYTE *DiData = new BYTE[w*h];
	int pos; //ָʾ�ṹԪ����λ��
	for (int i = 0; i <= w * h; i++) {
		DiData[i] = data[i];
	}
	for (int i = 1; i <= h - 2; i++) {
		for (int j = 1; j <= w - 2; j++) {
			pos = i * w + j;
			if (data[pos] == value) {
				continue;
			}
			else if (data[pos - 1] == value || data[pos + 1] == value || data[pos - w] == value || data[pos + w] == value) {
				DiData[pos] = value;
			}
		}
	}
	for (int i = 1; i <= h - 2; i++) {
		pos = i * w;
		if (data[pos] == value);
		else if (data[pos - w] == value || data[pos + 1] == value || data[pos + w] == value) {
			DiData[pos] = value;
		}
		pos += w - 1;
		if (data[pos] == value);
		else if (data[pos - w] == value || data[pos - 1] == value || data[pos + w] == value) {
			DiData[pos] = value;
		}
	}
	for (int i = 1; i <= w - 2; i++) {
		pos = i;
		if (data[pos] == value);
		else if (data[pos - 1] == value || data[pos + 1] == value || data[pos + w] == value) {
			DiData[pos] = value;
		}
		pos += (h - 1)*w;
		if (data[pos] == value);
		else if (data[pos - 1] == value || data[pos + 1] == value || data[pos - w] == value) {
			DiData[pos] = value;
		}
	}
	if (data[0] == value);
	else if (data[w] == value || data[1] == value) DiData[0] = value;
	if (data[w - 1] == value);
	else if (data[w - 2] == value || data[2 * w - 1] == value) DiData[w - 1] = value;
	if (data[(h - 1)*w] == value);
	else if (data[(h - 1)*w + 1] == value || data[(h - 2)*w] == value) DiData[(h - 1)*w] = value;
	if (data[h*w - 1] == value);
	else if (data[h*w - 2] == value || data[(h - 1)*w - 1] == value) DiData[h*w - 1] = value;
	return DiData;
}

/*��ʴ����*/
/*����������ָ��ͼ����������ָ�롢ͼ���ȡ�ͼ��߶��Լ�Ŀ�꣨ǰ����������ֵ*/
/*����ֵ������ָ�򾭸�ʴ��������������ָ��*/
BYTE *erosion(BYTE *data, int w, int h, int value) {
	BYTE *EroData = new BYTE[w*h];
	int pos; //ָʾ�ṹԪ����λ��
	if (value) value = 0;
	else value = 255;
	for (int i = 0; i <= w * h; i++) {
		EroData[i] = data[i];
	}
	for (int i = 1; i <= h - 2; i++) {
		for (int j = 1; j <= w - 2; j++) {
			pos = i * w + j;
			if (data[pos] == value) {
				continue;
			}
			else if (data[pos - w - 1] == value || data[pos - w] == value || data[pos - w + 1] == value || data[pos - 1] == value || data[pos + 1] == value || data[pos + w - 1] == value || data[pos + w] == value || data[pos + w + 1] == value) {
				EroData[pos] = value;
			}
		}
	}
	for (int i = 1; i <= h - 2; i++) {
		pos = i * w;
		if (data[pos] == value);
		else if (data[pos - w] == value || data[pos - w + 1] == value || data[pos + 1] == value || data[pos + w] == value || data[pos + w + 1] == value) {
			EroData[pos] = value;
		}
		pos += w - 1;
		if (data[pos] == value);
		else if (data[pos - w] == value || data[pos - w - 1] == value || data[pos - 1] == value || data[pos + w - 1] == value || data[pos + w] == value) {
			EroData[pos] = value;
		}
	}
	for (int i = 1; i <= w - 2; i++) {
		pos = i;
		if (data[pos] == value);
		else if (data[pos - 1] == value || data[pos + w - 1] == value || data[pos + w] == value || data[pos + w + 1] == value || data[pos + 1] == value) {
			EroData[pos] = value;
		}
		pos += (h - 1)*w;
		if (data[pos] == value);
		else if (data[pos - 1] == value || data[pos - w - 1] == value || data[pos - w] == value || data[pos - w + 1] == value || data[pos + 1] == value) {
			EroData[pos] = value;
		}
	}
	if (data[0] == value);
	else if (data[1] == value || data[w + 1] == value || data[w] == value) EroData[0] = value;
	if (data[w - 1] == value);
	else if (data[w - 2] == value || data[2 * w - 2] == value || data[2 * w - 1] == value) EroData[w - 1] = value;
	if (data[(h - 1)*w] == value);
	else if (data[(h - 2)*w] == value || data[(h - 2)*w + 1] == value || data[(h - 1)*w + 1] == value) EroData[(h - 1)*w] = value;
	if (data[h*w - 1] == value);
	else if (data[h*w - 2] == value || data[(h - 1)*w - 2] == value || data[(h - 1)*w - 1] == value) EroData[h*w - 1] = value;
	return EroData;
}

/*������*/
/*����������ָ��ͼ����������ָ�롢ͼ���ȡ�ͼ��߶��Լ�Ŀ�꣨ǰ����������ֵ*/
/*����ֵ������ָ�򾭿���������������ָ��*/
BYTE *opening(BYTE *data, int w, int h, int value) {
	BYTE *temp;
	BYTE *OpData;
	temp = erosion(data, w, h, value);
	OpData = dilation(temp, w, h, value);
	delete[] temp;
	return OpData;
}

/*�ղ���*/
/*����������ָ��ͼ����������ָ�롢ͼ���ȡ�ͼ��߶��Լ�Ŀ�꣨ǰ����������ֵ*/
/*����ֵ������ָ�򾭱ղ�������������ָ��*/
BYTE *closing(BYTE *data, int w, int h, int value) {
	BYTE *temp;
	BYTE *CloData;
	temp = dilation(data, w, h, value);
	CloData = erosion(temp, w, h, value);
	delete[] temp;
	return CloData;
}

/*ͼƬ�������*/
/*�����������ļ�ͷ������Ϣͷ���󡢵�ɫ�̡���������ͼ��ߴ硢�ļ�����Ϊ����*/
/*����ֵ���޷���ֵ*/
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