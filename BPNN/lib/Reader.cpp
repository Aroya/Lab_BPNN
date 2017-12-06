#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>

#include"Reader.h"
using namespace std;

static const char processedDate[] = "processDate.tmp";
static const char transformedFile[] = "transform.tmp";
static const char firstLineDeleted[] = "delete.tmp";

////将数据中的日期分开
//void dateprocess(const char*);
////直接删除首行
//void deleteFirstLine(const char*);
////将文件逗号和'/'转换为空格 返回每行的元素数
//int transform(const char*);


static int alreadyTest = 0;

Reader::Reader(const char*filename) {
	double *temp;
	//for reading
	int this_flag;
	double this_scalar;
	int this_column = 0;
	int this_row = -1;

	//dateprocess(filename);
	deleteFirstLine(filename,firstLineDeleted);
	columns = transform(firstLineDeleted,transformedFile);
	string sst;

	ifstream fin;
	fin.open(transformedFile);


	if(fin.is_open()){
		while (fin.good() && fin >> sst) {
			if (this_column == 0) {
				temp = new double[columns];
				data.push_back(temp);
				this_row++;
				data[this_row][this_column++] = 1.0;
				stringstream ss(sst);
				ss >> this_scalar;
				data[this_row][this_column++] = this_scalar;
			}
			else if (this_column < columns) {
				stringstream ss(sst);
				ss >> this_scalar;
				data[this_row][this_column++] = this_scalar;
			}
			else {
				this_column = 0;
				stringstream ss(sst);
				ss >> this_flag;
				flag.push_back(this_flag);
			}
		}
	}
	else {
		cout<<"Reader cannot open file!\n";
		system("pause");
	}
	rows = data.size();
}


Reader::Reader(const char*filename,const int&cols) {
	double *temp;
	//for reading
	int this_flag;
	double this_scalar;
	int this_column = 0;
	int this_row = -1;

	columns = cols;
	string sst;

	ifstream fin;
	fin.open(filename);


	if (fin.is_open()) {
		while (fin.good() && fin >> sst) {
			if (this_column == 0) {
				temp = new double[columns];
				data.push_back(temp);
				this_row++;
				data[this_row][this_column++] = 1.0;
				stringstream ss(sst);
				ss >> this_scalar;
				data[this_row][this_column++] = this_scalar;
			}
			else if (this_column < columns) {
				stringstream ss(sst);
				ss >> this_scalar;
				data[this_row][this_column++] = this_scalar;
			}
			else {
				this_column = 0;
				stringstream ss(sst);
				ss >> this_flag;
				flag.push_back(this_flag);
			}
		}
	}
	else {
		cout << "Reader cannot open file!\n";
		system("pause");
	}
	rows = data.size();
}


int transform(const char* t,const char*out) {
	int ans = 0;
	ofstream fout;
	ifstream fin;
	fin.open(t);
	fout.open(out);
	if (fin.is_open() && fout.is_open()) {
		char reading;
		bool first = 1;
		while (fin.good() && (reading=fin.get()) != -1) {
			if (reading == ','||reading=='/') {
				fout << ' ';
				if (first)ans++;
			}
			else if (first && reading == '\n') {
				ans++;
				fout << reading;
				first = 0;
			}
			else fout << reading;
		}
	}
	else {
		cout << "Transform cannot open file!\n";
		system("pause");
	}
	fout.close();
	fin.close();
	return ans;
}

double Reader::persents(double* t, bool display) {
	double dot;

	//评测指标
	int tp, fn, tn, fp;
	tp = 0; fn = 0; tn = 0; fp = 0;
	for (int i = 0; i < rows; i++) {
		dot = 0;
		for (int j = 0; j < columns; j++) {
			dot += (t[j] * data[i][j]);
		}
		dot = 1.0 / (1.0 + exp(-dot));
		if (dot > 0.5 && flag[i] == 1)tp++;
		if (dot > 0.5 && flag[i] == 0)fn++;
		if (dot < 0.5 && flag[i] == 0)tn++;
		if (dot < 0.5 && flag[i] == 1)fp++;
	}
	//评测指标
	double accurary = double(tp + tn) / double(tp + fn + tn + fp);
	//double recall = double(tp) / double(tp + fn);
	//double precision = double(tp) / double(tp + fp);
	//double f1 = 2 * precision*recall / (precision + recall);

	if (display) {
		//cout << "Accuray:" << accurary << "\t第" << alreadyTest++ << "次测试\n";
		cout << accurary << endl;
		//cout << "Recall:" << recall << endl;
		//cout << "Precision:" << precision << endl;
		//cout << "F1:" << f1 << endl;
	}
	return accurary;
}

void dateprocess(const char*t,const char*out) {
	ofstream fout;
	ifstream fin;
	fin.open(t);
	fout.open(out);
	if (!fin.is_open()) {
		cout << "Cannot open" << processedDate << endl;
		system("pause");
	}
	if (!fout.is_open()) {
		cout << "Cannot open" << t << endl;
		system("pause");
	}
	string buffer;
	fin >> buffer;
	//instant,dteday
	//instant,d,m,dy
	buffer[8] = 'y';
	buffer[9] = ',';
	buffer[10] = 'm';
	buffer[11] = ',';
	buffer[12] = 'd';
	fout << buffer;
	while (fin >> buffer)fout << buffer;
	fout.close();
}

void deleteFirstLine(const char*t,const char*out) {
	ofstream fout;
	ifstream fin;
	fin.open(t);
	fout.open(out);
	if (!fin.is_open()) {
		cout << "Cannot open" << processedDate << endl;
		system("pause");
	}
	if (!fout.is_open()) {
		cout << "Cannot open" << t << endl;
		system("pause");
	}
	string buffer;
	fin >> buffer;
	while (fin >> buffer)fout << buffer;
	fout.close();
}