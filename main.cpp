#include <iostream>
#include "ssdes.h"

using namespace std;

int main() {
	SBox s1(2, 8, "sbox1.txt"), s2(2, 8, "sbox2.txt");
	cout << "SBoxes inicializadas\n";
	cout << s1.get(6) << " " << s2.get(14) << endl;
	return 0;
}