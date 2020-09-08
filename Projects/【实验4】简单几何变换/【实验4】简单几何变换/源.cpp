#include<iostream>
#include<string>
#include<math.h>
#include<vector>
#include"bmp.h"
using namespace std;

#define pi 3.14159

//����ԭ������
BYTE* GetPixel(RGBInfo* info, int x, int y, int layer);
int* GetPixel(int* data, int width, int x, int y, int layer);
RGBInfo Translation(RGBInfo* Origin, int DeltaX, int DeltaY);
RGBInfo Rotation(RGBInfo* Origin, double theta);
void Bilinear(int* data, int width, int height, int x, int y);
void BubbleSort(int arr[], int size);
int NewX_r(int OriginX, int OriginY, int deltaX, double theta);
int NewY_r(int OriginX, int OriginY, int deltaY, double theta);
int OriginX_r(int NewX, int NewY, int deltaX, int deltaY, double theta);
int OriginY_r(int NewX, int NewY, int deltaX, int deltaY, double theta);
bool isHole(RGBInfo *info, int *data, int NewW, int NewX, int NewY, int deltaX, int deltaY, double theta);
RGBInfo Scale(RGBInfo *origin, double CoefX, double CoefY);
RGBInfo Shear(RGBInfo *origin, char axis, double Coef);
RGBInfo Mirror(RGBInfo *origin, char axis);

int main() {
	cout << "(C)2019, Gordon Freeman" << endl;
	string name;
	cout << "���������ڶ�BMPͼ����м򵥵ļ��α任��" << endl;
	cout << "�û���Ҫ�����任��ͼ�����ڳ����ͬ��Ŀ¼�£������������׺�����ڵ������ļ�����" << endl;
	cout << "���ڣ��������������ļ�����" << endl;
	RGBInfo origin;
	while (true) {
		getline(cin, name);
		origin = imread_RGB(name.c_str());
		if (origin.width_byte == 0) {
			cout << "�ļ������ڣ������������ļ�����" << endl;
		}
		else break;
	}
	BYTE* OriginData = origin.data;
	cout << "������Ҫִ�еĲ�����" << endl;
	cout << "��ע�⣺�ظ�ִ��ͬ���Ͳ�����������һ�εõ����ļ���" << endl;
	cout << "1)ƽ�� 2)���� 3)���� 4)���� 5)��ת 0)�˳�" << endl;
	int op;
	while (true) {
		scanf("%d", &op);
		//ƽ��
		if (op == 1) {
			int dx, dy;
			cout << "���������������ƶ����루���أ����м���һ���ո�ָ���" << endl;
			cout << "��ʾ�����븺ֵ������ / �����ƶ���" << endl;
			scanf("%d %d", &dx, &dy);
			RGBInfo translated = Translation(&origin, dx, dy);
			imwrite(translated, "Translated Image.bmp");
			delete[] translated.data;
			cout << "ƽ�Ʊ任��ɡ�" << endl;
		}
		//����
		else if (op == 2) {
			char axis;
			cout << "�����뾵��任�ķ�����X��Y��ʾ��:" << endl;
			rewind(stdin);
			scanf("%c", &axis);
			RGBInfo mirrored = Mirror(&origin, axis);
			imwrite(mirrored, "Mirrored Imgae.bmp");
			delete[] mirrored.data;
			cout << "����任��ɡ�" << endl;
		}
		//����
		else if (op == 3) {
			char axis;
			double Coef;
			cout << "��������б任�ķ�����X��Y��ʾ��������ϵ�����м���һ���ո�ָ���" << endl;
			cout << "��ʾ�����븺ֵ������ / ����ƫ�ơ�" << endl;
			rewind(stdin);
			scanf("%c %lf", &axis, &Coef);
			RGBInfo sheared = Shear(&origin, axis, Coef);
			imwrite(sheared, "Sheared Image.bmp");
			delete[] sheared.data;
			cout << "���б任��ɡ�" << endl;
		}
		//����
		else if (op == 4) {
			double cx, cy;
			cout << "��ֱ�����������������ϵ�����м���һ���ո�ָ���"<<endl;
			cout << "��ʾ��ϵ������1��ʾ���죬С��1��ʾѹ����" << endl;
			scanf("%lf %lf", &cx, &cy);
			RGBInfo scaled = Scale(&origin, 2, 1);
			imwrite(scaled, "Scaled Image.bmp");
			delete[] scaled.data;
			cout << "���ű任��ɡ�" << endl;
		}
		//��ת
		else if (op == 5) {
			double angle;
			cout << "��������ת�Ƕȣ������ƣ���Ŀǰֻ֧�ִ��������롣" << endl;
			scanf("%lf", &angle);
			RGBInfo rotated = Rotation(&origin, angle);
			imwrite(rotated, "rotated image.bmp");
			delete[] rotated.data;
			cout << "��ת�任��ɡ�" << endl;
		}
		else if (op == 0) break;
		else {
			cout << "�������Ų��Ϸ������������룺" << endl;
			continue;
		}
		cout << "������Ҫִ�еĲ�����" << endl;
		cout << "��ע�⣺�ظ�ִ��ͬ���Ͳ�����������һ�εõ����ļ���" << endl;
		cout << "1)ƽ�� 2)���� 3)���� 4)���� 5)��ת 0)�˳�" << endl;
	}

	cout << "��л����ʹ�ã�)" << endl;
	system("pause");
	return 0;
}

