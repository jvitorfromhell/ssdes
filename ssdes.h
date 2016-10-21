#include <string>

using namespace std;

/*
	Classe SBox: Usada para estanciar cada SBox somente uma vez por processo.
*/

class SBox {
private:
	unsigned int** matrix, lines, columns;

public:
	SBox(unsigned int lines, unsigned int columns, string filename);
	~SBox();

	unsigned int get(unsigned int key);
};

/*
	Funções internas do SSDES.
*/
unsigned int E(unsigned int Di_minus_one);
unsigned int F(unsigned int Di_minus_one, unsigned char Ki, SBox &s1, SBox &s2);
unsigned int round(unsigned int block, unsigned char Ki, SBox &s1, SBox &s2);

/*
	Funções auxiliares do SSDES.
*/

unsigned int circular_shift_left(unsigned int Ki);
unsigned int circular_shift_right(unsigned int Ki);

/*
	SSDES propriamente dito: encriptar ou desencriptar um bloco.
*/

unsigned int encrypt(unsigned int block, unsigned int key, unsigned int rounds, SBox &s1, SBox &s2);
unsigned int decrypt(unsigned int block, unsigned int key, unsigned int rounds, SBox &s1, SBox &s2);

/*
	Scripts para criptografar ou desencriptar um arquivo inteiro.
*/

void encrypt_text(string inputFile, string outputFile, unsigned int key, unsigned int rounds);
void decrypt_text(string inputFile, string outputFile, unsigned int key, unsigned int rounds);