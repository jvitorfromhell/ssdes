#include <iostream>
#include "ssdes.h"

using namespace std;

int main() {
	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");
	cout << "Testando F: Valor de Di_minus_one: 45; Valor de Ki: 10; \n";
	cout << "Valor de F(Di_minus_one, Ki): " << F(45, 10, s1, s2) << endl;
	return 0;
}