//ȡ���غ���
//����������RGBͼ����Ϣ��ͼ����ϢӦΪ������ͬ��ָ�룬�ᡢ�������Լ�������Ϊ����
//����ֵ������ָ����ȡ����ָ�������ݵ�ָ��
//ע�⣺RGBֵ���������B,G,R��˳��洢
//     ������Ϊ�������е�һ�����ص�����Ϊ(1,1)
BYTE* GetPixel(RGBInfo* info, int x, int y, int layer) {
	BYTE* data = info->data;
	int w_byte = info->width_byte;
	int index = (y - 1)*w_byte + (x - 1) * 3 + layer - 1;
	return &data[index];
}
//����ȡ���غ�����������������Ϊ���͵������������ת�任
int* GetPixel(int* data, int w_pixel, int x, int y, int layer) {
	int index = ((y - 1)*w_pixel + x - 1) * 3 + layer - 1;
	return &data[index];
}

//ƽ�ƺ���
//����������ԭͼ��Ϣָ�룬����������ƽ������Ϊ����
//����ֵ������ƽ��ͼ����Ϣ
RGBInfo Translation(RGBInfo* Origin, int DeltaX, int DeltaY) {
	RGBInfo New(*Origin);
	int nw = New.InfoHeader.biWidth = Origin->InfoHeader.biWidth + abs(DeltaX);
	int nh = New.InfoHeader.biHeight = Origin->InfoHeader.biHeight + abs(DeltaY);
	//�������ֽ���
	New.width_byte = (nw*New.InfoHeader.biBitCount + 31) / 32 * 4;
	int nw_b = New.width_byte;
	New.data = new BYTE[nh*nw_b];
	BYTE* NewData = New.data;
	memset(NewData, 0, nh*nw_b);
	int start_X, start_Y;
	if (DeltaX <= 0) start_X = 0;
	else start_X = DeltaX;
	if (DeltaY <= 0) start_Y = 0;
	else start_Y = DeltaY;
	int x, y, layer;
	for (y = 1; y <= Origin->InfoHeader.biHeight; y++) {
		for (x = 1; x <= Origin->InfoHeader.biWidth; x++) {
			for (layer = 1; layer <= 3; layer++) {
				*GetPixel(&New, start_X + x, start_Y + y, layer) = *GetPixel(Origin, x, y, layer);
			}
		}
	}
	//�޸�����ͼ����Ϣ
	New.FileHeader.bfSize = 56 + nw_b * nh;
	New.InfoHeader.biSizeImage = nw_b * nh + 2;
	return New;
}

