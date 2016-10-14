#include "ssdes.h"
#include <iostream>
#include <fstream>
#include <bitset>

SBox::SBox(unsigned int lines, unsigned int columns, string filename) {
	ifstream arq(filename);

	this->lines = lines;
	this->columns = columns;

	matrix = new unsigned int*[lines];

	for (unsigned int i = 0; i < lines; i++) {
		matrix[i] = new unsigned int[columns];
		for (unsigned int j = 0; j < columns; j++) {
			arq >> matrix[i][j];
		}
	}

	arq.close();

}

SBox::~SBox() {
	for (unsigned int i = 0; i < lines; i++) {
		delete(matrix[i]);
	}
	delete(matrix);
}

unsigned int SBox::get(unsigned int position) {
	return matrix[(position & 8) >> 3][position & 7];
}

unsigned int E(unsigned int Di_minus_one) {
	unsigned int first_two_bits = (Di_minus_one & 48) >> 4; 
	unsigned int third_bit = (Di_minus_one & 8) >> 3;
	unsigned int fourth_bit = (Di_minus_one & 4) >> 2;
	unsigned int last_two_bits = Di_minus_one & 3;
	return (first_two_bits << 6) | (fourth_bit << 5) | (third_bit << 4) | (fourth_bit << 3) | (third_bit << 2) | (last_two_bits);
}

unsigned int F(unsigned int Di_minus_one, unsigned char Ki, SBox &s1, SBox &s2) {
	unsigned int expanded = E(Di_minus_one);
	unsigned int pre_SBoxes = expanded ^ Ki;
	
	return (s1.get((pre_SBoxes & 240) >> 4) << 3) | (s2.get(pre_SBoxes & 15));
}

unsigned int round(unsigned int block, unsigned char Ki, SBox &s1, SBox &s2) {
	unsigned int Ei_minus_one = (block & 4032) >> 6;
	unsigned int Di_minus_one = (block & 63);

	unsigned int Ei = Di_minus_one;
	unsigned int Di = Ei_minus_one ^ F(Di_minus_one, Ki, s1, s2);

	return (Ei << 6) | Di;
}

unsigned char circular_shift_left(unsigned char Ki) {
	if ((Ki & 128) == 0) {
		return Ki << 1;
	}
	else {
		return (Ki << 1) | 1;
	}
}

unsigned char circular_shift_right(unsigned char Ki) {
	if ((Ki & 1) == 0) {
		return Ki >> 1;
	}
	else {
		return (Ki >> 1) | 128;
	}
}

unsigned int encrypt(unsigned int block, unsigned int key, unsigned int rounds) {
	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");

	unsigned char Ki = (key & 510) >> 1;

	for (unsigned int i = 0; i < rounds; i++) {
		block = round(block, Ki, s1, s2);
		Ki = circular_shift_left(Ki);
	}

	unsigned int Dn = (block & 63);
	unsigned int En = (block & 4032) >> 6;
	return (Dn << 6) | En;
}

unsigned int decrypt(unsigned int block, unsigned int key, unsigned int rounds) {
	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");

	unsigned char Ki = (key & 510) >> 1;

	for (unsigned int i = 0; i < rounds - 1; i++) {
		Ki = circular_shift_left(Ki);
	}

	for (unsigned int i = 0; i < rounds; i++) {
		block = round(block, Ki, s1, s2);
		Ki = circular_shift_right(Ki);
	}

	unsigned int Dn = (block & 63);
	unsigned int En = (block & 4032) >> 6;
	return (Dn << 6) | En;
}

void encrypt_text(string filename, unsigned int key) {
	ifstream input(filename);
	ofstream output(filename.append("_encrypted"));
	unsigned char c1, c2, c3;
	unsigned int curr_block_1, curr_block_2, encrypted_1, encrypted_2;

	input >> c1;
	input >> c2;
	input >> c3;

	curr_block_1 = (c1 << 4) | (c2 >> 4);
	curr_block_2 = ((c2 & 15) << 8) | c3;

	encrypted_1 << encrypt(curr_block_1, key, 16);
	encrypted_2 << encrypt(curr_block_2, key, 16);

	c1 = encrypted_1 >> 4;
	c2 = ((encrypted_1 & 15) << 4) | ((encrypted_2 & 3840) >> 8);
	c3 = encrypted_2 & 255;

	output << c1 << c2 << c3;

	input.close();
	output.close();

}

void decrypt_text(string filename, unsigned int key) {
	ifstream input(filename);
	ofstream output(filename.append("_decrypted"));
	unsigned char c1, c2, c3;
	unsigned int curr_block_1, curr_block_2, decrypted_1, decrypted_2;

	input >> c1;
	input >> c2;
	input >> c3;

	curr_block_1 = (c1 << 4) | (c2 >> 4);
	curr_block_2 = ((c2 & 15) << 8) | c3;

	decrypted_1 << decrypt(curr_block_1, key, 16);
	decrypted_2 << decrypt(curr_block_2, key, 16);

	c1 = decrypted_1 >> 4;
	c2 = ((decrypted_1 & 15) << 4) | ((decrypted_2 & 3840) >> 8);
	c3 = decrypted_2 & 255;

	output << c1 << c2 << c3;

	input.close();
	output.close();
}