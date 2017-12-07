#include<iostream>
#include"lib\Divider.h"
#include"lib\Reader.h"
#include"lib\BPNN.h"
using namespace std;
void main() {
	//删除首行
	//deleteFirstLine("BPNN_Dataset\\train.csv","dfl.csv");
	//deleteFirstLine("BPNN_Dataset\\test.csv", "dfl2.csv");
	//file分割
	//Divider("dfl.csv");
	//删除空格
	int trainCols = transform("train.csv","tra.csv");
	//int valiCols = transform("validation.csv", "vali.csv");
	//int testCols = transform("dfl2.csv", "test.csv");
	Reader train("tra.csv", trainCols);
	train.normalization();
	//train.writeFile("tra_no.csv");
	//Reader vali("vali.csv", valiCols);
	//Reader vali("test.csv", testCols);

	double **trainData = new double*[train.rows];
	for (int i = 0; i < train.rows; i++) trainData[i] = train.data[i];
	double **trainFlag = new double*[train.flag.size()];
	for (int i = 0; i < train.flag.size(); i++) {
		trainFlag[i] = new double[1] {double(train.flag[i])};
	}
	int layers = 3;
	int *layerNodes = new int[layers-1] {trainCols, 1};


	//build bpnn
	
	BPNN bpnn(layers);
	bpnn.setInputNodes(trainCols);
	bpnn.setLayerNodes(layerNodes);
	for (int i = 0; i < 100; i++) {
		//bpnn.runGroup(trainData, trainFlag, train.rows, defaultActive, defaultActiveD);
		bpnn.runGroup(trainData, trainFlag, train.rows, sigmoid, sigmoidD);
	}
	bpnn.runGroup(trainData, nullptr, train.rows, sigmoid, sigmoidD,true);
	//bpnn.runGroup(trainData, trainFlag, train.rows, defaultActive, defaultActiveD,true);

	//last line
	system("pause");
}