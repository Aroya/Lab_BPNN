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
	//bias
	double **bias;
	//write 0 to layerData
	void clearData();
	//record expected value
	double **expected;
public:
	//contain input,hidden and output
	BPNN(const int&CountOfLayers);
	//set input array size
	void setInputNodes(const int&Nodes);
	//copy input data to first index of private data
	void setInputData(const double*&NodeDataArray);
	//set layer array size
	void setLayerNodes(const int*&Nodes);
	//Get Ans
	void updateLayers();
	//Update W and bias
	void updateParameter();
	//input expect value(output node)
	void setExpectData(const double*&Data);
};

#endif