#include"BPNN.h"
#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

static double instant = 50;
static double lastLayerK = 1;

double defaultActive(const double&t) { return t; }
double defaultActiveD(const double&t) { return 1.0; }
double sigmoid(const double&t) { return 1.0 / (1.0 + exp(-t/instant)); }
double sigmoidD(const double&t) { 
	//double v = t;
	//if (v > 2)v = 2;
	//if (v < 2)v = -2;
	
	return 1.0 / (2.0 + exp(t/instant) + exp(-t/instant));
}

BPNN::BPNN(const int&Layers) {
	layers = Layers;
	layerW = new double**[Layers];
	fixW = new double**[Layers];
	layerData = new double*[Layers];
	activation = new double*[Layers];
	layerNodes = new int[Layers];
	bias = new double*[Layers];
	fixBias = new double*[Layers];
	expected = new double*[Layers];
	//inputs don't have W
	layerW[0] = nullptr;
	for (int i = 0; i < Layers; i++)layerNodes[i] = 0;
}
void BPNN::setInputNodes(const int&Nodes) {
	layerNodes[0] = Nodes;
	layerData[0] = new double[Nodes];
	//last term for bias
	//layerData[0][Nodes] = 1;
	
	//first columns are input
	//don't have bias
	bias[0] = nullptr;
	fixBias[0] = nullptr;
	//expected[0] = nullptr;
	//activation[0] = nullptr;
	layerW[0] = nullptr;
	fixW[0] = nullptr;

	//to not judge input layer
	//create them
	activation[0] = new double[Nodes];
	expected[0] = new double[Nodes];
}
void BPNN::setLayerNodes(int*Nodes) {
	int i, j, k, l, nodeReader, nodeReader2;
	for (i = 1, j = 0; i < layers; i++, j++) {
		layerNodes[i] = Nodes[j];
		nodeReader = Nodes[j];
		nodeReader2 = layerNodes[j];
		layerW[i] = new double*[nodeReader];
		fixW[i] = new double*[nodeReader];
		layerData[i] = new double[nodeReader];
		activation[i] = new double[nodeReader];
		bias[i] = new double[nodeReader];//new bias
		fixBias[i] = new double[nodeReader];
		//layerData[i][layerNodes[i]] = 1;//bias
		expected[i] = new double[nodeReader];
		for (k = 0; k < nodeReader; k++) {
			//W depends on the nodes before
			layerW[i][k] = new double[nodeReader2];
			fixW[i][k] = new double[nodeReader2];
			//init bias
			bias[i][k] = 1;
			for (l = 0; l < nodeReader2; l++) {
				layerW[i][k][l] = 1;
			}
		}
	}
}
void BPNN::setInputData(double*Data, double(*active)(const double&)) {
	int nodesReader = layerNodes[0];
	for (int i = 0; i < nodesReader; i++) {
		//layerData[0][i] = Data[i];
		//activation[0][i] = active(Data[i]);
		activation[0][i] = Data[i];
	}
}

