#ifndef TENSOR_H
#define TENSOR_H


#include<iostream>
#include<vector>
#include <random>
#include<string>
#include <sstream> 
#include <math.h>
#include <iomanip>
using namespace std;
template<class type> class Tensor {
public:
	Tensor();
	Tensor(vector<int> shape, bool random_initialization);
	~Tensor();
	int getRank();
	int getSize();
	vector<int> getShape();
	type* getLinearData();
	void setLinearData(type* new_data, vector<int> shape);
	type* getDataFrom(string pattern);
	string toString();
	void expandDim();
	static Tensor<double>& matmul(Tensor<double> arg1, Tensor<double> arg2);
	void static broadcast(Tensor<type>& this_tensor, Tensor<type>& other_tensor);
	Tensor<type>& operator=(const Tensor<type>& other);

	Tensor<type>& operator+(Tensor<type> other);
	Tensor<type>& operator*(Tensor<type> other);
protected:
	type * data;
	int rank;
	int size;
	vector<int> shape;
	static string to_string(type const& value);
private:
	int getNumBrackets(int i, int rank, int* shape);
};
#endif