//��ת����
//����������ԭͼ��Ϣָ�룬��ת�Ƕȣ������ƣ���Ϊ����
//����ֵ��������תͼ����Ϣ
RGBInfo Rotation(RGBInfo* Origin, double theta) {
	RGBInfo New(*Origin);
	int oh = Origin->InfoHeader.biHeight;
	int ow = Origin->InfoHeader.biWidth;
	int ow_b = Origin->width_byte;
	//��������������С
	int arr[4];
	arr[0] = 1;
	arr[1] = round(-(oh - 1)*sin(theta) + 1);
	arr[2] = round((ow - 1)*cos(theta) + 1);
	arr[3] = round((ow - 1)*cos(theta) - (oh - 1)*sin(theta) + 1);
	BubbleSort(arr, 4);
	int nw = New.InfoHeader.biWidth = arr[3] - arr[0] + 1;
	//�������ֽ���
	int nw_b = New.width_byte = (nw*New.InfoHeader.biBitCount + 31) / 32 * 4;
	int deltaX = -arr[0] + 1; //�������Ǻ���������Ĺ������꣬��Ҫ����deltaX���ܵõ��»����µĺ�����
	arr[0] = 0;
	arr[1] = round((oh - 1) * cos(theta) + 1);
	arr[2] = round((ow - 1)*sin(theta) + 1);
	arr[3] = round((ow - 1)*sin(theta) + (oh - 1) * cos(theta) + 1);
	BubbleSort(arr, 4);
	int nh = New.InfoHeader.biHeight = arr[3] - arr[0] + 1;
	int deltaY = -arr[0] + 1; //�������Ǻ���������Ĺ������꣬��Ҫ����deltaY���ܵõ��»����µ�������
	int *TmpData = new int[nw*nh * 3];
	memset(TmpData, -1, nw*nh * 3 * sizeof(int));
	//��ԭͼͶ����ʱ������
	int ox, oy, layer;
	int nx, ny;
	for (oy = 1; oy <= oh; oy++) {
		for (ox = 1; ox <= ow; ox++) {
			for (layer = 1; layer <= 3; layer++) {
				nx = NewX_r(ox, oy, deltaX, theta);
				ny = NewY_r(ox, oy, deltaY, theta);
				*GetPixel(TmpData, nw, nx, ny, layer) = *GetPixel(Origin, ox, oy, layer);
			}
		}
	}
	//����ն�
	for (ny = 1; ny <= nh; ny++) {
		for (nx = 1; nx <= nw; nx++) {
			if (*GetPixel(TmpData, nw, nx, ny, 1) == -1 || *GetPixel(TmpData, nw, nx, ny, 2) == -1 || *GetPixel(TmpData, nw, nx, ny, 3) == -1) {
				if (isHole(Origin, TmpData, nw, nx, ny, deltaX, deltaY, theta)) {
					Bilinear(TmpData, nw, nh, nx, ny);
				}
				else {
					for (layer = 1; layer <= 3; layer++) {
						*GetPixel(TmpData, nw, nx, ny, layer) = 0;
					}
				}
			}
		}
	}
	//������������
	New.data = new BYTE[nw_b*nh];
	for (ny = 1; ny <= nh; ny++) {
		for (nx = 1; nx <= nw; nx++) {
			for (layer = 1; layer <= 3; layer++) {
				*GetPixel(&New, nx, ny, layer) = *GetPixel(TmpData, nw, nx, ny, layer);
			}
		}
		//�����ֽڲ�0
		for (nx = nw * 3 + 1; nx <= nw_b; nx++) {
			New.data[(ny - 1)*nw_b + nx - 1] = 0;
		}
	}
	delete[] TmpData;
	//�޸�����ͼ����Ϣ
	New.FileHeader.bfSize = 56 + nw_b * nh;
	New.InfoHeader.biSizeImage = nw_b * nh + 2;
	return New;
}

//˫���Բ�ֵ����
//����������������ʱ����������������/�ߣ�����������Ϊ����
//����ֵ���޷���ֵ
//ע�⣺������ʱ������ӦΪ�Ѹ�ֵ��������
void Bilinear(int* data, int width, int height, int x, int y) {
	int layer;
	//(x,y)λ�����½�
	if (x == 1 || y == 1) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = *GetPixel(data, width, x + 1, y + 1, layer);
		}
	}
	//(x,y)λ�����½�
	else if (x == width && height == 1) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = *GetPixel(data, width, x - 1, y + 1, layer);
		}
	}
	//(x,y)λ�����Ͻ�
	else if (x == 1 && y == height) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = *GetPixel(data, width, x + 1, y - 1, layer);
		}
	}
	//(x,y)λ�����Ͻ�
	else if (x == width && y == height) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = *GetPixel(data, width, x - 1, y - 1, layer);
		}
	}
	//(x,y)λ�����
	else if (x == 1) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = (*GetPixel(data, width, x + 1, y + 1, layer) + *GetPixel(data, width, x + 1, y - 1, layer)) / 2.0 + 0.5;
		}
	}
	//(x,y)λ�ڵױ�
	else if (y == 1) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = (*GetPixel(data, width, x - 1, y + 1, layer) + *GetPixel(data, width, x + 1, y + 1, layer)) / 2.0 + 0.5;
		}
	}
	//(x,y)λ���ұ�
	else if (x == width) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = (*GetPixel(data, width, x - 1, y + 1, layer) + *GetPixel(data, width, x - 1, y - 1, layer)) / 2.0 + 0.5;
		}
	}
	//(x,y)λ�ڶ���
	else if (y == height) {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = (*GetPixel(data, width, x - 1, y - 1, layer) + *GetPixel(data, width, x + 1, y - 1, layer)) / 2.0 + 0.5;
		}
	}
	//(x,y)λ���ڲ�
	else {
		for (layer = 1; layer <= 3; layer++) {
			*GetPixel(data, width, x, y, layer) = (*GetPixel(data, width, x - 1, y - 1, layer) + *GetPixel(data, width, x - 1, y + 1, layer) + *GetPixel(data, width, x + 1, y - 1, layer) + *GetPixel(data, width, x + 1, y + 1, layer)) / 4.0 + 0.5;
		}
	}
}

