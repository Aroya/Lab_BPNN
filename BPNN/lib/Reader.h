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

//将数据中的日期分开
void dateprocess(const char*input,const char*output);
//直接删除首行
void deleteFirstLine(const char*input,const char*output);
//将文件逗号和'/'转换为空格 返回每行的元素数
int transform(const char*input,const char*output);

#endif