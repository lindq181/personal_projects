#include "Matrix.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using namespace std;

Matrix::Matrix(int i, int j) {
	//:num rows (i), num cols (j)

	cout << " calling integer constructor for Matrix \n";
	
	rows = i;
	cols = j;
	int count;

	float **rowArray;
	rowArray = new float*[i];

	for (count = 0; count < i; count++) {
		rowArray[i] = new float[j];
	}

	data = rowArray;

		
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
	//return (float*)(data + ((i * cols) + j));
	return &data[i][j];
}


//overload the << operator
std::ostream& operator<<(std::ostream &os, Matrix &m){
	//float *counter = m;

	//first prints out num rows and num cols
	os << m.rows << " " << m.cols << "\n";
	
	//then goes through each row printing out each value
	for (int i = 0; i < m.rows; i ++){
		for (int j = 0; j < m.cols; j++){
			os << (m.data[i,j]) << " ";
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
	int row = 0;
	int column = 0;
	float dataHolder;
	Matrix result;
	//bool createM = false;
	fin.open(filename.c_str(), std::ifstream::in);
	int count = 0; //keeps track of where we are in the file

	if(fin.is_open()) {

		while(!fin.eof()) {

			if( count == 0) {	//first element is num rows
				//rows = fin;
				fin >> row;
				count ++;
				//createM = true;
			} else if(count == 1) {	//second element is num cols
				//cols = fin;
				fin >> column;
				count ++;
				result = Matrix(row, column);
			} else {
				//*(data + (count -2)) = fin;	//the rest are all matrix data
				fin >> (**(result.data + (count - 2)));
			//fin >> (**result.data + (count - 2));
			}
		}
	//result.data = dataholder;
	fin.close();
	}

	return result;
}


