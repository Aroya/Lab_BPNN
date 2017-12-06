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
	int valiCols = transform("validation.csv", "vali.csv");
	//int testCols = transform("dfl2.csv", "test.csv");
	Reader train("tra.csv", trainCols);
	Reader vali("vali.csv", valiCols);

	double **trainData = new double*[train.rows];
	for (int i = 0; i < train.rows; i++)trainData[i] = train.data[i];
	double **trainFlag = new double*[train.flag.size()];
	for (int i = 0; i < train.flag.size(); i++) {
		trainFlag[i] = new double[1] {double(train.flag[i])};
	}
	int *layerNodes = new int[3] {trainCols,trainCols, 1};


	//build bpnn
	int layers = 4;
	BPNN bpnn(layers);
	bpnn.setInputNodes(trainCols);
	bpnn.setLayerNodes(layerNodes);
	for (int i = 0; i < 1000; i++) {
		bpnn.runGroup(trainData, trainFlag, train.rows, sigmoid, sigmoidD);
	}
	


	//last line
	system("pause");
}