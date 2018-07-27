#include "Tensor.h"
#include<iostream>
#include<vector>
#include <random>
#include<string>
#include <sstream> 
#include <math.h>
#include <iomanip>
using namespace std;


template <class type> Tensor<type>::Tensor() {

}
template <class type> Tensor<type>::Tensor(vector<int> shape, bool random_initialization) {
	this->rank = shape.size();
	this->shape = shape;
	int size = 1;
	for (vector<int>::iterator it = shape.begin(); it != shape.end(); it++) {
		size *= *it;
	}
	this->size = size;
	data = new type[this->size];
	if (random_initialization && (is_same< type, double>::value)) {
		default_random_engine generator;
		normal_distribution<double> distribution(0, 1);
		for (int i = 0; i < size; i++) {
			this->data[i] = distribution(generator);
		}
	}

}
template <class type> Tensor<type>::~Tensor() {
	delete[] this->data;
}
template <class type>	int Tensor<type>::getRank() {
	return this->rank;
}
template <class type>	int Tensor<type>::getSize() {
	return this->size;
}
template <class type>	vector<int> Tensor<type>::getShape() {
	return this->shape;
}
template <class type>	type* Tensor<type>::getLinearData() {
	return this->data;
}
template <class type>	void Tensor<type>::setLinearData(type* new_data, vector<int> shape) {
	delete[] this->data;
	int size = 1;
	for (vector<int>::iterator it = shape.begin(); it != shape.end(); it++) {
		size *= *it;
	}
	this->size = size;
	this->rank = shape.size();
	this->shape = shape;
	this->data = new_data;
}
template <class type>	type* Tensor<type>::getDataFrom(string pattern) {
	//TODO
	return this->data;
}
template <class type>	string Tensor<type>::toString()
{
	int rank = this->getRank();
	int* shape = new int[rank];
	for (int i = 0; i < rank; i++)
		shape[i] = this->getShape()[i];
	string s = string(rank, '[');
	int num_brackets = 0;
	for (int i = 1; i <= this->getSize(); i++) {
		num_brackets = getNumBrackets(i, rank, shape);
		s.append(to_string(this->data[i - 1]));
		if (num_brackets > 0)
			s.append(string(num_brackets, ']') + ",\n" + string(rank - num_brackets, ' ') + string(num_brackets, '['));
		else
			s.append(", ");
	}
	delete[] shape;
	return s.substr(0, s.size() - rank - 2);
}
template <class type>	void Tensor<type>::expandDim() {
	this->rank++;
	this->shape.push_back(1);
}
template <class type> Tensor<double>& Tensor<type>::matmul(Tensor<double> arg1, Tensor<double> arg2) {
	try
	{
		if ((arg1.getRank() > 2) || (arg2.getRank() > 2) || (arg1.getShape().at(1) != arg2.getShape().at(0))) {
			throw 0;
		}
	}
	catch (int e)
	{
		cout << "Tensors not compatible for matrix multiplication" << e << '\n';
	}
	if (arg1.getRank() == 1) {
		arg1.expandDim();
	}
	if (arg1.getRank() == 1) {
		arg2.expandDim();
	}
	int out_size = arg1.getShape().at(0)*arg2.getShape().at(1);
	vector<int> out_shape{ arg1.getShape().at(0),arg2.getShape().at(1) };
	double* out_data = new double[out_size];
	int out_index = 0;
	double sum = 0.0;
	int rows = arg1.getShape().at(0);
	int columns = arg1.getShape().at(1);
	int columns_out = arg2.getShape().at(1);
	double* mat1 = arg1.getLinearData();
	double* mat2 = arg2.getLinearData();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns_out; j++) {
			for (int k = 0; k < columns; k++)
				sum += mat1[i*columns + k] * mat2[j + k * columns_out];
			out_data[out_index] = sum;
			out_index++;
			sum = 0.0;
		}
	}
	Tensor<double>* out = new Tensor<double>;
	out->setLinearData(out_data, out_shape);
	return *out;
}
template <class type> void Tensor<type>::broadcast(Tensor<type>& this_tensor, Tensor<type>& other_tensor) {
	int this_rank = this_tensor.rank;
	int other_rank = other_tensor.rank;
	int dif_rank = abs(this_rank - other_rank);
	vector<int> this_shape = this_tensor.shape;
	vector<int> other_shape = other_tensor.shape;
	vector<int>::iterator it;
	if (this_rank >= other_rank) {
		for (int i = 0; i < dif_rank; i++) {
			it = other_shape.begin();
			other_shape.insert(it, 1);
		}

	}
	else {
		for (int i = 0; i < dif_rank; i++) {
			it = this_shape.begin();
			this_shape.insert(it, 1);
		}
	}
	int out_rank = this_shape.size();
	int out_size = 1;
	for (int i = 0; i < out_rank; i++)
		if (this_shape.at(i) > other_shape.at(i))
			out_size *= this_shape.at(i);
		else
			out_size *= other_shape.at(i);
	type * this_data = new type[out_size];
	type * other_data = new type[out_size];
	try {
		int current_dim_prod = 1;
		int this_curser = 0;
		int other_curser = 0;
		for (int i = out_rank - 1; i >= 0; i--) {

			if (this_shape.at(i) != other_shape.at(i))
				if (this_shape.at(i) == 1) {
					this_shape.at(i) = other_shape.at(i);

				}
				else if (other_shape.at(i) == 1)
				{
					other_shape.at(i) = this_shape.at(i);

				}
				else
					throw 0;

		}
	}
	catch (int e)
	{
		delete[] this_data;
		delete[] other_data;
		cout << "Tensors not compatible for broadcast" << e << '\n';
	}
	int out_current_dim_prod = 1;
	int this_current_dim_prod = 1;
	int other_current_dim_prod = 1;
	int j = out_rank - 1;
	for (int i = 0; i < out_size; i++) {
		if (j >= 0) {
			if (i%out_current_dim_prod == 0) {
				out_current_dim_prod *= this_shape.at(j);
				this_current_dim_prod *= this_tensor.shape.at(j);
				other_current_dim_prod *= other_tensor.shape.at(j);
				j--;
			}
		}

		other_data[i] = other_tensor.data[i%other_current_dim_prod];
		this_data[i] = this_tensor.data[i%this_current_dim_prod];
	}
	this_tensor.setLinearData(this_data, this_shape);
	other_tensor.setLinearData(other_data, other_shape);
}
template <class type>	Tensor<type>& Tensor<type>::operator=(const Tensor<type>& other) {
	type* this_data = new type[other.size];
	for (int i = 0; i < other.size; i++)
		this_data[i] = other.data[i];
	this->setLinearData(this_data, other.shape);
	return *this;
}

