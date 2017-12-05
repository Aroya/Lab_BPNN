#include<vector>
#include<iostream>
#include<fstream>
#include<cmath>
#include<thread>
#include<ctime>
#include"Reader.h"
using namespace std;
#include"LR.h"

void cacWeight(thread_bag*);
void cacErr(thread_bag2*tb);

Omega::Omega(const int& cols) {
	nowPers = 0;
	bestPer = 0;
	columns = cols;
	data = new double[columns];
	best = new double[columns];
	for (int i = 0; i < columns; i++) {
		data[i] = 0.0;
	}
}
//from file

Omega::Omega() {
	ifstream fin;
	fin.open("Omega.tmp");
	if (fin.is_open()) {
		fin >> nowPers;
		fin >> columns;
		data = new double[columns];
		int i = 0;
		double reading;
		while (fin.good() && (fin >> reading)) {
			data[i++] = reading;
		}
		fin.close();
	}
	else {
		cout << "Omega cannot open \"Omega.tmp\"!\n";
		system("pause");
	}

	fin.open("Omega_best.tmp");
	if (fin.is_open()) {
		fin >> bestPer;
		cout << "读取到的最优值:" << bestPer << endl;
		fin >> columns;
		best = new double[columns];
		int i = 0;
		double reading;
		while (fin.good() && (fin >> reading)) {
			best[i++] = reading;
		}
		fin.close();
	}
	else {
		cout << "Omega cannot open \"Omega_best.tmp\"!\n";
		system("pause");
	}
}

void Omega::writeFile() {
	ofstream fout;
	fout.open("Omega.tmp");
	if (fout.is_open()) {
		fout << nowPers << ' ' << columns;
		for (int i = 0; i < columns; i++) {
			fout << ' ' << data[i];
		}
		fout.close();
	}
	else {
		cout << "Omega cannot open file!\n";
		system("pause");
	}
	fout.open("Omega_best.tmp");
	if (fout.is_open()) {
		
		//存储最优W
		//fout << bestPer << ' ' << columns;
		//cout << "存储的最优值:" << bestPer << endl;
		//for (int i = 0; i < columns; i++) {
		//	fout << ' ' << best[i];
		//}
		//存储普通W
		fout << nowPers << ' ' << columns;
		cout << "存储的W:" << nowPers << endl;
		for (int i = 0; i < columns; i++) {
			fout << ' ' << data[i];
		}
		fout.close();
	}
	else {
		cout << "Omega cannot open file!\n";
		system("pause");
	}
}

void Omega::iterate(Reader& reader, const int&times) {
	for (int i = 0; i < times; i++)	iterateOnce(reader);
}

