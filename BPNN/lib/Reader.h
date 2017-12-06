#ifndef AROYA_READER
#define AROYA_READER

#include<vector>
using namespace std;

class Reader {
public:
	Reader(const char*);
	Reader(const char*filename, const int&cols);
	//~Reader();
	vector<double*>data;
	vector<int>flag;
	double persents(double*, bool display = 0);
	int columns, rows;
};

//�������е����ڷֿ�
void dateprocess(const char*input,const char*output);
//ֱ��ɾ������
void deleteFirstLine(const char*input,const char*output);
//���ļ����ź�'/'ת��Ϊ�ո� ����ÿ�е�Ԫ����
int transform(const char*input,const char*output);

#endif