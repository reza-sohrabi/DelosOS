#include<iostream>
#include<vector>
#include <random>
#include<string>
#include <sstream> 
using namespace std;
template<class type> class Tensor {
public:
	Tensor(vector<int> shape, bool randomInitialization) {
		this->rank = shape.size();
		this->size = 1;
		this->shape = shape;
		for (vector<int>::iterator it = shape.begin(); it != shape.end(); it++) {
			size *= *it;
		}
		data = new type[this->size];
		if (randomInitialization && (is_same< type, double>::value)) {
			default_random_engine generator;
			normal_distribution<double> distribution(0, 1);
			for (int i = 0; i < size; i++) {
				this->data[i] = distribution(generator);
			}
		}

	}
	int getRank() {
		return this->rank;
	}
	int getSize() {
		return this->size;
	}
	vector<int> getShape() {
		return this->shape;
	}
	type* getLinearData() {
		return this->data;
	}
	type* getDataFrom(string pattern) {

		return this->data;
	}

	string toString() {
		
		int counter = 0;
		vector<string> vec;
		string row = "[";
		for (int j = 1; j <= this->size; j++) {
			if (j%this->shape[this->rank - 1] != 0) {
				row.append(to_string(this->data[j-1]) + ", ");
			}
			else {
				row.append(to_string(this->data[j-1]) + "]");
					vec.push_back(row);
				row = "[";
			}
		}
		vector<string>* vecs = new vector<string>[this->rank];
		hi this
		vecs[this->rank - 1] = vec;
		for (int i = this->rank - 1; i > 0; i--) {
			row = "[";
			for (int j = 1; j <= vecs[i].size(); j++) {
				if (j%this->shape[i-1] != 0) {
					row.append( " "+vecs[i].at(j-1)+ ",\n ");
				}
				else {
					row.append(vecs[i].at(j-1) + "]");
					vecs[i-1].push_back(row);
					row = "[";
				}

			}
		}
		

		
		return vecs[0].at(0);

	}
protected:
	type * data;
	int rank;
	int size;
	vector<int> shape;
	static string to_string(type const& value) {
		ostringstream sstr;
		sstr << value;
		return sstr.str();
	}

};
int main() {
	double sa;

	vector<int> s;
	s.push_back(4);
	s.push_back(3);
	s.push_back(2);
	s.push_back(2);
	Tensor<double> T(s, true);
	cout << T.toString();
	char c = getchar();
	return 0;

}