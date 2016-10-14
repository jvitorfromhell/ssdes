#include <iostream>
#include "ssdes.h"

using namespace std;

int main() {
	cout << "Valor de encrypt para 45, chave 10 e 8 rodadas: " << encrypt(45, 10, 8) << endl;
	cout << "Valor de decrypt para 2607, chave 10 e 8 rodadas: " << decrypt(2607, 10, 8) << endl;
	return 0;
}