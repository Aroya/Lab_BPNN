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
	//ɾ���ո�
	int trainCols = transform("train.csv","tra.csv");
	int valiCols = transform("validation.csv", "vali.csv");
	//int testCols = transform("dfl2.csv", "test.csv");
	Reader train("tra.csv", trainCols);
	Reader vali("vali.csv", valiCols);

	//build bpnn
	int layers = 4;
	BPNN bpnn(layers);
	bpnn.setInputNodes(trainCols);
	bpnn.setLayerNodes(new int[] {trainCols, trainCols, 1});



	//last line
	system("pause");
}