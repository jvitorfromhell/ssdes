#include "ssdes.h"
#include <iostream>
#include <fstream>

SBox::SBox(int lines, int columns, string filename) {
	ifstream arq(filename);

	this->lines = lines;
	this->columns = columns;

	matrix = new int*[lines];

	for (int i = 0; i < lines; i++) {
		matrix[i] = new int[columns];
		for (int j = 0; j < columns; j++) {
			arq >> matrix[i][j];
		}
	}

	arq.close();

}

SBox::~SBox() {
	for (int i = 0; i < lines; i++) {
		delete(matrix[i]);
	}
	delete(matrix);
}

int SBox::get(int position) {
	return matrix[position & 8 >> 3][position & 7];
}

int E(int Di_minus_one) {
	int first_two_bits = (Di_minus_one & 48) >> 4; 
	int third_bit = (Di_minus_one & 8) >> 3;
	int fourth_bit = (Di_minus_one & 4) >> 2;
	int last_two_bits = Di_minus_one & 3;
	return (first_two_bits << 6) | (fourth_bit << 5) | (third_bit << 4) | (fourth_bit << 3) | (third_bit << 2) | (last_two_bits);
}