void Omega::iterateOnce(Reader& reader) {
	//to do
	//计数器
	int thisCol, thisRow, cols, rows;
	cols = reader.columns;
	rows = reader.rows;
	//权重
	double *weight = new double[rows];
	//Err
	double *err = new double[cols];
#ifdef TIME
	//时间
	time_t nowTime = clock();
#endif
#ifndef MULTI_THREADS
	//首先计算每一样例的权重
	for (thisRow = 0; thisRow < rows; thisRow++) {
		weight[thisRow] = 0;
		for (thisCol = 0; thisCol < cols; thisCol++) {
			weight[thisRow] += data[thisCol] * reader.data[thisRow][thisCol];
		}
	}
	//计算每一列的Err
	for (thisCol = 0; thisCol < cols; thisCol++) {
		err[thisCol] = 0;
		for (thisRow = 0; thisRow < rows; thisRow++) {
			err[thisCol] += (1.0 / (1.0 + exp(-weight[thisRow])) - double(reader.flag[thisRow]))
				*reader.data[thisRow][thisCol];
		}
	}
#endif // !MULTI_THREADS
#ifdef MULTI_THREADS
	//首先计算每一样例的权重
	int rowsCut4 = rows / 4, rowsStep;
	thisRow = 0;
	rowsStep = thisRow + rowsCut4;
	thread_bag tb0(&reader, data, weight, cols, thisRow, rowsStep);
	thread weight0(cacWeight, &tb0);

	thisRow = rowsStep;
	rowsStep = thisRow + rowsCut4;
	thread_bag tb1(&reader, data, weight, cols, thisRow, rowsStep);
	thread weight1(cacWeight, &tb1);

	thisRow = rowsStep;
	rowsStep = thisRow + rowsCut4;
	thread_bag tb2(&reader, data, weight, cols, thisRow, rowsStep);
	thread weight2(cacWeight, &tb2);
	
	thisRow = rowsStep;
	thread_bag tb3(&reader, data, weight, cols, thisRow, rows);
	thread weight3(cacWeight, &tb3);
	weight0.join();
	weight1.join();
	weight2.join();
	weight3.join();
	//计算每一列的Err
	int colsCut4 = cols / 4, colsStep;
	thisCol = 0;
	thisCol = 0;
	colsStep = thisCol + colsCut4;
	thread_bag2 tb20(&reader, data, weight, err, rows, thisCol, colsStep);
	thread err0(cacErr, &tb20);

	thisCol = colsStep;
	colsStep = thisCol + colsCut4;
	thread_bag2 tb21(&reader, data, weight, err, rows, thisCol, colsStep);
	thread err1(cacErr, &tb21);

	thisCol = colsStep;
	colsStep = thisCol + colsCut4;
	thread_bag2 tb22(&reader, data, weight, err, rows, thisCol, colsStep);
	thread err2(cacErr, &tb22);

	thisCol = colsStep;
	thread_bag2 tb23(&reader, data, weight, err, rows, thisCol, cols);
	thread err3(cacErr, &tb23);

	err0.join();
	err1.join();
	err2.join();
	err3.join();

#endif
#ifdef TIME
	cout << "计算权值与Err使用时间:\t" << clock() - nowTime << "ms\n";
#endif
	//得到nextData
	//动态step 分阶段step
	double n;
	//if (nowPers < 0.75) {
	//	 n = (1 - nowPers) / 10.0;
	//}
	//else if (nowPers < 0.775)n = (1 - nowPers) / 1000.0;
	//else n = (1 - nowPers) / 11500.0;

	//静态步长
	n = 1;
	for (thisCol = 0; thisCol < cols; thisCol++) {
		data[thisCol] = data[thisCol] - n*err[thisCol];
	}
	nowPers = reader.persents(data, 1);
	if (nowPers > bestPer) {
		for (thisCol = 0; thisCol < cols; thisCol++)best[thisCol] = data[thisCol];
		bestPer = nowPers;
	}
	delete[]weight;
	delete[]err;
}
void Omega::writeTest(const vector<double*>&test) {
	int testSize = test.size();
	int *writeData = new int[testSize];
	double sum;
	for (int i = 0; i < testSize; i++) {
		sum = 0;
		for (int j = 0; j < columns; j++) {
			sum += (test[i][j] * best[j]);//使用最好的记录
		}
		//double temp = 1.0 / (1.0 + exp(-sum));
		writeData[i] = (1.0 / (1.0 + exp(-sum)) > 0.5 ? 1 : 0);
	}
	ofstream fout;
	fout.open("15352014_Caizheng_LR.csv");
	if (fout.is_open()) {
		for (int i = 0; i < testSize; i++)fout << writeData[i] << endl;
		fout.close();
	}
	else {
		cout << "WriteTest cannot open file!\n";
		system("pause");
	}
	if (writeData)delete[]writeData;
}
void cacWeight(thread_bag*tb) {
	int thisRow, thisCol;
	for (thisRow = tb->begin; thisRow < tb->end; thisRow++) {
		tb->weight[thisRow] = 0;
		for (thisCol = 0; thisCol < tb->cols; thisCol++) {
			tb->weight[thisRow] += tb->data[thisCol] * tb->reader->data[thisRow][thisCol];
		}
	}
}
void cacErr(thread_bag2*tb) {
	int thisCol, thisRow;
	for (thisCol = tb->begin; thisCol < tb->end; thisCol++) {
		tb->err[thisCol] = 0;
		for (thisRow = 0; thisRow < tb->rows; thisRow++) {
			tb->err[thisCol] += (1.0 / (1.0 + exp(-tb->weight[thisRow])) - double(tb->reader->flag[thisRow]))*tb->reader->data[thisRow][thisCol];
		}
	}
}