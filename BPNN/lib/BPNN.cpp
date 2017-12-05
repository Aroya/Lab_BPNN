
#include"BPNN.h"

using namespace std;

BPNN::BPNN(const int&Layers) {
	layers = Layers;
	layerW = new double**[Layers];
	layerData = new double*[Layers];
	layerNodes = new int[Layers];
	bias = new double*[Layers];
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
}
void BPNN::setLayerNodes(const int*&Nodes) {
	int i, j, k, l, nodeReader, nodeReader2;
	for (i = 1, j = 0; i < layers; i++, j++) {
		layerNodes[i] = Nodes[i];
		nodeReader = Nodes[i];
		nodeReader2 = layerNodes[j];
		layerW[i] = new double*[nodeReader];
		layerData[i] = new double[nodeReader];
		bias[i] = new double[nodeReader];//new bias
		//layerData[i][layerNodes[i]] = 1;//bias
		for (k = 0; k < nodeReader; k++) {
			//W depends on the nodes before
			layerW[i][k] = new double[nodeReader2];
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