template <class type>	Tensor<type>& Tensor<type>::operator+(Tensor<type> other) {
	Tensor<type> this_tensor;
	this_tensor = *this;
	Tensor<type> other_tensor;
	other_tensor = other;
	broadcast(this_tensor, other_tensor);
	Tensor<type>* out = new Tensor<type>;
	vector<int> out_shape = this_tensor.shape;
	type * out_data = new type[this_tensor.size];
	for (int i = 0; i < this_tensor.size; i++) {
		out_data[i] = this_tensor.data[i] + other_tensor.data[i];
	}
	out->setLinearData(out_data, out_shape);
	return *out;
}
template <class type>	Tensor<type>& Tensor<type>::operator*(Tensor<type> other) {
	Tensor<type> this_tensor;
	this_tensor = *this;
	Tensor<type> other_tensor;
	other_tensor = other;
	broadcast(this_tensor, other_tensor);
	Tensor<type>* out = new Tensor<type>;
	vector<int> out_shape = this_tensor.shape;
	type * out_data = new type[this_tensor.size];
	for (int i = 0; i < this_tensor.size; i++) {
		out_data[i] = this_tensor.data[i] * other_tensor.data[i];
	}
	out->setLinearData(out_data, out_shape);
	return *out;
}
template <class type> string Tensor<type>::to_string(type const& value) {
	ostringstream sstr;
	sstr << fixed << setprecision(5) << value;
	string s = sstr.str();
	if (value > 0)
		s = ' ' + s;
	return s;
}

template <class type>	int Tensor<type>::getNumBrackets(int i, int rank, int* shape) {
	int result = 0;
	for (int j = rank - 1; j >= 0; j--) {
		if (i%shape[j] == 0) {
			result++;
			i /= shape[j];
		}
		else
			break;
	}
	return result;
}

