#include "ssdes.h"
#include <iostream>
#include <fstream>

/*
	Construtor da classe SBox.
*/

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

/*
	Destrutor da classe SBox.
*/

SBox::~SBox() {
	for (unsigned int i = 0; i < lines; i++) {
		delete(matrix[i]);
	}
	delete(matrix);
}

/*
	get: Retorna o valor da matrix da SBox correspondente ao valor de entrada.
*/

unsigned int SBox::get(unsigned int position) {
	return matrix[(position & 8) >> 3][position & 7];
}

/*
	E: expande o valor do bloco de entrada de acordo com a definição do SSDES.
*/

unsigned int E(unsigned int Di_minus_one) {
	unsigned int first_two_bits = (Di_minus_one & 48) >> 4; 
	unsigned int third_bit = (Di_minus_one & 8) >> 3;
	unsigned int fourth_bit = (Di_minus_one & 4) >> 2;
	unsigned int last_two_bits = Di_minus_one & 3;
	return (first_two_bits << 6) | (fourth_bit << 5) | (third_bit << 4) | (fourth_bit << 3) | (third_bit << 2) | (last_two_bits);
}

/*
	F: calcula bloco atual expandido xor subchave atual e depois passa esse valor pelas SBoxes.
*/

unsigned int F(unsigned int Di_minus_one, unsigned char Ki, SBox &s1, SBox &s2) {
	unsigned int expanded = E(Di_minus_one);
	unsigned int pre_SBoxes = expanded ^ Ki;
	
	return (s1.get((pre_SBoxes & 240) >> 4) << 3) | (s2.get(pre_SBoxes & 15));
}

/*
	round: Retorna parte alta do bloco atual xor F(parte baixa do bloco atual).
*/

unsigned int round(unsigned int block, unsigned char Ki, SBox &s1, SBox &s2) {
	unsigned int Ei_minus_one = (block & 4032) >> 6;
	unsigned int Di_minus_one = (block & 63);

	unsigned int Ei = Di_minus_one;
	unsigned int Di = Ei_minus_one ^ F(Di_minus_one, Ki, s1, s2);

	return (Ei << 6) | Di;
}

/*
	circular_shift_left: shifta o valor de entrada pra esquerda, com carry bit.
*/

unsigned int circular_shift_left(unsigned int Ki) {
	if ((Ki & 256) == 0) {
		return (Ki << 1) & (511);
	}
	else {
		return ((Ki << 1) | 1) & (511);
	}
}

/*
	circular_shift_right: shifta o valor de entrada pra direita, com carry bit.
*/

unsigned int circular_shift_right(unsigned int Ki) {
	if ((Ki & 1) == 0) {
		return (Ki >> 1) & (511);
	}
	else {
		return ((Ki >> 1) | 256) & (511);
	}
}

/*
	encrypt: Encripta um bloco de 12 bits, usando uma chave de 9 bits, utilizando o algoritmo SSDES.
			 O número de rodadas é definido por rounds.
*/

unsigned int encrypt(unsigned int block, unsigned int key, unsigned int rounds, SBox &s1, SBox &s2) {

	unsigned int Ki = key & 510;

	for (unsigned int i = 0; i < rounds; i++) {
		block = round(block, (unsigned char)(Ki >> 1), s1, s2);
		Ki = circular_shift_left(Ki);
	}

	unsigned int Dn = (block & 63);
	unsigned int En = (block & 4032) >> 6;
	return (Dn << 6) | En;
}

/*
	decrypt: Desencripta um bloco de 12 bits, usando uma chave de 9 bits, utilizand o algoritmo SSDES.
			 O numero de rodadas é definido por rounds.
*/

unsigned int decrypt(unsigned int block, unsigned int key, unsigned int rounds, SBox &s1, SBox &s2) {

	unsigned int Ki = key & 510;

	for (unsigned int i = 0; i < rounds - 1; i++) {
		Ki = circular_shift_left(Ki);
	}

	for (unsigned int i = 0; i < rounds; i++) {
		block = round(block, (unsigned char)(Ki >> 1), s1, s2);
		Ki = circular_shift_right(Ki);
	}

	unsigned int Dn = (block & 63);
	unsigned int En = (block & 4032) >> 6;
	return (Dn << 6) | En;
}

/*
	encrypt_text: Encripta um arquivo inteiro utilizando o SSDES, com a chave key.
*/

void encrypt_text(string inputFile, string outputFile, unsigned int key, unsigned int rounds) {
	ifstream input(inputFile);
	ofstream output(outputFile);
	unsigned char c1, c2, c3;
	unsigned int curr_block_1, curr_block_2, encrypted_1, encrypted_2, length, curr = 0;

	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");

	input.seekg (0, input.end);
    length = input.tellg();
    input.seekg (0, input.beg);

	while (curr < length) {
		input >> noskipws >> c1;
		++curr;
		if (curr < length) {
			input >> noskipws >> c2;
			++curr;
			if (curr < length) {
				input >> noskipws >> c3;
				++curr;
			}
			else {
				c3 = '_';
			}
		}
		else {
			c2 = '_';
			c3 = '_';
		}

		curr_block_1 = (c1 << 4) | (c2 >> 4);
		curr_block_2 = ((c2 & 15) << 8) | c3;

		encrypted_1 = encrypt(curr_block_1, key, rounds, s1, s2);
		encrypted_2 = encrypt(curr_block_2, key, rounds, s1, s2);

		c1 = encrypted_1 >> 4;
		c2 = ((encrypted_1 & 15) << 4) | ((encrypted_2 & 3840) >> 8);
		c3 = encrypted_2 & 255;

		output << c1;
		output << c2;
		output << c3;	
	}

	input.close();
	output.close();

}

/*
	decrypt_text: Desencripta um arquivo inteiro utilizando o SSDES, com a chave key.
*/

void decrypt_text(string inputFile, string outputFile, unsigned int key, unsigned int rounds) {
	ifstream input(inputFile);
	ofstream output(outputFile);
	unsigned char c1, c2, c3;
	unsigned int curr_block_1, curr_block_2, decrypted_1, decrypted_2, length, curr = 0;

	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");

	input.seekg (0, input.end);
    length = input.tellg();
    input.seekg (0, input.beg);

	while (curr < length) {
		input >> noskipws >> c1;
		++curr;
		if (curr < length) {
			input >> noskipws >> c2;
			++curr;
			if (curr < length) {
				input >> noskipws >> c3;
				++curr;
			}
			else {
				c3 = '_';
			}
		}
		else {
			c2 = '_';
			c3 = '_';

		}

		curr_block_1 = (c1 << 4) | (c2 >> 4);
		curr_block_2 = ((c2 & 15) << 8) | c3;

		decrypted_1 = decrypt(curr_block_1, key, rounds, s1, s2);
		decrypted_2 = decrypt(curr_block_2, key, rounds, s1, s2);

		c1 = decrypted_1 >> 4;
		c2 = ((decrypted_1 & 15) << 4) | ((decrypted_2 & 3840) >> 8);
		c3 = decrypted_2 & 255;

		output << c1;
		output << c2;
		output << c3;
	}

	input.close();
	output.close();
}