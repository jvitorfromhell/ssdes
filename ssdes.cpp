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

}

int SBox::get(int position) {
	return matrix[position & 8 >> 3][position & 7];
}