#include<iostream>
#include<string>
#include"bmp.h"
using namespace std;

int main() {
	cout << "�˳������ڶ�ͼ�����˫���˲�����" << endl;
	cout << "������ͬ��Ŀ¼��BMPͼ����������ƣ�������׺������" << endl;
	string name;
	cin >> name;
	RGBInfo origin = imread_RGB(name.c_str());
	while (origin.data == NULL) {
		cout << "�����������ļ�����" << endl;
		cin >> name;
		origin = imread_RGB(name.c_str());
	}
	cout << "������ռ����׼����ǿ�����׼��м���һ���ո�ָ���" << endl;
	double sig_s, sig_r;
	cin >> sig_s >> sig_r;
	cout << "���ڽ���˫���˲�����...";
	RGBInfo bi = bilateral_Lab(origin, sig_s, sig_r);
	imwrite(bi, "Bilaterally Filtered.bmp");
	cout << "���" << endl;

	system("pause");
	return 0;
}