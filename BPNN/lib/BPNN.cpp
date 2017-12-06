#include"BPNN.h"
#include<cmath>
using namespace std;

double defaultActive(const double&t) { return t; }
double defaultActiveD(const double&t) { return 1.0; }
double sigmoid(const double&t) { return 1.0 / (1.0 + exp(-t)); }
double sigmoidD(const double&t) { return 1.0 / (2.0 + exp(t) + exp(-t)); }

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
	rate = 0.001;
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
void BPNN::setLayerNodes(const int*&Nodes) {
	int i, j, k, l, nodeReader, nodeReader2;
	for (i = 1, j = 0; i < layers; i++, j++) {
		layerNodes[i] = Nodes[j];
		nodeReader = Nodes[i];
		nodeReader2 = layerNodes[j];
		layerW[i] = new double*[nodeReader];
		fixW[i] = new double*[nodeReader];
		layerData[i] = new double[nodeReader];
		activation[i] = new double[nodeReader];
		bias[i] = new double[nodeReader];//new bias
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
void BPNN::setInputData(const double*&Data) {
	int nodesReader = layerNodes[0];
	for (int i = 0; i < nodesReader; i++) {
		layerData[0][i] = Data[i];
	}
}

void BPNN::clearData() {
	int i, j, k, p, q, m;
	for (i = 1, p = 0; i < layers; i++, p++) {
		j = layerNodes[i];
		m = layerNodes[j];
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
				sum += layerData[j][l] * layerW[i][k][l];
			}
			//bias
			sum += bias[i][k];
			layerData[i][k] = sum;
			activation[i][k] = active(sum);
		}
	}
}
void BPNN::setExpectData(const double*&Data) {
	int i = layers - 1;
	int j = layerNodes[i];
	for (int k = 0; k < j; k++) {
		expected[i][k] = Data[k];
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
			biasDiff = 2.0 * (expected[i][j] - layerData[i][j])*activeD(layerData[i][j]);
			fixBias[i][j] += biasDiff;
			//W_i & X_i
			for (l = 0; l < nodesReader2; l++) {
				fixW[i][j][l] += biasDiff*layerData[k][l];
				expected[k][l] += biasDiff*layerW[i][j][l];
			}
		}
		//average X_i after all train
		//for (l = 0; l < nodesReader2; l++) {
		//	expected[k][l] /= double(nodesReader);
		//}
		//learning
		//after run all train
		//this function process one train
	}
}
void BPNN::runGroup(const double**&group, const double**&flag,const int&groups,
	double(*active)(const double&), double(*activeD)(const double&)) {
	clearData();
	for (int i = 0; i < groups; i++) {
		setInputData(group[i]);
		updateLayers(active);
		setExpectData(flag[0]);
		updateParameter(activeD);

	}
}