void BPNN::clearData() {
	int i, j, k, p, q, m;
	for (i = 1, p = 0; i < layers; i++, p++) {
		j = layerNodes[i];
		m = layerNodes[p];
		for (k = 0; k < j; k++) {
			//layerData[i][k] = 0;
			expected[i][k] = 0;
			activation[i][k] = 0;
			fixBias[i][k] = 0;
			for (q = 0; q < m; q++) {
				fixW[i][k][q] = 0;
			}
		}
	}
}
void BPNN::updateLayers(double(*active)(const double&)) {
	int i, j, k, l, nodeReader, nodeReader2;
	double sum;
	for (i = 1, j = 0; i < layers; i++, j++) {
		nodeReader = layerNodes[i];
		nodeReader2 = layerNodes[j];
		for (k = 0; k < nodeReader; k++) {
			sum = 0;
			for (l = 0; l < nodeReader2; l++) {
				//sum += layerData[j][l] * layerW[i][k][l];
				sum += (activation[j][l] * layerW[i][k][l]);
			}
			//bias
			sum += bias[i][k];
			layerData[i][k] = sum;
			//if (i == layers - 1)system("pause");
			activation[i][k] = active(sum);
#ifdef  ShowAllNode//实验结果展示用
			cout << "layer:" << i << "\tnode:" << k << "\tactivation:" << activation[i][k] << endl;
#endif //  ShowAllNode
		}
	}
	//最后一层
	i = layers - 1;
	nodeReader = layerNodes[i];
	for (j = 0; j < nodeReader; j++) {
		activation[i][j] = lastLayerK*layerData[i][j];
	}
}
void BPNN::setExpectData(double* Data,double(*active)(const double&)) {
	int i = layers - 1;
	int j = layerNodes[i];
	for (int k = 0; k < j; k++) {
		expected[i][k] = Data[k] - activation[i][k];
#ifdef ShowAllNode//展示实验结果
		cout << "k:" << i << "\tnode:" << k << "\tdiff:" << expected[i][k] << endl;
#endif // ShowAllNode
	}
}
void BPNN::updateParameter(double(*activeD)(const double&)) {
	int i, j, k, l, nodesReader, nodesReader2;
	double biasDiff;
	for (i = layers - 1, k = layers - 2; i > 0; i--, k--) {
		nodesReader = layerNodes[i];
		nodesReader2 = layerNodes[k];
		//clear data before
		for (l = 0; l < nodesReader2; l++) {
			expected[k][l] = 0;
		}
		for (j = 0; j < nodesReader; j++) {
			//bias
			//biasDiff = 2.0 * (expected[i][j] - activation[i][j])*activeD(layerData[i][j]);
			if (i == layers - 1) {
				biasDiff = 2.0 * expected[i][j] * lastLayerK* layerData[i][j];
			}
			else biasDiff = 2.0 * expected[i][j]*activeD(layerData[i][j]);
#ifdef ShowAllNode//展示实验结果
			biasDiff = 2.0 * expected[i][j] * activeD(layerData[i][j]);
			cout << "layer:" << i << "\tnode:" << j << "\tbiasDiff:" << biasDiff << endl;
			cout << activation[i][j] << '\t' << activeD(layerData[i][j]) << endl;
#endif // ShowAllNode

			fixBias[i][j] += biasDiff;
			//W_i & X_i
			for (l = 0; l < nodesReader2; l++) {
				fixW[i][j][l] += biasDiff*activation[k][l];
				
				expected[k][l] += biasDiff*layerW[i][j][l];
				//cout << fixW[i][j][l] << ' ' << expected[k][l] << endl;
			}
		}
		//average expected before
		for (l = 0; l < nodesReader2; l++) {
			expected[k][l] /= i;
		}

		//learning W_i and bias
		//after run all train
		//this function process one train
	}
}
void BPNN::runGroup(double**group, double**flag,const int&groups,
	double(*active)(const double&), double(*activeD)(const double&),
	int writeFile) {
	if (writeFile) {
		clearData();
		loss = 0;
		//forward = 0;
		for (int i = 0; i < groups; i++) {
			setInputData(group[i],active);
			updateLayers(active);
			setExpectData(flag[i], active);
			if(writeFile>0)updateParameter(activeD);
			for (int j = 0; j < layerNodes[layers - 1]; j++) {
				//forward += flag[i][j] - layerData[layers - 1][j];
				loss += pow(flag[i][j] - activation[layers - 1][j], 2);
			}
		}
		
		if (writeFile > 0) {//>0训练
			cout << (loss /= double(groups)) << endl;
			learn(groups);
		}
		else {//<0验证
			cout<< ','<<(loss /= double(groups)) << endl;
		}
	}
	else {//==0测试
		ofstream fout;
		fout.open("result.file");
		clearData();
		for (int i = 0; i < groups; i++) {
			setInputData(group[i],active);
			updateLayers(active);
			for (int i = 0; i < layerNodes[layers - 1]; i++) {
				fout << activation[layers - 1][i] << endl;
			}
		}
		fout.close();
	}
}
void BPNN::learn(const int&groups) {
	int i, j, k, l, nodesReader, nodesReader2;
	for (i = 1, j = 0; i < layers; i++, j++) {
		nodesReader = layerNodes[i];
		nodesReader2 = layerNodes[j];

		for (k = 0; k < nodesReader; k++) {
			//bias
			bias[i][k] += (dynamicRate()*fixBias[i][k] / groups);
#ifdef ShowAllNode//展示实验结果
			cout << "layer:" << i << "\tnode:" << k << "\tfixBias:" <<
				dynamicRate()*fixBias[i][k] / groups << endl;
			cout << "fixedBias:" << bias[i][k] << endl;
#endif // ShowAllNode
			//W_i
			for (l = 0; l < nodesReader2; l++) {
				layerW[i][k][l] += (dynamicRate()*fixW[i][k][l] / groups);
#ifdef ShowAllNode//展示实验结果
				cout << "layer:" << i << "\tnode:" << k << "\tWi:" << l << "\tfixW:" << 
					dynamicRate()*fixW[i][k][l] / groups << endl;
				cout << "fixedWi:" << layerW[i][k][l] << endl;
#endif
				

			}
		}
	}
}
double BPNN::dynamicRate() {
#ifdef ShowAllNode//展示实验结果
	return 0.1;
#endif
	
	//if(forward>0)return 0.0001 / (1 + exp(-loss));
	//else return -0.0001 / (1 + exp(-loss));
	if (loss > 30000)return 0.0005;
	else if (loss > 20000)return 0.0004;
	else if (loss > 12500)return 0.0003;
	else if (loss > 11000)return 0.0002;
	else return 0.0001;
	//return forward / 100;
	//return 100;
}