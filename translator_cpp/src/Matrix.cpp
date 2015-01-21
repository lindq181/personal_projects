#include "Matrix.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using namespace std;

Matrix::Matrix(int i, int j) {
	//:num rows (i), num cols (j)

	rows = i;
	cols = j;

	*data = 0;	
}

//makes a new matrix out of a given matrix m
Matrix::Matrix(const Matrix& m) {
	
	rows = m.rows;
	cols = m.cols;

	data = m.data;

	
}


//returns number of rows
int Matrix::numRows(){
	return rows;
}

//returns number of columns
int Matrix::numCols(){
	return cols;
}

//returns the value in the matrix element i,j
float *Matrix::access(const int i, const int j) const {
	return (float*)(data + ((i * cols) + j));
}


//overload the << operator
std::ostream& operator<<(std::ostream &os, Matrix &m){
	//float *counter = m;

	//first prints out num rows and num cols
	os << m.rows << " " << m.cols << "\n";
	
	//then goes through each row printing out each value
	for (int i = 0; i < m.rows; i ++){
		for (int j = 0; j < m.cols; j++){
			os << &m << " ";
			//counter ++;
		}
		os << "\n";	//newline after a completed row
	}
	
	return os;
}

//read a matrix from a file
Matrix Matrix::readMatrix(std::string filename){
	
	ifstream fin;
	//Matrix result = new Matrix();
	int rowHolder;
	int colHolder;
	float *dataHolder;
	Matrix result;
	bool createM = false;
	fin.open(filename.c_str(), std::ifstream::in);
	int count = 0; //keeps track of where we are in the file
	if(fin.is_open()){
		while(!fin.eof()){
			if( count == 0){	//first element is num rows
				//rows = fin;
				fin >> rowHolder;
				count ++;
				createM = true;
			}
			else if(count == 1){	//second element is num cols
				//cols = fin;
				fin >> colHolder;
				count ++;
				result = Matrix(rowHolder, colHolder);
			}
			//else
				//*(data + (count -2)) = fin;	//the rest are all matrix data
			//fin >> (*dataHolder + (count - 2));
			//}
		}
	//result.data = dataholder;
	fin.close();
	}

	return result;
}


