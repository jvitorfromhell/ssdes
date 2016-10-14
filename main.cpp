#include <iostream>
#include "ssdes.h"

using namespace std;

int main() {
	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");
	cout << "Testando round: Valor de block: 45; Valor de Ki: 10; \n";
	cout << "Valor de round(block, Ki): " << round(45, 10, s1, s2) << endl;
	return 0;
}