//ð��������������
//�����������������飬����ߴ���Ϊ����
//����ֵ���޷���ֵ
void BubbleSort(int arr[], int size) {
	int i, j;
	int temp;
	for (i = 1; i <= size - 1; i++) {
		for (j = 0; j <= size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

//���ڼ�����ת�任�У����ص����»����º�/������ĺ���
//����������ԭʼ���꣬����ϵƽ�������Լ���ת�Ƕ���Ϊ����
//����ֵ�������º�/������
int NewX_r(int OriginX, int OriginY, int deltaX, double theta) {
	return round((OriginX - 1)*cos(theta) - (OriginY - 1) * sin(theta)) + 1 + deltaX;
}
int NewY_r(int OriginX, int OriginY, int deltaY, double theta) {
	return round((OriginX - 1)*sin(theta) + (OriginY - 1) * cos(theta)) + 1 + deltaY;
}

//ȡԭ���꺯������ת���Σ�
//���������������꣬����ϵƽ�������Լ���ת�Ƕ���Ϊ����
//����ֵ������ԭʼ��/������
int OriginX_r(int NewX, int NewY, int deltaX, int deltaY, double theta) {
	int tranX = NewX - deltaX;
	int tranY = NewY - deltaY;
	return round((tranX - 1)*cos(theta) + (tranY - 1) * sin(theta)) + 1;
}
int OriginY_r(int NewX, int NewY, int deltaX, int deltaY, double theta){
	int tranX = NewX - deltaX;
	int tranY = NewY - deltaY;
	return round(-(tranX - 1) * sin(theta) + (tranY - 1) * cos(theta)) + 1;
}

//�ж���ת�任�õ�����ʱ�������е�ĳ�����Ƿ���ͼ���ڲ��ն�
//����������ԭͼ��Ϣָ�룬��ʱ����������ʱ��������ȣ������꣬����ϵƽ��������ת�Ƕ���Ϊ����
//����ֵ������ͼ���ڲ��ն��򷵻�true�����򷵻�false
bool isHole(RGBInfo *info, int *data, int NewW, int NewX, int NewY, int deltaX, int deltaY, double theta) {
	int ox = OriginX_r(NewX, NewY, deltaX, deltaY, theta);
	int oy = OriginY_r(NewX, NewY, deltaX, deltaY, theta);
	int layer;
	if (ox<1 || ox>info->InfoHeader.biWidth || oy<1 || oy>info->InfoHeader.biHeight) return false;
	else return true;
}

//���ź���
//����������ͼ����Ϣָ�룬x����/y�������ű�����Ϊ����
//����ֵ����������ͼ����Ϣ
RGBInfo Scale(RGBInfo *origin, double CoefX, double CoefY) {
	RGBInfo New(*origin);
	int ow = origin->InfoHeader.biWidth;
	int oh = origin->InfoHeader.biHeight;
	int ow_b = origin->width_byte;
	int nw = New.InfoHeader.biWidth = round(ow*CoefX);
	int nh = New.InfoHeader.biHeight = round(oh*CoefY);
	int nw_b = New.width_byte = (nw*New.InfoHeader.biBitCount + 31) / 32 * 4;
	New.data = new BYTE[nw_b*nh];
	memset(New.data, 0, nw_b*nh * sizeof(BYTE));
	int nx, ny, layer;
	int ox, oy;
	for (ny = 1; ny <= nh; ny++) {
		oy = round(ny / CoefY);
		for (nx = 1; nx <= nw; nx++) {
			ox = round(nx / CoefX);
			for (layer = 1; layer <= 3; layer++) {
				*GetPixel(&New, nx, ny, layer) = *GetPixel(origin, ox, oy, layer);
			}
		}
	}
	//�޸�����ͼ����Ϣ
	New.FileHeader.bfSize = 56 + nw_b * nh;
	New.InfoHeader.biSizeImage = nw_b * nh + 2;
	return New;
}

//���к���
//����������ͼ����Ϣָ�룬���з��򣬴���ϵ����Ϊ����
//����ֵ�����ش���ͼ����Ϣ��������з����������ԭͼ����Ϣ
RGBInfo Shear(RGBInfo *origin, char axis, double Coef) {
	RGBInfo New(*origin);
	int ow = origin->InfoHeader.biWidth;
	int oh = origin->InfoHeader.biHeight;
	int ow_b = origin->width_byte;
	int nw, nh, nw_b;
	int ox, oy, layer;
	int nx, ny;
	//�������
	if (axis == 'X' || axis == 'x') {
		int deltaX;
		nh = New.InfoHeader.biHeight = oh;
		if (Coef < 0) {
			deltaX = -round(1 + oh * Coef) + 1;
			nw = New.InfoHeader.biWidth = ow + deltaX;
		}
		else {
			deltaX = 0;
			nw = New.InfoHeader.biWidth = round(ow + oh * Coef);
		}
		nw_b = New.width_byte = (nw*New.InfoHeader.biBitCount + 31) / 32 * 4;
		New.data = new BYTE[nw_b*nh];
		memset(New.data, 0, nw_b*nh * sizeof(BYTE));
		for (oy = 1; oy <= oh; oy++) {
			ny = oy;
			for (ox = 1; ox <= ow; ox++) {
				nx = round(ox + oy * Coef) + deltaX;
				for (layer = 1; layer <= 3; layer++) {
					*GetPixel(&New, nx, ny, layer) = *GetPixel(origin, ox, oy, layer);
				}
			}
		}
	}
	//�������
	else if (axis == 'Y' || axis == 'y') {
		int deltaY;
		if (Coef < 0) {
			deltaY = -round(1 + ow * Coef) + 1;
			nh = New.InfoHeader.biHeight = oh + deltaY;
		}
		else {
			deltaY = 0;
			nh = New.InfoHeader.biHeight = round(oh + ow * Coef);
		}
		nw = New.InfoHeader.biWidth = ow;
		nw_b = origin->width_byte;
		New.data = new BYTE[nw_b*nh];
		memset(New.data, 0, nw_b*nh * sizeof(BYTE));
		for (ox = 1; ox <= ow; ox++) {
			nx = ox;
			for (oy = 1; oy <= oh; oy++) {
				ny = round(oy + Coef * ox) + deltaY;
				for (layer = 1; layer <= 3; layer++) {
					*GetPixel(&New, nx, ny, layer) = *GetPixel(origin, ox, oy, layer);
				}
			}
		}
	}
	//�޸�����ͼ����Ϣ
	New.FileHeader.bfSize = 56 + nw_b * nh;
	New.InfoHeader.biSizeImage = nw_b * nh + 2;
	return New;
}

//������
//����������ͼ����Ϣָ�룬��������Ϊ����
//����ֵ�����ؾ���ͼ����Ϣ�����뾵�����������ԭʼͼ����Ϣ
RGBInfo Mirror(RGBInfo *origin, char axis) {
	RGBInfo New(*origin);
	int w = New.InfoHeader.biWidth;
	int h = New.InfoHeader.biHeight;
	int w_b = New.width_byte;
	int ox, oy, nx, ny, layer;
	if (axis == 'X' || axis == 'x') {
		New.data = new BYTE[h*w_b];
		for (oy = 1; oy <= h; oy++) {
			ny = oy;
			for (ox = 1; ox <= w; ox++) {
				nx = w - ox + 1;
				for (layer = 1; layer <= 3; layer++) {
					*GetPixel(&New, nx, ny, layer) = *GetPixel(origin, ox, oy, layer);
				}
			}
		}
	}
	else if (axis == 'Y' || axis == 'y') {
		New.data = new BYTE[h*w_b];
		for (oy = 1; oy <= h; oy++) {
			ny = h - oy + 1;
			for (ox = 1; ox <= w; ox++) {
				nx = ox;
				for (layer = 1; layer <= 3; layer++) {
					*GetPixel(&New, nx, ny, layer) = *GetPixel(origin, ox, oy, layer);
				}
			}
		}
	}
	return New;
}