#include<iostream>
#include"lib\Divider.h"
#include"lib\Reader.h"
#include"lib\BPNN.h"
using namespace std;
void main() {
	//ɾ������
	//deleteFirstLine("BPNN_Dataset\\train.csv","dfl.csv");
	//deleteFirstLine("BPNN_Dataset\\test.csv", "dfl2.csv");
	//file�ָ�
	//Divider("dfl.csv");
	//�ָ�����Ϊ�ո�
	int trainCols = transform("train.csv","tra.csv");
	//int valiCols = transform("validation.csv", "vali.csv");
	int testCols = transform("dfl2.csv", "test.csv");

	Reader train("tra.csv", trainCols);
	train.normalization(1);
	//Reader vali("vali.csv", valiCols);
	//vali.normalization(1);
	Reader test("test.csv", testCols);
	test.normalization(1);

	double **trainData = new double*[train.rows];
	for (int i = 0; i < train.rows; i++) trainData[i] = train.data[i];
	double **trainFlag = new double*[train.flag.size()];
	for (int i = 0; i < train.flag.size(); i++) {
		trainFlag[i] = new double[1] {double(train.flag[i])};
	}

	//double **valiData = new double*[vali.rows];
	//for (int i = 0; i < vali.rows; i++) valiData[i] = vali.data[i];
	//double **valiFlag = new double*[vali.flag.size()];
	//for (int i = 0; i < vali.flag.size(); i++) {
	//	valiFlag[i] = new double[1]{ double(vali.flag[i]) };
	//}

	double **testData = new double*[test.rows];
	for (int i = 0; i < test.rows; i++) testData[i] = test.data[i];

	int layers = 3;
	int *layerNodes = new int[layers-1] {trainCols-1, 1};

	//build bpnn
	BPNN bpnn(layers);
	bpnn.setInputNodes(trainCols);
	bpnn.setLayerNodes(layerNodes);
	
	for (int i = 0; i < 200; i++) {
		//bpnn.runGroup(trainData, trainFlag, train.rows, defaultActive, defaultActiveD);
		bpnn.runGroup(trainData, trainFlag, train.rows, sigmoid, sigmoidD);//Ĭ�ϲ���Ϊ1��������֤�����ѧϰ
		//bpnn.runGroup(valiData, valiFlag, vali.rows, sigmoid, sigmoidD,-1);//<0ֻ��֤���
	}
	
	bpnn.runGroup(testData, nullptr, test.rows, sigmoid, sigmoidD, 0);//=0���д���ļ�
	//bpnn.runGroup(trainData, trainFlag, train.rows, sigmoid, sigmoidD,0);

	//last line
	system("pause");
}