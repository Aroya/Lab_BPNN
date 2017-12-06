#ifndef AROYA_BPNN
#define AROYA_BPNN


double defaultActive(const double&t);
double defaultActiveD(const double&t);
double sigmoid(const double&t);
double sigmoidD(const double&t);
//first use BPNN to init
//second use setInputData to init inputArray
//then use setLayerNodes to init LayerArray
//last use setAnsNodes to init AnsArray
class BPNN {
private:
	//first index point to  
	//hidden_layout[layer0....layerN]
	//second index point to the Node
	//third index point to the W_i
	double ***layerW;
	//record nodes of every layer
	int *layerNodes;
	//record count of layer
	int layers;
	//for caculate Y=WX
	//also the X_i for next layer
	double **layerData;
	//after activation function
	double **activation;
	//bias
	double **bias;
	//write 0 to layerData
	void clearData();
	//record expected value
	double **expected;
	//expect fixed value of W
	double ***fixW;
	//expect fixed value of bias
	double **fixBias;
	//learing rate
	double rate;
	//after update learn from the data
	void learn(const int&groups);
	double forward;

	//Í³¼Æ
	double loss;
	double dynamicRate();
public:
	//contain input,hidden and output
	BPNN(const int&CountOfLayers);
	//set input array size
	void setInputNodes(const int&Nodes);
	//copy input data to first index of private data
	void setInputData(double* NodeDataArray);
	//set layer array size(not contain input)
	void setLayerNodes(int* Nodes);
	//Get Ans
	void updateLayers(double(*)(const double&) = defaultActive);
	//Update W and bias
	void updateParameter(double(*)(const double&) = defaultActiveD);
	//input expect value(output node)
	void setExpectData(double*Data,double(*)(const double&));
	//run with group data
	void runGroup(double**groupData, double**flag, const int&groups,
		double(*)(const double&) = defaultActive, double(*)(const double&) = defaultActiveD);
	//set learning rate
	void setRate(const double&t) { rate = t; }
};



#endif