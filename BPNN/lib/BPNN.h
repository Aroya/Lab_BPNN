#ifndef AROYA_BPNN
#define AROYA_BPNN

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
public:
	//contain input,hidden and output
	BPNN(const int&CountOfLayers);
	//set input array size
	void setInputNodes(const int&Nodes);
	//copy input data to first index of private data
	void setInputData(const double*&NodeDataArray);
	//set layer array size(not contain input)
	void setLayerNodes(const int*&Nodes);
	//Get Ans
	void updateLayers(double(*)(const double&) = defaultActive);
	//Update W and bias
	void updateParameter(double(*)(const double&) = defaultActiveD);
	//input expect value(output node)
	void setExpectData(const double*&Data);
	//run with group data
	void runGroup(const double**&groupData,const int&groups);
	//set learning rate
	void setRate(const double&t) { rate = t; }
};

#endif