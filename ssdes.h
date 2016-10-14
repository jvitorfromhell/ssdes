#include <string>

using namespace std;

class SBox {
private:
	unsigned int** matrix, lines, columns;

public:
	SBox(unsigned int lines, unsigned int columns, string filename);
	~SBox();

	unsigned int get(unsigned int key);
};

unsigned int E(unsigned int Di_minus_one);
unsigned int F(unsigned int Di_minus_one, unsigned char Ki, SBox &s1, SBox &s2);
unsigned int round(unsigned int block, unsigned char Ki, SBox &s1, SBox &s2);

unsigned char circular_shift_left(unsigned char Ki);
unsigned char circular_shift_right(unsigned char Ki);

unsigned int encrypt(unsigned int block, unsigned int key, unsigned int rounds);
unsigned int decrypt(unsigned int block, unsigned int key, unsigned int rounds);

void encrypt_text(string filename, unsigned int key);
void decrypt_text(string filename, unsigned int key);