#ifndef AROYA_PLA
#define AROYA_PLA

#define MULTI_THREADS//多线程开关
//#define TIME//显示运行时间信息

class Omega {
private:
	double nowPers;
	double bestPer;
public:
	double* data;
	double* best;
	Omega(const int& columns);
	//from file
	Omega();
	void writeFile();
	void writeTest(const vector<double*>&);
	void iterateOnce(Reader&);
	void iterate(Reader&, const int&times);
	int columns;
};

struct thread_bag {
	Reader*reader;
	int cols, begin, end;
	double*weight;
	double*data;
	thread_bag(Reader*reader_, double*data_, double*weight_, int&cols_, int&begin_, int end_) {
		reader = reader_; data = data_; weight = weight_; cols = cols_; begin = begin_; end = end_;
	}
};

struct thread_bag2 {
	Reader*reader;
	int rows, begin, end;
	double*weight;
	double*err;
	double*data;
	thread_bag2(Reader*reader_, double*data_, double*weight_,double*err_, int&rows_, int&begin_, int end_) {
		reader = reader_; data = data_; weight = weight_; rows = rows_; begin = begin_; end = end_; err = err_;
	}
};


#endif