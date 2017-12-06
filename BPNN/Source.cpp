#include<iostream>
#include"lib\Divider.h"
#include"lib\Reader.h"
#include"lib\BPNN.h"
using namespace std;
void main() {
	//file分割
	//Divider("BPNN_Dataset\\origin.csv");
	//删除首行
	deleteFirstLine("train.csv", "train1.csv");
	deleteFirstLine("validation.csv", "vali1.csv");
	//删除空格
	int trainCols=transform("")


	//last line
	system("pause");
}